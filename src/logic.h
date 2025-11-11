#ifndef LOGIC_H
#define LOGIC_H
#include <stdbool.h>
#include "level.h"

int try_move(Level *lvl, int dx, int dy);
bool check_win(Level *lvl);

#endif
