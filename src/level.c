#include "level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_tiles(char **tiles, int width, int height) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            printf("%c", tiles[i][j]);
        }
        printf("\n");
    }
}

Level *load_level(const char *filename) {
    
    FILE *fptr;

    fptr = fopen(filename, "r");

    if(fptr == NULL) {
        printf("Not able to open the file.\n");
        exit(EXIT_FAILURE);
    }

    char file_line[100];

    int max_line_len = 0;
    int rows = 0;

    while(fgets(file_line, 100, fptr)) {
        if(strlen(file_line) - 1 > max_line_len) {
            max_line_len = strlen(file_line) - 1;
        }
        rows++;
    }

    rewind(fptr);

    Level *level = malloc(sizeof(Level));

    level->width = max_line_len;
    level->height = rows;
    level->tiles = malloc(rows * sizeof(char*));

    int line_ctr = 0;
    while(fgets(file_line, 100, fptr)) {
        int line_len = 0;
        level->tiles[line_ctr] = malloc((level->width) * sizeof(char));
        for (int i = 0; file_line[i] != '\0' && file_line[i] != '\n'; i++) {
            if(file_line[i] == '@'){
                level->player.x = line_ctr;
                level->player.y = line_len;
            }
            level->tiles[line_ctr][i] = file_line[i];
            line_len++;
        }
        for (int i = line_len; i < level->width; i++) {
            level->tiles[line_ctr][i] = ' ';
        }
        line_ctr++;
    }

    fclose(fptr);

    return level;
}

// TODO: Free all allocated memory for Level
void free_level(Level *lvl) {
    for(int i = 0; i < lvl->height; i++) {
        free(lvl->tiles[i]);
    }
    free(lvl->tiles);
    free(lvl);
}


int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Error, please provide a filename!\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];

    Level *loaded_level = load_level(filename);

    free_level(loaded_level);

    return 0;
}
