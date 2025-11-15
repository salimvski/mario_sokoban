#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "level.h"

void init_textures(SDL_Renderer *renderer);

void destroy_textures();

void render_map_sdl(SDL_Renderer *renderer, Level *lvl);

void render_map_sdl(SDL_Renderer *renderer, Level *lvl);

#endif
