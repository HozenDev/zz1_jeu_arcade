#ifndef _game_h_
#define _game_h_

#include "../sdl/sdl.h"
#include "../log/log.h"
#include "../utils/utils.h"
#include "../animation/animation.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define GAME_DELAY 30
#define ALL_TIME 20000

#define NB_GAME_MOLE 3

struct game_state_s {
    int mx;
    int my;

    int running;
    int end;

    SDL_Event event;

    struct sprite_s ** sprites;
    int nb_sprite;

    float score;
    float time;
    float delay;

    float time_end_delay;
    
    SDL_Rect game_rect;
};

typedef struct game_state_s game_state_t;  

struct game_s {
    int sw;
    int sh;

    SDL_Renderer * renderer;
    SDL_Window * window;
    TTF_Font * font;

    struct background_s * back;
    struct game_state_s state;
};

typedef struct game_s game_t;

SDL_bool game_kill_mole(int x,int y,SDL_Rect mole);
void game_graphic_update(game_t game);
void game_mouse_state_update(game_state_t * g_state);
int game_initialisation(game_t ** game);
int game_loop(void);

#endif
