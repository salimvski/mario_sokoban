#ifndef RENDER_H
#define RENDER_H

#include "level.h"

// Print the map to the console
void render_map(Level *lvl);

// Print additional info (moves count, goals remaining)
void render_status(Level *lvl);

#endif // RENDER_H
