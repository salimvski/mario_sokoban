#include "render.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

static SDL_Texture *wall_tex = NULL;
static SDL_Texture *floor_tex = NULL;
static SDL_Texture *box_tex = NULL;
static SDL_Texture *goal_tex = NULL;
static SDL_Texture *player_tex[4]; // up, down, left, right


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
    SDL_DestroyTexture(wall_tex);   wall_tex = NULL;
    SDL_DestroyTexture(floor_tex);  floor_tex = NULL;
    SDL_DestroyTexture(box_tex);    box_tex = NULL;
    SDL_DestroyTexture(goal_tex);   goal_tex = NULL;
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(player_tex[i]);
        player_tex[i] = NULL;
    }
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
