#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "level.h"

// Initialize textures once after renderer creation
void init_textures(SDL_Renderer *renderer);

// Free all textures when closing the game
void destroy_textures();

// Render the entire map + player
void render_map_sdl(SDL_Renderer *renderer, Level *lvl);

#endif
