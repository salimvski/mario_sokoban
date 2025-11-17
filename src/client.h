#ifndef CLIENT_H
#define CLIENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void handle_sigint(int sig);
int show_main_menu(SDL_Renderer *renderer, TTF_Font *font);
int show_level_menu(SDL_Renderer *renderer, TTF_Font *font);

#endif
