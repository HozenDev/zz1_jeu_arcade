#include "sdl.h"
#include "../log/log.h"

/**
 * @brief Create a SDL_Renderer associated with a window
 *
 * @param char * name, name of the window
 * @param int width, width of the window
 * @param int height, height of the window
 *
 * @return SDL_Window * window, the window created
 */
SDL_Renderer * sdl_create_renderer(SDL_Window * window)
{
    SDL_Renderer * renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
	zlog(stderr, ERROR, "Erreur d'initialisation de la SDL : %s", SDL_GetError());
    }
    return renderer;
}

/**
 * @brief Create SDL Window
 *
 * @param char * name, name of the window
 * @param int width, width of the window
 * @param int height, height of the window
 *
 * @return SDL_Window * window, the window created
 */
SDL_Window * sdl_create_window(char * name, int width, int height)
{
    SDL_Window * window;

    window = SDL_CreateWindow(name,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_SHOWN);
    if (!window)
    {
	zlog(stderr, ERROR, "Erreur d'initialisation de la SDL : %s", SDL_GetError());
    }
    return window;
}

/**
 * @brief Draw rectangle from with an SDL_Rect
 *
 * @param SDL_Renderer, renderer where the image will be print
 * @param SDL_Rect rect, texture associated with an image
 */
void sdl_draw_rect(SDL_Renderer * renderer, SDL_Rect rect)
{
    SDL_RenderFillRect(renderer, &rect);
}

/**
 * @brief Modify renderer color
 *
 * @param SDL_Renderer * renderer, renderer to be modified
 * @param SDL_Color renderer_color, new color of the renderer
 */
void sdl_set_renderer_color(SDL_Renderer * renderer, SDL_Color renderer_color)
{
    /* set renderer color */
    SDL_SetRenderDrawColor(renderer,
			   renderer_color.r,
			   renderer_color.g,
			   renderer_color.b,
			   renderer_color.a);
}

/**
 * @brief Initialisation of SDL Image
 */
void sdl_init_image(void)
{
    int flags=IMG_INIT_JPG|IMG_INIT_PNG;
    int initted= 0;

    initted = IMG_Init(flags);

    if((initted&flags) != flags)
    {
	zlog(stderr, ERROR, "IMG_Init: Impossible d'initialiser les formats JPG et PNG!", NULL);
	zlog(stderr, ERROR, "IMG_Init: %s", IMG_GetError());
    }
}

/**
 * @brief Render image
 *
 * @param SDL_Renderer, renderer where the image will be print
 * @param SDL_Texture * texture, texture associated with an image
 * @param SDL_Rect rect, shape of the image
 */
void sdl_render_image(SDL_Renderer * renderer, SDL_Texture * texture, SDL_Rect rect)
{
    /* copie de l'image dans le renderer */
    SDL_RenderCopy(renderer, texture, NULL, &rect); 
}

/**
 * @brief Load image by creating a texture associated with
 *
 * @param SDL_Renderer, renderer where the image will be print
 * @param char * name, image's name
 *
 * @return SDL_Texture * texture, texture associated with the image
 */
SDL_Texture * sdl_load_image(SDL_Renderer * renderer, char * name)
{
    SDL_Texture * texture = IMG_LoadTexture(renderer, name);
    if (!texture)
    {
	zlog(stderr, WARNING, "SDL_CreateTextureFromSurface: '%s'", "texture can't load");
    }
    else
    {
	zlog(stdout, INFO, "SDL_CreateTextureFromSurface: %s", "Texture has been loaded");
    }
    return texture;
}

/**
 * @brief Draw a rectangle from coordinates
 *
 * @param SDL_Renderer, renderer where the rectangle will be print
 * @param int x, coordinate top left x of the rectangle
 * @param int y, coordinate top left y of the rectangle
 * @param int w, width of the rectangle
 * @param int h, height of the rectangle
 */
void sdl_draw_rect_coords(SDL_Renderer * renderer, int x, int y, int w, int h)
{
    /* draw a rectangle with SDL */
    sdl_draw_rect(renderer, (SDL_Rect) {.x = x, .y = y, .w = w, .h = h});
}

/**
 * @brief Draw a filled circle
 *
 * @param renderer, renderer where the circle will be printed
 * @param x, x center of the circle
 * @param y, y center of the circle
 * @param radius, radius of the circle
 * @param color, color of the circle
 */
void sdl_draw_circle_filled(SDL_Renderer *renderer, int x, int y, int radius)
{
    int w, h, dx, dy;

    for (w = 0; w < radius * 2; w++)
    {
        for (h = 0; h < radius * 2; h++)
        {
            dx = radius - w; // horizontal offset
            dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void sdl_draw_diamond(SDL_Renderer * renderer, int center_x, int center_y, int radius, SDL_Color c)
{
    SDL_Vertex trisup[3] =
        {
            {
                {center_x-radius, center_y},
                {c.r, c.g, c.b, c.a},
                {0.f, 0.f}
            },
            {
                {center_x, center_y+radius},
                {c.r, c.g, c.b, c.a},
                {0.f, 0.f}
            },
            {
                {center_x+radius, center_y},
                {c.r, c.g, c.b, c.a},
                {0.f, 0.f}
            }
        };
    SDL_Vertex triinf[3] =
        {
            {
                {center_x-radius, center_y},
                {c.r, c.g, c.b, c.a},
                {0.f, 0.f}
            },
            {
                {center_x, center_y-radius},
                {c.r, c.g, c.b, c.a},
                {0.f, 0.f}
            },
            {
                {center_x+radius, center_y},
                {c.r, c.g, c.b, c.a},
                {0.f, 0.f}
            }
        };

    if( SDL_RenderGeometry(renderer, NULL, trisup, 3, NULL, 0) < 0 ) {SDL_Log("%s\n", SDL_GetError());}
    if( SDL_RenderGeometry(renderer, NULL, triinf, 3, NULL, 0) < 0 ) {SDL_Log("%s\n", SDL_GetError());}
}


/**
 * @brief Draw a circle based on the middle point circle algorithm
 *
 * @param SDL_Renderer, renderer where the circle will be print
 * @param int center_x, coordonate x of center of the circle to be print
 * @param int center_y, coordonate y of center of the circle to be print
 * @param int radius, radius of the circle
 */
void sdl_draw_circle(SDL_Renderer * renderer, int center_x, int center_y, int radius)
{
    /* draw a circle with SDL */

    /* Middle point circle algorithm */
    /* https://en.wikipedia.org/w/index.php?title=Midpoint_circle_algorithm&oldid=889172082#C_example */

    int x = (radius - 1);
    int y = 0;
    int dx = 1;
    int dy = 1;
    int error = dx - (radius << 1);

    while (x >= y)
    {
	/* the loop fill every octant of the circle */
        /* Each of the following renders an octant of the circle */
        SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);

	if (error <= 0)
	{
	    ++y;
	    error += dy;
	    dy += 2;
	}

	if (error > 0)
	{
	    --x;
	    dx += 2;
	    error += dx - (radius << 1);
	}
    }
}

/**
 * @brief Scale Image to fit with a SDL_Rect
 *
 * @param SDL_Rect * rect, rectangle to be fitted with the image
 * @param SDL_Texture * img, image to scale
 * @param float window_height, height of the current window
 */
void sdl_scale_rect_image(SDL_Rect * rect, SDL_Texture * img, float wh, float ww, int horizontal)
{
    float scale_factor;
    (*rect).x = (*rect).y = 0;
    SDL_QueryTexture(img, NULL, NULL, &(*rect).w, &(*rect).h);

    if (horizontal) scale_factor = wh / ((float) (*rect).h);
    else scale_factor = ww / ((float) (*rect).w);
    zlog(stdout, DEBUG, "scale factor: %g", scale_factor);

    zlog(stdout, DEBUG, "img rect w|h before scale: %d %d", (*rect).w, (*rect).h);
    (*rect).h = (*rect).h*scale_factor;
    (*rect).w = (*rect).w*scale_factor;
    zlog(stdout, DEBUG, "img rect w|h after scale: %d %d", (*rect).w, (*rect).h);
}

/* ---------------- Text functions ---------------- */

/**
 * @brief Print text on a window
 *
 * @param SDL_Window * window, window to print
 * @param SDL_Renderer * renderer, renderer to update
 * @param TTF_Font * font, text font to print
 * @param char * text, string to print
 * @param SDL_Point p, position from where the text printed
 * @param SDL_Color color, color of the text
 */
void sdl_print_text(SDL_Window * window, SDL_Renderer * renderer,
                    TTF_Font * font, char * text, SDL_Point p, SDL_Color color)
{
    int iW, iH;
    int window_width, window_height;
    SDL_Surface * text_surf;     
    SDL_Texture * text_text;
    SDL_Rect rect;

    /* open the font */
    if (!text)
    {
	zlog(stderr, ERROR, "TTF: impossible d'écrire une chaine NULL", NULL);
	return;
    }

    /* create the render of the text */
    text_surf = TTF_RenderText_Blended(font, text, color);
    text_text = SDL_CreateTextureFromSurface(renderer, text_surf);

    SDL_GetWindowSize(window, &window_width, &window_height);

    /* center the text on the point */
    if (p.x == -1) p.x = (window_width - text_surf->w) >> 1;
    if (p.y == -1) p.y = (window_height - text_surf->h) >> 1;

    /* update rectangle of the text */
    rect.x = p.x;
    rect.y = p.y;
    rect.w = text_surf->w;
    rect.h = text_surf->h;

    /* render the text */
    SDL_QueryTexture(text_text, NULL, NULL, &iW, &iH);
    SDL_RenderCopy(renderer, text_text, NULL, &rect);

    /* free textures and surface */
    SDL_DestroyTexture(text_text);
    SDL_FreeSurface(text_surf);
}

/**
 * @brief Quit sdl
 */
void sdl_quit(void)
{
    SDL_Quit();
}

/**
 * @brief Quit sdl ttf
 */
void sdl_quit_text(void)
{
    TTF_Quit();
}

/**
 * @brief Init sdl ttf
 */
void sdl_init_text(void)
{
    if (TTF_Init() != 0)
    {
	zlog(stderr, ERROR, "Erreur d'initialisation TTF : %s\n", TTF_GetError());
    }
}

/**
 * @brief Draw a segment between two points
 *
 * @param renderer the rendering content
 * @param x1 x coordinate of the start point
 * @param x1 y coordinate of the start point
 * @param x1 x coordinate of the end point
 * @param x1 y coordinate of the end point
 */
void sdl_draw_segment(SDL_Renderer * renderer, int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);   
}


/**
 * @brief Set the icon of an SDL window.
 *
 * This function loads an image file containing the icon and sets it as the icon for the specified SDL window.
 *
 * @param window   The SDL window for which to set the icon.
 * @param icones   Pointer to the SDL surface that will hold the loaded icon image.
 */
void sdl_set_icon(SDL_Window*  window, SDL_Surface ** icones)
{
    * icones = IMG_Load("data/icones.png");
  if (!* icones) SDL_Log("Erreur chargement icones sdl_set_icon : %s\n", SDL_GetError());
  else
      SDL_SetWindowIcon(window, *icones);
}
