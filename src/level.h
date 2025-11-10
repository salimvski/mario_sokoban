#ifndef LEVEL_H
#define LEVEL_H

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    char **tiles;      // 2D map
    int width;
    int height;
    Position player;
} Level;

Level *load_level(const char *filename);

void free_level(Level *lvl);

#endif // LEVEL_H
