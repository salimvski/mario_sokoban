#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
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

    Level *original_level = load_level(argv[1]);
    Level *current_level = malloc(sizeof(Level));
    copy_level(current_level, original_level);

    if (!current_level) {
        printf("Failed to load level!\n");
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }   

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image init error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    Mix_Music *bgm = Mix_LoadMUS("assets/mario_theme.mp3");
    if (bgm) Mix_PlayMusic(bgm, -1);

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
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_RenderSetLogicalSize(renderer, current_level->width * 32, current_level->height * 32);

    init_textures(renderer);

    int running = 1;
    SDL_Event e;

    TTF_Font *font = TTF_OpenFont("assets/fonts/MarioFont.ttf", 30);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        running = 0;
    }

    int in_menu = 1;
    int selected_option = 0;

    SDL_RenderSetLogicalSize(renderer, 0, 0);

    while (in_menu) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
                in_menu = 0;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        selected_option = (selected_option - 1 + 2) % 2;
                        break;
                    case SDLK_DOWN:
                        selected_option = (selected_option + 1) % 2;
                        break;
                    case SDLK_RETURN:
                        if (selected_option == 0) {
                            in_menu = 0;
                        } else if (selected_option == 1) {
                            running = 0;
                            in_menu = 0;
                        }
                        break;
                    case SDLK_ESCAPE:
                        running = 0;
                        in_menu = 0;
                        break;
                }
            }
        }

        render_menu(renderer, font, selected_option);
        SDL_Delay(16);
    }

    SDL_RenderSetLogicalSize(renderer, current_level->width * 32, current_level->height * 32);

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
            else if (e.type == SDL_KEYDOWN) {
                int dx = 0, dy = 0;
                switch (e.key.keysym.sym) {
                    case SDLK_q: 
                        running = 0;
                        break;
                    case SDLK_r: 
                        reset_level(current_level, original_level); 
                        break;
                    case SDLK_UP:    dx = -1; break;
                    case SDLK_DOWN:  dx = 1; break;
                    case SDLK_LEFT:  dy = -1; break;
                    case SDLK_RIGHT: dy = 1; break;
                    case SDLK_ESCAPE: running = 0; break;
                }
                try_move(current_level, dx, dy);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_map_sdl(renderer, current_level);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        if (check_win(current_level)) {
            printf("🎉 You win!\n");
            fflush(stdout);
            SDL_Delay(1500);
            running = 0;
        }
    }

    destroy_textures();
    free_level(current_level);
    free_level(original_level);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();

    return 0;
}
