#ifndef seed_h
#define seed_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void save_seed(long int seed);
void create_seed(long int * seed);
void load_seed(long int * seed);
void generate_seed(int save);

#endif
