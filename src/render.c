#include "render.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

static SDL_Texture *wall_tex = NULL;
static SDL_Texture *floor_tex = NULL;
static SDL_Texture *box_tex = NULL;
static SDL_Texture *goal_tex = NULL;
static SDL_Texture *player_tex[4]; // up, down, left, right
static SDL_Texture *menu_background = NULL;


static SDL_Texture* load_texture(SDL_Renderer *renderer, const char *path) {
    SDL_Surface *surf = IMG_Load(path);
    if (!surf) {
        printf("Failed to load %s: %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return tex;
}

void init_textures(SDL_Renderer *renderer) {
    menu_background = load_texture(renderer, "assets/menu/menu_background.png");
    wall_tex   = load_texture(renderer, "assets/tiles/wall.png");
    floor_tex  = load_texture(renderer, "assets/tiles/floor.png");
    box_tex    = load_texture(renderer, "assets/tiles/box.png");
    goal_tex   = load_texture(renderer, "assets/tiles/goal.png");
    player_tex[0] = load_texture(renderer, "assets/player/mario_right.png");
    player_tex[1] = load_texture(renderer, "assets/player/mario_right.png");
    player_tex[2] = load_texture(renderer, "assets/player/mario_right.png");
    player_tex[3] = load_texture(renderer, "assets/player/mario_right.png");
}

void destroy_textures() {
    SDL_DestroyTexture(menu_background);
    SDL_DestroyTexture(wall_tex);
    SDL_DestroyTexture(floor_tex);
    SDL_DestroyTexture(box_tex);
    SDL_DestroyTexture(goal_tex);
    for (int i = 0; i < 4; i++)
        SDL_DestroyTexture(player_tex[i]);
}

void render_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (menu_background != NULL) {
        SDL_RenderCopy(renderer, menu_background, NULL, NULL);
    } else {
        printf("menu_background is NULL\n");
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char *options[] = {"Lancer le jeu", "Quitter"};
    int y_start = 375;
    int spacing = 60;

    for (int i = 0; i < 2; i++) {
        SDL_Color color = (i == selected_option) ? yellow : white;

        SDL_Surface *text_surface = TTF_RenderUTF8_Blended(font, options[i], color);
        if (!text_surface) {
            printf("TTF_RenderUTF8_Blended error: %s\n", TTF_GetError());
            continue;
        }

        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (!text_texture) {
            printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
            SDL_FreeSurface(text_surface);
            continue;
        }

        SDL_SetTextureBlendMode(text_texture, SDL_BLENDMODE_BLEND);

        int text_w, text_h;
        SDL_QueryTexture(text_texture, NULL, NULL, &text_w, &text_h);
        SDL_Rect dst = {475 - text_w / 2, y_start + i * spacing, text_w, text_h};

        SDL_RenderCopy(renderer, text_texture, NULL, &dst);

        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }

    SDL_RenderPresent(renderer);
}


void render_map_sdl(SDL_Renderer *renderer, Level *lvl) {
    int tile_size = 32;
    SDL_Rect dst;

    for (int i = 0; i < lvl->height; i++) {
        for (int j = 0; j < lvl->width; j++) {
            dst.x = j * tile_size;
            dst.y = i * tile_size;
            dst.w = dst.h = tile_size;

            char t = lvl->tiles[i][j];
            SDL_Texture *tex = floor_tex; // default

            switch (t) {
                case '#': tex = wall_tex; break;
                case '$': tex = box_tex; break;
                case '.': tex = goal_tex; break;
                default:  tex = floor_tex; break;
            }

            SDL_RenderCopy(renderer, tex, NULL, &dst);
        }
    }

    // Draw player (always down for now)
    dst.x = lvl->player.y * tile_size;
    dst.y = lvl->player.x * tile_size;
    dst.w = dst.h = tile_size;
    SDL_RenderCopy(renderer, player_tex[1], NULL, &dst);
}
