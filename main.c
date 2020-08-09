#include "pokedex.h"
#include <stdio.h>
#include "interfaz_pokedex.h"
#define TAM_MAXIMO 15

bool comando_valido(char comando, char comandos_validos[TAM_MAXIMO]){
    int pos = 0;
    bool valido = false;
    while ((comandos_validos[pos] != '#') && !valido){
        valido = (comandos_validos[pos] == comando);
        pos++;
    }
    return valido;
}

int main(){
    char primeros_comandos[] = {'I','S','H','#'};
    ///char segundos_comandos[] = {'H','G','S','A','E','C','V','M','P','#'};
    char comando =  '_';
    iniciar_interfaz();
    scanf(" %c",&comando);
    while (!comando_valido(comando,primeros_comandos)){
        interfaz_error();
        scanf(" %c",&comando);
    }
}
