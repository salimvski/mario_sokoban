#include "render.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

static SDL_Texture *wall_tex = NULL;
static SDL_Texture *floor_tex = NULL;
static SDL_Texture *box_tex = NULL;
static SDL_Texture *goal_tex = NULL;
static SDL_Texture *player_tex[4];
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
    player_tex[0] = load_texture(renderer, "assets/player/player_front.png");
    player_tex[1] = load_texture(renderer, "assets/player/player_up.png");
    player_tex[2] = load_texture(renderer, "assets/player/player_left.png");
    player_tex[3] = load_texture(renderer, "assets/player/player_right.png");
}

void destroy_textures() {
    if (menu_background) SDL_DestroyTexture(menu_background);
    menu_background = NULL;

    SDL_DestroyTexture(wall_tex);   wall_tex = NULL;
    SDL_DestroyTexture(floor_tex);  floor_tex = NULL;
    SDL_DestroyTexture(box_tex);    box_tex = NULL;
    SDL_DestroyTexture(goal_tex);   goal_tex = NULL;

    for (int i = 0; i < 4; i++) {
        if (player_tex[i]) {
            SDL_DestroyTexture(player_tex[i]);
            player_tex[i] = NULL;
        }
    }
}

void render_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (menu_background != NULL) {
        SDL_Rect bgRect = {0, 0, window_w, window_h};
        SDL_RenderCopy(renderer, menu_background, NULL, &bgRect);
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char *options[] = {"Jouer", "Quitter"};
    int spacing = 60;

    int title_x = window_w * 0.66;
    int title_y = window_h * 0.25;

    for (int i = 0; i < 2; i++) {
        SDL_Color color = (i == selected_option) ? yellow : white;

        SDL_Surface *text_surf = TTF_RenderUTF8_Blended(font, options[i], color);
        SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);

        int tw, th;
        SDL_QueryTexture(text_tex, NULL, NULL, &tw, &th);

        SDL_Rect dst = {
            title_x,
            title_y + 100 + (i * spacing),
            tw,
            th
        };

        SDL_RenderCopy(renderer, text_tex, NULL, &dst);

        SDL_FreeSurface(text_surf);
        SDL_DestroyTexture(text_tex);
    }

    SDL_RenderPresent(renderer);
}

void render_level_menu(SDL_Renderer *renderer, TTF_Font *font, int selected_option) {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (menu_background != NULL) {
        SDL_Rect bgRect = {0, 0, window_w, window_h};
        SDL_RenderCopy(renderer, menu_background, NULL, &bgRect);
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    int title_x = window_w * 0.60;
    int title_y = window_h * 0.22;

    const char *options[] = {"Niveau 1", "Niveau 2", "Retour"};
    int spacing = 60;

    for (int i = 0; i < 3; i++) {
        SDL_Color color = (i == selected_option) ? yellow : white;

        SDL_Surface *text_surf = TTF_RenderUTF8_Blended(font, options[i], color);
        SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);

        int tw, th;
        SDL_QueryTexture(text_tex, NULL, NULL, &tw, &th);

        SDL_Rect dst = {
            title_x,
            title_y + 110 + (i * spacing),
            tw,
            th
        };

        SDL_RenderCopy(renderer, text_tex, NULL, &dst);

        SDL_FreeSurface(text_surf);
        SDL_DestroyTexture(text_tex);
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
            SDL_Texture *tex = floor_tex;

            switch (t) {
                case '#': tex = wall_tex; break;
                case '$': tex = box_tex; break;
                case '.': tex = goal_tex; break;
            }

            SDL_RenderCopy(renderer, tex, NULL, &dst);
        }
    }

    dst.x = lvl->player.y * tile_size;
    dst.y = lvl->player.x * tile_size;
    dst.w = dst.h = tile_size;

    SDL_Texture *tex;
    switch (lvl->player.dir) {
        case 'F': tex = player_tex[0]; break;
        case 'U': tex = player_tex[1]; break;
        case 'L': tex = player_tex[2]; break;
        case 'R': tex = player_tex[3]; break;
        default:  tex = player_tex[0]; break;
    }

    SDL_RenderCopy(renderer, tex, NULL, &dst);
}
