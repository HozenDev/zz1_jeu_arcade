#include "animation.h"
#include "../log/log.h"

#include <dirent.h>
#include <stdio.h>

void animation_free_background(struct background_s * b)
{
    SDL_DestroyTexture(b->t);
}

void animation_free_sprite(struct sprite_s * s)
{
    free(s->r);
    SDL_DestroyTexture(s->t);
}

void animation_render_sprite(SDL_Renderer * renderer,
                             struct sprite_s * s)
{
    sdl_render_image(renderer, s->t, s->r[(int) s->current_animation]);
}

void animation_update_sprite(struct sprite_s * s, float speed)
{
    s->current_animation += speed;
    if ((int) s->current_animation >= s->n_state) s->current_animation = s->n_state-1;
}

struct sprite_s animation_spritesheet_from_file(SDL_Renderer * renderer, char * fname, int nb_frame)
{
    int i;
    int height, width;

    struct sprite_s * s;
    
    s = (struct sprite_s *) malloc(sizeof(struct sprite_s));
    s->current_animation = 0.0;
    s->n_state = nb_frame;
    
    s->t = sdl_load_image(renderer, fname);

    SDL_QueryTexture(s->t, NULL, NULL, &width, &height);

    zlog(stdout, DEBUG, "%d, %d", width, height);
    
    int offset_x = width / nb_frame,                // La largeur d'une vignette de l'image
    offset_y = height / 1 ;                // La hauteur d'une vignette de l'image

    s->r = (SDL_Rect *) malloc(sizeof(SDL_Rect)*nb_frame);
    
    /* construction des différents rectangles autour de chacune des vignettes de la planche */
    i=0;
    for (int y = 0; y < height; y += offset_y)
    {
      for (int x = 0; x < width; x += offset_x)
      {
          s->r[i].x = x;
          s->r[i].y = y;
          s->r[i].w = offset_x;
          s->r[i].h = offset_y;
          i++;
      }
    }

    return *s;
}

struct background_s * animation_background_from_file(SDL_Renderer * renderer,
                                                     char * fname)
{
    struct background_s * b = NULL;

    b = (struct background_s *) malloc(sizeof(struct background_s));
    b->name = (char *) malloc(sizeof(char)*strlen(fname));
    strcpy(b->name, fname);

    b->t = sdl_load_image(renderer, fname);

    b->r.x = 0;
    b->r.y = 0;

    SDL_QueryTexture(b->t, NULL, NULL, &b->r.w, &b->r.h);
    
    return b;
}

void animation_render_background(SDL_Renderer * renderer,
                                 struct background_s * b,
                                 int screen_w, int screen_h)
{
    SDL_Rect r_screen;
    SDL_Rect destRect;
    int n = ((int) screen_w/b->r.w) + 2;

    (void) screen_h;
    zlog(stdout, DEBUG, "number of print bakcground '%s': %d", b->name, n);

    r_screen.x = b->r.x;
    r_screen.y = b->r.y;
    r_screen.w = b->r.w;
    r_screen.h = b->r.h;

    while (r_screen.x < screen_w)
    {
        destRect.x = r_screen.x;
        destRect.y = 0;
        destRect.w = b->r.w;
        destRect.h = b->r.h;
        SDL_RenderCopy(renderer, b->t, NULL, &destRect);
        r_screen.x += b->r.w;
    }
}
