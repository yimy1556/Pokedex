#include "pokedex.h"
#include <stdio.h>

int main(){
    pokedex_t* pokedex =pokedex_crear("yimy");
    pokedex_avistar(pokedex,"avistamientos.txt");
    printf("Ultimos Capturados\n");
    pokedex_ultimos_capturados(pokedex);
    printf("Ultimos Vistos\n");
    pokedex_ultimos_vistos(pokedex);
    printf("-------------------fsdsd----------------------\n");
    pokedex_informacion(pokedex,25,"Pikante");
}
