#include "level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
