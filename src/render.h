#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "level.h"
#include <SDL2/SDL_ttf.h>

void init_textures(SDL_Renderer *renderer);

void destroy_textures();

void render_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option);

void render_map_sdl(SDL_Renderer *renderer, Level *lvl);



#endif
