#ifndef _utils_h_
#define _utils_h_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../sdl/sdl.h"

struct colors_s {
    SDL_Color BLACK;
    SDL_Color WHITE;
    SDL_Color RED;
    SDL_Color GREEN;
    SDL_Color BLUE;
    SDL_Color YELLOW;
};

extern const struct colors_s colors_available;

float distance(SDL_Point pt1, SDL_Point pt2);

void free_matrix_char(char ** matrix, unsigned int n);

void free_matrix_float(float ** matrix, unsigned int n);

int generate_random_number(int min, int max);

int max(int a, int b);
int min(int a, int b);
void utils_matrix_copy(float ** srxmatrix,float ** destmatrix,int n);
void utils_swap(int *a,int *b);
void utils_shuffle(int* liste,int taille);
void utils_initlist0(int * liste,int taille);
void utils_distance_liste(int * indice,float ** distance,float * dist,int taille);
void utils_copy_list(int *listsrc,int* listdst,int taille);

#endif
