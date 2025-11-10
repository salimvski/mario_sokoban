#include "render.h"
#include <stdio.h>

void render_map(Level *lvl) {

    int player_pos_x = lvl->player.x;
    int player_pos_y = lvl->player.y;

    printf("x: %d and y: %d\n", player_pos_x, player_pos_y);

    for(int i = 0; i < lvl->height; i++) {
        for(int j = 0; j < lvl->width; j++) {
            if(i == player_pos_x && j == player_pos_y) {
                printf("%c ", '@');
            } else {
                printf("%c ", lvl->tiles[i][j]);
            }
        }
        printf("\n");
    }
}

void render_status(Level *lvl) {
    printf("Player position is x: %d and y: %d\n", lvl->player.x, lvl->player.y);
}
