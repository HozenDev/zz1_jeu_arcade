#include "seed.h"

void save_seed(long int seed)
{
    FILE *fp = fopen("seed_save", "w+");
    fprintf(fp, "%ld", seed);
    fclose(fp);
}

void create_seed(long int * seed)
{
    *seed = time(NULL);
}

void load_seed(long int * seed)
{
    FILE *fp = fopen("seed_save", "r+");
    fscanf(fp, "%ld", seed);
    fclose(fp);
}

void generate_seed(int behavior)
{
    long int seed;
    create_seed(&seed);
    switch (behavior) {
    case 0:
	save_seed(seed);
	break;
    case 1:
	load_seed(&seed);
	break;
    default:
	fprintf(stdout, "generate_seed(int behaviour)\tparameter invalid: behaviour (0 or 1)\n");
	fprintf(stdout, "\t=> random seed and save\n");
	save_seed(seed);
	break;
    }
    srand(seed);
}
