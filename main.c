#include "pokedex.h"
#include <stdio.h>

int main(){
    pokedex_t* pokedex = pokedex_prender();
    pokedex_ultimos_capturados(pokedex);
    printf("________$$$$$__________\n");
    pokedex_ultimos_vistos(pokedex);
}
