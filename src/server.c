#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "level.h"
#include "logic.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int server_socket = -1;


ssize_t read_n_bytes(int sock, void *buf, size_t n) {
    size_t total = 0;
    while (total < n) {
        ssize_t r = read(sock, (char*)buf + total, n - total);
        if (r <= 0) return r; // error or closed
        total += r;
    }
    return total;
}


void handle_sigint(int sig)
{
    printf("\nCaught SIGINT, shutting down server...\n");
    if (server_socket >= 0)
    {
        close(server_socket);
    }
    exit(EXIT_SUCCESS);
}

void run_server(int port, char *level_name)
{

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (server_socket < 0)
    {
        perror("socket failed !\n");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(struct sockaddr_in));

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(port);

    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
    {
        perror("Bind failed, incorrect port !\n");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0)
    {
        perror("listen failed\n");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    socklen_t addr_len = sizeof(myaddr);

    int client_fd;
    while (1)
    {

        printf("Server listening on port %d...\n", port);
        client_fd = accept(server_socket, (struct sockaddr *)&myaddr, &addr_len);
        if (client_fd < 0)
        {
            if (errno == EINTR)
                continue;
            perror("accept");
            continue;
        }

        printf("✅ New client connected!\n");

        Level *original_level = load_level(level_name);

        Level *current_level = malloc(sizeof(Level));

        copy_level(current_level, original_level);

        if (!current_level)
        {
            printf("Failed to load level!\n");
            exit(EXIT_FAILURE);
        }

        send_level(client_fd, current_level);

        while (1)
        {

            // Send game state to client

            // Send back updated state

            int move[2];
            ssize_t n = read_n_bytes(client_fd, move, sizeof(move));
            if (n == sizeof(move)) {
                int dx = move[0];
                int dy = move[1];
                try_move(current_level, dx, dy);
                send_level(client_fd, current_level);
            } else if (n == 0) {
                printf("Client disconnected\n");
            } else {
                perror("read failed");
            }
        }

        free_level(current_level);
        free_level(original_level);

        close(client_fd);
    }
}



int main(int argc, char *argv[]) {

    int port;
    char *level_name;

    if (argc < 3) {
        printf("Usage: %s <port> <level_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);

    level_name = argv[2];

    signal(SIGINT, handle_sigint);
    run_server(port, level_name);

    
    return 0;
}