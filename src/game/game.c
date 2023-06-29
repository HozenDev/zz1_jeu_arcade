#include "game.h"

void game_loop_state_update(game_state_t * g_state)
{
    int i;
    animation_t * new_a = NULL;

    if (!g_state->end) g_state->time += g_state->delay;
    for (i = 0; i < g_state->nb_sprite; ++i)
    {
        animation_update_sprite(&g_state->sprites[i], 0.2);
    }
    if (!g_state->end && g_state->time >= ALL_TIME) {
        g_state->end = 1;
        g_state->time = ALL_TIME;
        new_a = animation_create_animation_reverse(4);
        for (i = 0; i < g_state->nb_sprite; ++i) {
            animation_change_animation(g_state->sprites[i], new_a);
        }
        free(new_a);
    }
}

void game_change_sprites(game_t * game)
{
    (void) game;
    
    /* todo changer les sprites avec une probabilité */
}

/**
 *@brief Test if a point is in a rect
 *
 *@param int x, x coordinate of the point 
 *@param int y, y coordinate of the point
 *@param rect,the rect
 *
 *@return true if the taupe is killed
 */
SDL_bool game_kill_mole(int x,int y,SDL_Rect mole)
{
    SDL_bool kill=SDL_FALSE;
    if(x>mole.x && x<mole.x+mole.w && y>mole.y && y<mole.y+mole.h){
        kill=SDL_TRUE;
    } 
    return(kill);
}

/**
 * @brief Free a game
 *
 * @param game, the game struct to be cleared
 */
void game_free_game(game_t * game)
{
    int i;
    
    if (game)
    {
        /* free window and game renderer */
        if (game->window) {SDL_DestroyWindow(game->window); game->window = NULL;}
        if (game->renderer) {SDL_DestroyRenderer(game->renderer); game->renderer = NULL;}

        /* clear game attributs */
        game->sw = 0;
        game->sh = 0;

        /* clear state */
        game->state.mx = 0;
        game->state.my = 0;

        for (i = 0; i < game->state.nb_sprite; ++i)
        {
            animation_free_sprite(game->state.sprites[i]);
        }
        free(game->state.sprites);

        animation_free_background(game->back);

        free(game);
    }
}

/**
 * @brief Update game graphic
 * 
 * @param game_t to update graphic
 */
void game_graphic_update(game_t game)
{
    /* todo : update graphic */
    char buf[2048];
    int i;

    /* animation_render_background(game.renderer, game.back, game.sw, game.sh); */
    sdl_render_image(game.renderer, game.back->t, game.back->r);
    
    /* render all sprites */
    for (i = 0; i < game.state.nb_sprite; ++i)
    {
        animation_render_sprite(game.renderer, game.state.sprites[i]);
    }

    if (game.state.end) { /* écran de fin de jeu */
        sdl_set_renderer_color(game.renderer, (SDL_Color) {.r=0, .g=0, .b=0, .a=150});
        sdl_draw_rect_coords(game.renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        sdl_print_text(game.window, game.renderer, game.font, "Rejouer",
                       (SDL_Point) {.x = -1, .y = 200}, colors_available.WHITE);
        sdl_print_text(game.window, game.renderer, game.font, "Quitter",
                       (SDL_Point) {.x = -1, .y = 400}, colors_available.WHITE);
    }
    
    /* print score */
    sprintf(buf, "Score: %d", (int) game.state.score);
    sdl_print_text(game.window, game.renderer, game.font, buf,
                   (SDL_Point) {.x = 10, .y = 50}, colors_available.BLACK);

    /* print time */
    sprintf(buf, "Temps: %.1f/%d", game.state.time/1000, (int) ALL_TIME/1000);
    sdl_print_text(game.window, game.renderer, game.font, buf,
                   (SDL_Point) {.x = 10, .y = 10}, colors_available.BLACK);
}

void game_state_reset(game_state_t * g_state)
{
    int i;
    animation_t * new_a = NULL;
    
    g_state->running = 1;
    g_state->end = 0;

    g_state->score = 0.0;
    g_state->time = 0.0;
    g_state->delay = GAME_DELAY;

    new_a = animation_create_animation(4);

    for (i = 0; i < g_state->nb_sprite; ++i)
    {
        g_state->sprites[i]->a->current_animation = 0.0;
        g_state->sprites[i]->d.x
            = rand()%(g_state->game_rect.w-g_state->game_rect.x) + g_state->game_rect.x;
        g_state->sprites[i]->d.y
            = rand()%(g_state->game_rect.h-g_state->game_rect.y) + g_state->game_rect.y;
        animation_change_animation(g_state->sprites[i], new_a);
    }
    free(new_a);
}

/**
 * @brief Update a game state
 *
 * @param g_state, the game's state to be updated
 */
void game_mouse_state_update(game_state_t * g_state)
{
    int i;

    if (!g_state->end) {
        if (g_state->event.button.button == SDL_BUTTON_LEFT) /* click souris gauche */
        {
            for (i = 0; i < g_state->nb_sprite; ++i) { 
                if (game_kill_mole(g_state->mx, g_state->my, g_state->sprites[i]->d))
                {
                    g_state->score+=1;
                    g_state->sprites[i]->d.x
                        = rand()%(g_state->game_rect.w) + g_state->game_rect.x;
                    g_state->sprites[i]->d.y
                        = rand()%(g_state->game_rect.h) + g_state->game_rect.y;
                    g_state->sprites[i]->a->current_animation = 0.0;
                    break;
                }
            }
        }
        else if (g_state->event.button.button == SDL_BUTTON_RIGHT) /* click souris droit */
        {
            /* todo: changement état après click souris droit */
        }
    }
    else { /* gestion fin de jeu */
        if (g_state->my <= (int) SCREEN_HEIGHT/2)
            game_state_reset(g_state);
        else g_state->running = 0;
    }
}

/**
 * @brief initialise a game_t structure
 *
 * @param game, game_t to be initialized
 *
 * @return exit code, 0 success, 1 failure
 */
int game_initialisation(game_t ** game)
{
    (*game) = (game_t *) malloc(sizeof(game_t));
    
    (*game)->sw = SCREEN_WIDTH;
    (*game)->sh = SCREEN_HEIGHT;

    /* initialize (*game) state */
    (*game)->state.mx = 0;
    (*game)->state.my = 0;
    (*game)->state.running = 1;

    /* ------ initialisation SDL2 --------- */
    
    /* SDL initialisation */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        zlog(stderr, ERROR, "Erreur d'initialisation de la SDL : %s", SDL_GetError());
        exit(-1);
    }
    zlog(stdout, INFO, "OK '%s'", "SDL is initialized.");

    /* création de la fenetre principale */
    (*game)->window = sdl_create_window("JEU DU VOYAGEUR", (*game)->sw, (*game)->sh);
    if (!(*game)->window) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "game_loop: Window is initialized.");

    /* création du renderer */
    (*game)->renderer = sdl_create_renderer((*game)->window);
    if (!(*game)->renderer) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "game_loop: Renderer is initialized.");
    SDL_SetRenderDrawBlendMode((*game)->renderer, SDL_BLENDMODE_BLEND);

    /* init de ttf */
    sdl_init_text();
    (*game)->font = TTF_OpenFont("../data/Socake.ttf", 50);
    if (!(*game)->font) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "game_loop: Font is initialized.");

    /* ------ génération objets du jeu --------- */

    (*game)->state.game_rect.x = 20;
    (*game)->state.game_rect.y = 100;
    (*game)->state.game_rect.w = 635;
    (*game)->state.game_rect.h = 400;

    zlog(stdout, DEBUG, "plateau de jeu de taille: %d %d %d %d",
         (*game)->state.game_rect.x, (*game)->state.game_rect.y,
         (*game)->state.game_rect.w, (*game)->state.game_rect.h);

    (*game)->state.delay = GAME_DELAY;
    (*game)->state.time = 0;
    
    (*game)->back = animation_background_from_file((*game)->renderer, "../data/back_tree.jpg");
    sdl_scale_rect_image(&(*game)->back->r, (*game)->back->t,
                         (*game)->sh, (*game)->sw, 0);

    (*game)->state.nb_sprite = NB_GAME_MOLE;
    
    (*game)->state.sprites = (struct sprite_s **) malloc(sizeof(struct sprite_s *) * (*game)->state.nb_sprite);

    for (int i = 0; i < NB_GAME_MOLE; ++i)
    {
        (*game)->state.sprites[i] = animation_spritesheet_from_file((*game)->renderer, "../data/taupe_spritesheet.png", 4);
    }

    zlog(stdout, DEBUG, "%d %d %d %d", (*game)->state.sprites[0]->r[0].x,
         (*game)->state.sprites[0]->r[0].y,
         (*game)->state.sprites[0]->r[0].w,
         (*game)->state.sprites[0]->r[0].h);

    return EXIT_SUCCESS;
}

/**
 * @brief game loop, initialise, manage events, update graphics, free
 *
 * @return exit code: 0 success, != 0 failure
 */
int game_loop()
{
    game_t * game = NULL;
    
    SDL_Event * event;
    
    game_initialisation(&game);

    event = &(game->state.event);
    
    /* Boucle de jeu */
    while (game->state.running == 1)
    {
        /* Boucle d'évènements */
        while (SDL_PollEvent(event))
        {
            switch(event->type)
            {
            case SDL_WINDOWEVENT:
        	switch (event->window.event)
        	{
        	case SDL_WINDOWEVENT_CLOSE:
        	    zlog(stdout, INFO, "sdl event window close", NULL);
        	    break;
        	}
        	break;
            case SDL_KEYDOWN:
                
        	break;
            case SDL_MOUSEMOTION:
                /* update mouse position */
                SDL_GetMouseState(&game->state.mx, &game->state.my);
                break;
            case SDL_MOUSEBUTTONDOWN:
                game_mouse_state_update(&game->state);
        	break;
            case SDL_QUIT:
        	zlog(stdout, INFO, "event->type: SDL_QUIT", NULL);
        	game->state.running = 0;
                break;
            }
        }

        game_loop_state_update(&game->state);
        game_change_sprites(game);
        game_graphic_update(*game);

        SDL_RenderPresent(game->renderer);
        
        /* delai minimal = 1 */
        SDL_Delay(game->state.delay);
    }

    game_free_game(game);
    
    return 0;
}
