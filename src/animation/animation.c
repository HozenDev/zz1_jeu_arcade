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
    int i;
    
    for (i = 0; i < s->n_state; ++i)
    {
        SDL_DestroyTexture(s->states[i]);
    }
}

void animation_render_sprite(SDL_Renderer * renderer,
                             struct sprite_s * s)
{
    sdl_render_image(renderer, s->states[(int) s->current_animation], s->r);
}

void animation_update_sprite(struct sprite_s * s, float speed)
{
    s->current_animation += speed;
    if ((int) s->current_animation >= s->n_state) s->current_animation = 0.0;
}

struct sprite_s * animation_sprite_from_file(SDL_Renderer * renderer, char * rname)
{
    struct sprite_s * s = NULL;
    
    struct dirent *pDirent = NULL;
    DIR *pDir = NULL;

    char buf[MAX_PATH]; /* realpath of files */
    int i = 0;
    
    pDir = opendir (rname);
    if (pDir == NULL) {
        zlog(stderr, ERROR, "Cannot open directory '%s'\n", rname);
        exit(-1);
    }

    s = (struct sprite_s *) malloc(sizeof(struct sprite_s));
    s->r.x = 0;
    s->r.y = 0;
    s->current_animation = 0.0;
    
    while ((pDirent = readdir(pDir)) != NULL) {
        if (strcmp(pDirent->d_name, ".") != 0 && strcmp(pDirent->d_name, "..") != 0)
        {
            strncpy(buf, rname, MAX_PATH);
            strncat(buf, pDirent->d_name, MAX_PATH-1);
            zlog(stdout, DEBUG, "'%s'", buf);

            s->states[i] = sdl_load_image(renderer, buf);
            i++;
        }
    }

    s->n_state = i;
    if (i > 0)
    {
        SDL_QueryTexture(s->states[0], NULL, NULL, &s->r.w, &s->r.h);
    }

    closedir (pDir);
    return s;
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
