#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "level.h"

void init_textures(SDL_Renderer *renderer);

void destroy_textures();

void render_map_sdl(SDL_Renderer *renderer, Level *lvl);

void render_map_sdl(SDL_Renderer *renderer, Level *lvl);

void render_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option);

void render_level_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option);


#endif
