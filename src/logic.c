#include "logic.h"

// TODO: Move player in memory and handle pushing boxes
int try_move(Level *lvl, int dx, int dy) {
    // TODO
    return 0;
}

int check_win(Level *lvl) {
    for(int i = 0; i < lvl->height; i++) {
        for(int j = 0; j < lvl->width; j++) {
            if(lvl->tiles[i][j] == '*') {
                return false;
            }
        }
    }
    return true;
}
