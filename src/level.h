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

Level *load_level(const char *filename);
void free_level(Level *lvl);

#endif
