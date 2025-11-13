#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include "level.h"
#include "logic.h"
#include "server.h"

int server_socket = -1;

ssize_t read_n_bytes(int sock, void* buf, size_t n) {
    size_t total = 0;
    while (total < n) {
        ssize_t r = read(sock, (char*)buf + total, n - total);
        if (r <= 0)
            return r;
        total += r;
    }
    return total;
}

void handle_sigint(int sig) {
    printf("\nCaught SIGINT, shutting down server...\n");
    if (server_socket >= 0) {
        close(server_socket);
    }
    exit(EXIT_SUCCESS);
}

void* client_thread(void* arg) {
    client_info_t* client = (client_info_t*)arg;
    int client_fd = client->sock;
    int id = client->id;
    char *level_name = client->level_name;

    printf("Client %d connected\n", id);

    char buffer[256];

    Level* original_level = load_level(level_name);

    Level* current_level = malloc(sizeof(Level));

    copy_level(current_level, original_level);

    if (!current_level) {
        printf("Failed to load level!\n");
        exit(EXIT_FAILURE);
    }

    send_level(client_fd, current_level);

    while (1) {

        int move[2];
        ssize_t n = read_n_bytes(client_fd, move, sizeof(move));

        if (n == sizeof(move)) {
            int dx = move[0];
            int dy = move[1];

            try_move(current_level, dx, dy);
            send_level(client_fd, current_level);
        } else if (n == 0) {
            printf("Client disconnected\n");
            break;
        } else {
            perror("read failed");
            break;
        }
    }

    free_level(current_level);
    free_level(original_level);

    close(client_fd);
    free(client);
    return NULL;
}

int setup_server_socket(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 10) < 0) {
        perror("Listen failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    return sock;
}

int main(int argc, char* argv[]) {
    int port;
    char* level_name;

    if (argc < 3) {
        printf("Usage: %s <port> <level_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);

    level_name = argv[2];

    signal(SIGINT, handle_sigint);

    int server_socket = setup_server_socket(port);

    printf("Server listening on port %d...\n", port);

    int client_id = 0;

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        client_info_t* info = malloc(sizeof(client_info_t));
        info->sock = client_fd;
        info->id = client_id++;
        info->level_name = level_name; 

        pthread_t tid;
        pthread_create(&tid, NULL, client_thread, info);
        pthread_detach(tid);
    }

    close(server_socket);
    return 0;
}