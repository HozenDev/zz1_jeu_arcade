#ifndef _animation_h_
#define _animation_h_

#include "../sdl/sdl.h"

#define MAX_PATH 2048
#define MAX_TEXTURE 20

struct sprite_s {
    SDL_Rect r;
    char * name;
    int n_state;
    float current_animation;
    SDL_Texture * states[MAX_TEXTURE];
};

typedef struct sprite_s sprite_t;

struct background_s {
    SDL_Texture * t;
    SDL_Rect r;
    char * name;
};

typedef struct background_s background_t;

void animation_render_sprite(SDL_Renderer * renderer, struct sprite_s * s);
void animation_update_sprite(struct sprite_s * s, float speed);
struct sprite_s * animation_sprite_from_file(SDL_Renderer * renderer, char * rname);

struct background_s * animation_background_from_file(SDL_Renderer * renderer, char * fname);
void animation_render_background(SDL_Renderer * renderer, struct background_s * b,
                                 int screen_w, int screen_h);

#endif
