#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "level.h"
#include "logic.h"
#include "render.h"

int client_socket = -1;

void handle_sigint(int sig) {
    printf("\nCaught SIGINT, shutting down client...\n");
    if (client_socket >= 0) {
        close(client_socket);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_sigint);

    if (argc < 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        return 1;
    }

    const char* server_ip = argv[1];
    int port = atoi(argv[2]);

    // === Connect to server ===
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        return 1;
    }
    printf("✅ Connected to server %s:%d\n", server_ip, port);

    // === SDL setup ===
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image init error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    Mix_Music* bgm = Mix_LoadMUS("assets/mario_theme.mp3");
    if (bgm)
        Mix_PlayMusic(bgm, -1);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int screen_w = DM.w;
    int screen_h = DM.h;
    SDL_Window* window = SDL_CreateWindow(
        "Mario Sokoban",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        screen_w / 2, screen_h / 2,
        SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    init_textures(renderer);

    int running = 1;
    int first = 0;
    SDL_Event e;
    Level* level = NULL;

    level = recv_level(client_socket);

    SDL_RenderSetLogicalSize(renderer, level->width * 32, level->height * 32);

    SDL_RenderClear(renderer);
    render_map_sdl(renderer, level);
    SDL_RenderPresent(renderer);

    while (running) {
        // === Receive latest level state ===
        // if (level) free_level(level);

        if (!level) {
            printf("Connection closed or level receive failed.\n");
            break;
        }

        int dx = 0, dy = 0;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
            else if (e.type == SDL_KEYDOWN) {
                printf("its a key down\n");
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        running = 0;
                        break;
                    case SDLK_r:
                        printf("its a reset !\n");
                        // reset_level(current_level, original_level);
                        break;
                    case SDLK_w:
                        dx = -1;
                        break;
                    case SDLK_s:
                        dx = 1;
                        break;
                    case SDLK_a:
                        dy = -1;
                        break;
                    case SDLK_d:
                        dy = 1;
                        break;
                    case SDLK_ESCAPE:
                        running = 0;
                        break;
                }
                // try_move(current_level, dx, dy);
            }
        }

        printf("dx: %d and dy: %d\n", dx, dy);
        // === Send move to server ===
        if (dx != 0 || dy != 0) {
            int move[2] = {dx, dy};
            printf("im gonna send move !\n");
            send(client_socket, move, sizeof(move), 0);
            level = recv_level(client_socket);
        }

        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_map_sdl(renderer, level);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    if (level)
        free_level(level);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    Mix_CloseAudio();
    SDL_Quit();
    close(client_socket);

    return 0;
}
