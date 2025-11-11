#include "logic.h"
#include <stdio.h>

int try_move(Level *lvl, int dx, int dy) {

    int next_x = lvl->player.x + dx;
    int next_y = lvl->player.y + dy;

    // Against wall
    if (lvl->tiles[next_x][next_y] == '#') {
        return 0;
    }

    // Push box
    if(lvl->tiles[next_x][next_y] == '$') {
        if(lvl->tiles[next_x + dx][next_y + dy] != '#') {
            lvl->tiles[next_x + dx][next_y + dy] = '$';
            lvl->tiles[next_x][next_y] = ' ';
            lvl->player.x = next_x;
            lvl->player.y = next_y;
           return 1;
        }
        return 0;
    }

    lvl->player.x = next_x;
    lvl->player.y = next_y;

    
    return 0;
}

bool check_win(Level *lvl) {
    for(int i = 0; i < lvl->height; i++) {
        for(int j = 0; j < lvl->width; j++) {
            if(lvl->tiles[i][j] == '.') {
                return false;
            }
        }
    }
    return true;
}
