#include "../log/log.h"
#include "../game/game.h"
#include "../seed/seed.h"

int main(int argc, char *argv[])
{
    int save = 1;
    long int seed;

    create_seed(&seed);
    
    enable_log(argc, argv);

    if (argc > 2)
    {
        if (strcmp(argv[2], "--load") == 0)
        {
            load_seed(&seed);
            save = 0;
            zlog(stdout, INFO, "chargement de la seed: %ld", seed);
        }
    }
    
    if (save) save_seed(seed);

    srand(seed);
    
    game_loop();
    
    return 0;
}
