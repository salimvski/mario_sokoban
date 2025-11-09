#ifndef LOGIC_H
#define LOGIC_H

#include "level.h"

// Attempt to move player by dx/dy, handle box pushing
int try_move(Level *lvl, int dx, int dy);

// Check if all boxes are on goal tiles
int check_win(Level *lvl);

#endif // LOGIC_H
