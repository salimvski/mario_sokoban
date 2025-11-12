#ifndef LEVEL_H
#define LEVEL_H

typedef struct {
    int x, y;
} Player;

typedef struct {
    int width, height;
    char **tiles;
    Player player;
} Level;

void send_level(int client_fd, Level *level);

void reset_level(Level *current_level, Level *original_level);

void copy_level(Level *current_level, Level *original_level);

Level *load_level(const char *filename);

void free_level(Level *lvl);

#endif
