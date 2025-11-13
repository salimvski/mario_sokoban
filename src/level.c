#include "level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


int update_level_from_socket(int sock, Level *lvl) {

    int width, height;
    if (recv(sock, &width, sizeof(int), MSG_WAITALL) <= 0) return -1;
    if (recv(sock, &height, sizeof(int), MSG_WAITALL) <= 0) return -1;

    if (width != lvl->width || height != lvl->height) return -1;

    if (recv(sock, &lvl->player, sizeof(Player), MSG_WAITALL) <= 0) return -1;

    for (int i = 0; i < lvl->height; i++) {
        if (recv(sock, lvl->tiles[i], lvl->width, MSG_WAITALL) <= 0) return -1;
    }

    return 0;
}


Level *receive_level_from_socket(int sock) {
    Level *lvl = malloc(sizeof(Level));
    if (!lvl) return NULL;

    recv(sock, &lvl->width, sizeof(int), MSG_WAITALL);
    recv(sock, &lvl->height, sizeof(int), MSG_WAITALL);
    recv(sock, &lvl->player, sizeof(Player), MSG_WAITALL);

    lvl->tiles = malloc(lvl->height * sizeof(char *));
    for (int i = 0; i < lvl->height; i++) {
        lvl->tiles[i] = malloc(lvl->width + 1);
        recv(sock, lvl->tiles[i], lvl->width, MSG_WAITALL);
        lvl->tiles[i][lvl->width] = '\0';
    }

    return lvl;
}



ssize_t send_all(int sock, void *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t n = send(sock, (char*)buf + total, len - total, 0);
        if (n <= 0) return n; // error or closed
        total += n;
    }
    return total;
}

void send_level(int client_fd, Level *level) {

    if (send_all(client_fd, &level->width, sizeof(int)) <= 0) {
        perror("send width failed");
        return;
    }
    if (send_all(client_fd, &level->height, sizeof(int)) <= 0) {
        perror("send height failed");
        return;
    }

    if (send_all(client_fd, &level->player, sizeof(Player)) <= 0) {
        perror("send player failed");
        return;
    }

    for (int i = 0; i < level->height; i++) {
        if (send_all(client_fd, level->tiles[i], level->width * sizeof(char)) <= 0) {
            perror("send tiles failed");
            return;
        }
    }
}


void reset_level(Level *current_level, Level *original_level) {
    
    current_level->player = original_level->player;

    for (int i = 0; i < original_level->height; i++) {
        memcpy(current_level->tiles[i], original_level->tiles[i], original_level->width);
    }
}


void copy_level(Level *current_level, Level *original_level) {

    current_level->tiles = malloc(sizeof(char*) * original_level->height);
    for (int i = 0; i < original_level->height; i++) {
        current_level->tiles[i] = malloc(sizeof(char) * original_level->width);
    }


    current_level->width = original_level->width;
    current_level->height = original_level->height;
    current_level->player = original_level->player;

    for (int i = 0; i < original_level->height; i++) {
        memcpy(current_level->tiles[i], original_level->tiles[i], original_level->width);
    }

}

Level *load_level(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    Level *lvl = malloc(sizeof(Level));
    lvl->width = 0;
    lvl->height = 0;
    lvl->tiles = NULL;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        lvl->height++;
        int len = strlen(line);
        if (line[len - 1] == '\n') line[len - 1] = '\0';
        if (len > lvl->width) lvl->width = len - 1;
    }

    fseek(f, 0, SEEK_SET);

    lvl->tiles = malloc(sizeof(char *) * lvl->height);
    for (int i = 0; i < lvl->height; i++) {
        lvl->tiles[i] = malloc(lvl->width + 1);
        fgets(line, sizeof(line), f);
        for (int j = 0; j < lvl->width; j++) {
            char c = line[j];
            if (c == '@') {
                lvl->player.x = i;
                lvl->player.y = j;
                lvl->tiles[i][j] = ' ';
            } else {
                lvl->tiles[i][j] = c;
            }
        }
    }

    fclose(f);
    return lvl;
}

void free_level(Level *lvl) {
    for (int i = 0; i < lvl->height; i++)
        free(lvl->tiles[i]);
    free(lvl->tiles);
    free(lvl);
}
