#ifndef _sdl_h_ind_
#define _sdl_h_ind_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Window * sdl_create_window(char *, int, int);

SDL_Renderer * sdl_create_renderer(SDL_Window *);
void sdl_set_renderer_color(SDL_Renderer *, SDL_Color);

void sdl_draw_diamond(SDL_Renderer * renderer, int center_x, int center_y, int radius, SDL_Color c);
void sdl_draw_circle_filled(SDL_Renderer *renderer, int x, int y, int radius);
void sdl_draw_circle(SDL_Renderer *, int center_x, int center_y, int radius);
void sdl_draw_rect(SDL_Renderer *, SDL_Rect);
void sdl_draw_segment(SDL_Renderer * renderer, int x1, int y1, int x2, int y2);
void sdl_draw_rect_coords(SDL_Renderer *, int x, int y, int w, int h);

void sdl_init_image(void);
void sdl_render_image(SDL_Renderer *, SDL_Texture *, SDL_Rect);
SDL_Texture * sdl_load_image(SDL_Renderer *, char * name);
void sdl_scale_rect_image(SDL_Rect * rect, SDL_Texture * img, float wh, float ww, int horizontal);

void sdl_print_text(SDL_Window * window, SDL_Renderer * renderer,
                    TTF_Font * font, char * text, SDL_Point p, SDL_Color color);
void sdl_init_text(void);
void sdl_quit_text(void);
void sdl_quit(void);

#endif
