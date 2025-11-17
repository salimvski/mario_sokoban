#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "level.h"
#include "logic.h"
#include "protocol.h"
#include "render.h"
#include "client.h"

int client_socket = -1;
static pid_t server_pid = -1;

void handle_sigint(int sig) {
    printf("\nCaught signal %d (%s), shutting down client...\n",
           sig, strsignal(sig));
    if (client_socket >= 0) {
        close(client_socket);
    }
    if (server_pid > 0) {
        kill(server_pid, SIGTERM);
    }
    exit(EXIT_SUCCESS);
}

int show_main_menu(SDL_Renderer *renderer, TTF_Font *font) {
    int selected = 0;
    SDL_Event e;
    const char *options[] = {"Jouer", "Quitter"};
    int running = 1;

    (void)options;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return -1;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        selected = (selected - 1 + 2) % 2;
                        break;
                    case SDLK_DOWN:
                        selected = (selected + 1) % 2;
                        break;
                    case SDLK_RETURN:
                        return selected;
                    case SDLK_ESCAPE:
                        return 1;
                }
            }
        }

        render_menu(renderer, font, selected);
        SDL_Delay(16);
    }
    return -1;
}

int show_level_menu(SDL_Renderer *renderer, TTF_Font *font) {
    int selected = 0;
    SDL_Event e;
    const char *options[] = {"Level 1", "Level 2", "Retour"};

    (void)options;

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return -1;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        selected = (selected - 1 + 3) % 3;
                        break;
                    case SDLK_DOWN:
                        selected = (selected + 1) % 3;
                        break;
                    case SDLK_RETURN:
                        return selected;
                    case SDLK_ESCAPE:
                        return 2;
                }
            }
        }

        render_level_menu(renderer, font, selected);
        SDL_Delay(16);
    }
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_sigint);

    int use_external_server = 0;
    const char *server_ip = "127.0.0.1";
    int port = 5666;

    if (argc >= 3) {
        server_ip = argv[1];
        port = atoi(argv[2]);
        use_external_server = 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image init error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf init Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize: %s\n", Mix_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    Mix_Music* background_music =
        Mix_LoadMUS("assets/audio/music/periwinkle.mp3");
    if (!background_music) {
        fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());
    } else {
        Mix_VolumeMusic(MIX_MAX_VOLUME * 0.3);
        Mix_PlayMusic(background_music, -1);
    }

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int screen_w = DM.w;
    int screen_h = DM.h;

    SDL_Window* window = SDL_CreateWindow(
        "Mario Sokoban",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        screen_w / 2, screen_h / 2,
        SDL_WINDOW_RESIZABLE);

    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        Mix_FreeMusic(background_music);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        Mix_FreeMusic(background_music);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    init_textures(renderer);

    TTF_Font *font = TTF_OpenFont("assets/fonts/MarioFont.ttf", 30);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        destroy_textures();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_FreeMusic(background_music);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    int chosen_level = 0;

    while (chosen_level == 0) {
        int main_choice = show_main_menu(renderer, font);
        if (main_choice != 0) {
            TTF_CloseFont(font);
            destroy_textures();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            if (background_music) Mix_FreeMusic(background_music);
            Mix_CloseAudio();
            TTF_Quit();
            IMG_Quit();
            SDL_Quit();
            return 0;
        }

        if (use_external_server) {
            chosen_level = -1;
        } else {
            int level_choice = show_level_menu(renderer, font);
            if (level_choice == 0) {
                chosen_level = 1;
            } else if (level_choice == 1) {
                chosen_level = 2;
            } else {
                continue;
            }
        }
    }

    if (!use_external_server && chosen_level > 0) {
        char level_path[64];
        snprintf(level_path, sizeof(level_path),
                 "levels/level%d.txt", chosen_level);

        char port_str[16];
        snprintf(port_str, sizeof(port_str), "%d", port);

        server_pid = fork();
        if (server_pid == 0) {
            execl("./server", "./server", port_str, level_path, (char*)NULL);
            perror("execl server failed");
            exit(1);
        }

        SDL_Delay(300);
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket");
        TTF_CloseFont(font);
        destroy_textures();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        if (background_music) Mix_FreeMusic(background_music);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(client_socket);
        TTF_CloseFont(font);
        destroy_textures();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        if (background_music) Mix_FreeMusic(background_music);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    if (connect(client_socket,
                (struct sockaddr*)&serv_addr,
                sizeof(serv_addr)) < 0) {
        perror("connect failed");
        close(client_socket);
        TTF_CloseFont(font);
        destroy_textures();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        if (background_music) Mix_FreeMusic(background_music);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    printf("✅ Connected to server %s:%d\n", server_ip, port);

    int running = 1;
    SDL_Event e;

    Level* level = receive_level_from_socket(client_socket);
    if (!level) {
        printf("Failed to receive level from server.\n");
        close(client_socket);
        TTF_CloseFont(font);
        destroy_textures();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        if (background_music) Mix_FreeMusic(background_music);
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_RenderSetLogicalSize(renderer, level->width * 32, level->height * 32);

    SDL_RenderClear(renderer);
    render_map_sdl(renderer, level);
    SDL_RenderPresent(renderer);

    char player_direction = 'F'; 

    while (running) {
        if (!level) {
            printf("Connection closed or level receive failed.\n");
            break;
        }

        Command cmd = {0};

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = 0;
                    printf("Quit event detected!\n");
                    break;

                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_q:
                        case SDLK_ESCAPE:
                            running = 0;
                            printf("Quit key pressed!\n");
                            break;
                        case SDLK_r:
                            cmd.type = 'R';
                            break;
                        case SDLK_w:
                            cmd.type = 'W';
                            cmd.dx = -1;
                            player_direction = 'U';
                            break;
                        case SDLK_s:
                            cmd.type = 'S';
                            cmd.dx = 1;
                            player_direction = 'S';
                            break;
                        case SDLK_a:
                            cmd.type = 'A';
                            cmd.dy = -1;
                            player_direction = 'L';
                            break;
                        case SDLK_d:
                            cmd.type = 'D';
                            cmd.dy = 1;
                            player_direction = 'R';
                            break;
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        send(client_socket, &cmd, sizeof(cmd), 0);

        if (update_level_from_socket(client_socket, level) < 0) {
            printf("Failed to update level\n");
            break;
        }

        level->player.dir = player_direction;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_map_sdl(renderer, level);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    destroy_textures();
    free_level(level);
    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    if (background_music) Mix_FreeMusic(background_music);
    Mix_CloseAudio();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    if (client_socket >= 0) {
        close(client_socket);
    }

    if (server_pid > 0 && !use_external_server) {
        kill(server_pid, SIGTERM);
    }

    return 0;
}
