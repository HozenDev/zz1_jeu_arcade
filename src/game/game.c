#include "game.h"
/**
 *@brief Test if a point is in a rect
 *
 *@param int x, x coordinate of the point 
 *@param int y,y coordinate of the point
 *@param rect,the rect
 */
SDL_bool kill_mole(int x,int y,SDL_Rect Taupe){

    SDL_bool kill=SDL_FALSE;
    if(x>Taupe.x && x<Taupe.x+Taupe.w && y>Taupe.y && y<Taupe.y+Taupe.h){
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

    /* animation_render_background(game.renderer, game.back, game.sw, game.sh); */
    sdl_scale_rect_image(&game.back->r, game.back->t, game.sh);
    sdl_render_image(game.renderer, game.back->t, game.back->r);
    
    sdl_print_text(game.window, game.renderer, game.font, "JEU DES TAUPES",
                   (SDL_Point) {.x = -1, .y = 50}, colors_available.BLACK);

    sprintf(buf, "Score: %.2f", game.state.score);

    sdl_print_text(game.window, game.renderer, game.font, buf,
                   (SDL_Point) {.x = -1, .y = game.sh-80}, colors_available.BLACK);

}

/**
 * @brief Update a game state
 *
 * @param g_state, the game's state to be updated
 */
void game_state_update(game_state_t * g_state)
{
    /*
      todo: interargir click souris:
      */
    if (g_state->event.button.button == SDL_BUTTON_LEFT) /* click souris gauche */
    {
        if(kill_mole(g_state->mx,g_state->my,g_state->sprites->r)){
                g_state->score+=1;
        }
    }
    else if (g_state->event.button.button == SDL_BUTTON_RIGHT) /* click souris droit */
    {
        /* todo: changement état après click souris droit */
    }
    /* maj score */
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

    /* init de ttf */
    sdl_init_text();
    (*game)->font = TTF_OpenFont("../data/aventi.ttf", 32);
    if (!(*game)->font) exit(-1);
    zlog(stdout, INFO, "OK '%s'", "game_loop: Font is initialized.");

    /* ------ génération objets du jeu --------- */

    (*game)->back = animation_background_from_file((*game)->renderer, "../data/background_grass.jpeg");

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
                game_state_update(&game->state);
        	break;
            case SDL_QUIT:
        	zlog(stdout, INFO, "event->type: SDL_QUIT", NULL);
        	game->state.running = 0;
                break;
            }
        }

        game_graphic_update(*game);

        SDL_RenderPresent(game->renderer);
        
        /* delai minimal = 1 */
        SDL_Delay(30);
    }

    game_free_game(game);
    
    return 0;
}
