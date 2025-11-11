#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include "level.h"
#include "logic.h"
#include "render.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: please provide a level file!\n");
        return 1;
    }

    Level *level = load_level(argv[1]);
    if (!level) {
        printf("Failed to load level!\n");
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image init error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    Mix_Music *bgm = Mix_LoadMUS("assets/mario_theme.mp3");
    if (bgm) Mix_PlayMusic(bgm, -1); // -1 = loop forever



    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int screen_w = DM.w;
    int screen_h = DM.h;

    SDL_Window *window = SDL_CreateWindow(
        "Mario Sokoban",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        screen_w / 2, screen_h / 2,
        SDL_WINDOW_RESIZABLE
    );
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetLogicalSize(renderer, level->width * 32, level->height * 32);

    init_textures(renderer);

    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
            else if (e.type == SDL_KEYDOWN) {
                int dx = 0, dy = 0;
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    dx = -1; break;
                    case SDLK_DOWN:  dx = 1; break;
                    case SDLK_LEFT:  dy = -1; break;
                    case SDLK_RIGHT: dy = 1; break;
                    case SDLK_ESCAPE: running = 0; break;
                }
                try_move(level, dx, dy);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_map_sdl(renderer, level);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        if (check_win(level)) {
            printf("🎉 You win!\n");
            SDL_Delay(1500);
            running = 0;
        }
    }

    destroy_textures();
    free_level(level);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();


    return 0;
}
