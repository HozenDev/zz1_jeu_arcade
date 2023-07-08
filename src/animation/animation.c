#include "animation.h"
#include "../log/log.h"

#include <dirent.h>
#include <stdio.h>

void animation_change_animation(struct sprite_s * s, struct animation_s * a)
{
    int i;

    if (s->a->array) free(s->a->array);
    
    s->a->n = a->n;
    s->a->current_animation = a->current_animation;
    s->a->array = (int *) malloc(sizeof(int)*a->n);

    for (i = 0; i < a->n; ++i)
    {
        s->a->array[i] = a->array[i];
    }
}

void animation_free_background(struct background_s * b)
{
    if (b->name) free(b->name);
    if (b->t) SDL_DestroyTexture(b->t);
}

void animation_free_sprite(struct sprite_s * s)
{
    if (s)
    {
        if (s->a)
        {
            if (s->a->array) free(s->a->array);
            free(s->a);
        }
        if (s->r) free(s->r);
        if (s->name) free(s->name);
        if (s->t) SDL_DestroyTexture(s->t);
        free(s);
    }
}

void animation_render_sprite(SDL_Renderer * renderer,
                             struct sprite_s * s)
{
    /* sdl_render_image(renderer, s->t, s->r[s->a->array[(int) s->a->current_animation]]); */
    SDL_RenderCopy(renderer,
                   s->t,
                   &s->r[s->a->array[(int) s->a->current_animation]],
                   &s->d);
}

void animation_update_sprite(struct sprite_s ** s, float speed)
{
    (*s)->a->current_animation += speed;
    if ((int) (*s)->a->current_animation >= (*s)->a->n) (*s)->a->current_animation = (*s)->a->n - 1;
}

animation_t * animation_create_animation_reverse(int n)
{
    int i;
    animation_t * a = (animation_t *) malloc(sizeof(animation_t));

    a->current_animation = 0.0;
    a->n = n;
    a->array = (int *) malloc(sizeof(int)*n);

    for (i = 0; i < n; ++i) a->array[i] = n-i-1;

    return a;
}

animation_t * animation_create_animation(int n)
{
    int i;
    animation_t * a = (animation_t *) malloc(sizeof(animation_t));

    a->current_animation = 0.0;
    a->n = n;
    a->array = (int *) malloc(sizeof(int)*n);

    for (i = 0; i < n; ++i) a->array[i] = i;

    return a;
}

struct sprite_s * animation_spritesheet_from_file(SDL_Renderer * renderer, char * fname, int nb_frame)
{
    int i = 0;
    int x, y;
    int height, width;
    int offset_x;
    int offset_y;
    int n_fname = strlen(fname);

    struct sprite_s * s = NULL;
    
    s = (struct sprite_s *) malloc(sizeof(struct sprite_s));

    s->name = (char *) malloc(sizeof(char)*n_fname);
    strncpy(s->name, fname, n_fname-1);
        
    /* chargement de la texture */
    
    s->t = sdl_load_image(renderer, fname);

    /* initialisation des rectangles du spritesheet */
    
    SDL_QueryTexture(s->t, NULL, NULL, &width, &height);

    offset_x = width / nb_frame;
    offset_y = height / 1;
    
    s->r = (SDL_Rect *) malloc(sizeof(SDL_Rect)*nb_frame);
    
    for (y = 0; y < height; y += offset_y)
    {
      for (x = 0; x < width; x += offset_x)
      {
          s->r[i].x = x;
          s->r[i].y = y;
          s->r[i].w = offset_x;
          s->r[i].h = offset_y;
          i++;
      }
    }

    s->d = (SDL_Rect) {.x = rand()%(width-s->r[0].w), .y = rand()%(width-s->r[0].h), .w = offset_x, .h = offset_y};

    /* initialisation de l'animation */
    s->a = animation_create_animation(nb_frame);

    return s;
}

struct background_s * animation_background_from_file(SDL_Renderer * renderer,
                                                     char * fname)
{
    int fname_n = strlen(fname);
    struct background_s * b = NULL;

    b = (struct background_s *) malloc(sizeof(struct background_s));
    b->name = (char *) malloc(sizeof(char)*strlen(fname));
    strncpy(b->name, fname, fname_n-1);

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
