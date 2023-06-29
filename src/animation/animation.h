#ifndef _animation_h_
#define _animation_h_

#include "../sdl/sdl.h"

#define MAX_PATH 2048
#define MAX_TEXTURE 20

struct animation_s {
    float current_animation;
    int * array; /* array of indices of an animation */
    int n; /* size of the animation */
};

typedef struct animation_s animation_t;

struct sprite_s {
    SDL_Rect * r; /* rect from texture */
    SDL_Rect d; /* rect from screen */
    int n;
    char * name; 
    SDL_Texture * t; 
    struct animation_s * a;
};

typedef struct sprite_s sprite_t;

struct background_s {
    SDL_Texture * t;
    SDL_Rect r;
    char * name;
};

typedef struct background_s background_t;

void animation_change_animation(struct sprite_s * s, struct animation_s * a);

void animation_free_sprite(struct sprite_s * s);
void animation_free_background(struct background_s * b);
void animation_render_sprite(SDL_Renderer * renderer, struct sprite_s * s);
void animation_update_sprite(struct sprite_s ** s, float speed);
struct sprite_s * animation_spritesheet_from_file(SDL_Renderer * renderer, char * fname, int nb_frame);

animation_t * animation_create_animation_reverse(int n);
animation_t * animation_create_animation(int n);

struct background_s * animation_background_from_file(SDL_Renderer * renderer, char * fname);
void animation_render_background(SDL_Renderer * renderer, struct background_s * b,
                                 int screen_w, int screen_h);

#endif
