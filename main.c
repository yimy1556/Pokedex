#include "pokedex.h"
#include <stdio.h>
#include <stdbool.h>
#include "interfaz_pokedex.h"
#define TAM_MAXIMO 15 
#define AVISTAMIENTO "avistamientos.txt"
#define EVOLUCIONES "evoluciones.txt"
#define SALIR 'S'
#define INICIAR 'I'
#define GUARDAR 'G'
#define AYUDA 'H'
#define AVISTAR 'A'
#define EVOLUCUIONAR 'E'
#define CAPTURADOS 'C'
#define VISTOS 'V'
#define INFORMACION_ESPECIE 'M'
#define INFORMACION_POKEMON 'P'

bool comando_valido(char comando, char comandos_validos[TAM_MAXIMO]){
    int pos = 0;
    bool valido = false;
    while ((comandos_validos[pos] != '#') && !valido){
        valido = (comandos_validos[pos] == comando);
        pos++;
    }
    return valido;
}

void  ingreso_comando(char* comando){
    printf("ingrese comando = ");
    scanf(" %c",comando);
}
void ingreso_nombre_entrenador(char nombre[TAM_MAXIMO]){
    printf("Ingrese Nombre = ");
    scanf(" %s", nombre);
}

void primera_etapa(char* comando, char comandos_validos[TAM_MAXIMO],bool* salir ){
    do{  
        while (!comando_valido(*comando,comandos_validos)){
            interfaz_error();
            ingreso_comando(comando);
        }
        if(*comando == AYUDA) {
            interfaz_primera_etapa();
            ingreso_comando(comando);
        }
        if (*comando == SALIR) *salir = true;
    }while ((*comando != INICIAR) && !*salir);
}

void informacion_pokedex(pokedex_t* pokedex,bool tipo_info){
    int especie = 0;
    char nombre_pokemon[TAM_MAXIMO];
    printf("ingrese numero de especie = ");
    scanf(" %i ", &especie);
    if (tipo_info){
        printf("ingrese nombre de pokemon = ");
        scanf(" %s",nombre_pokemon);
    }
    pokedex_informacion(pokedex,especie,(tipo_info)? nombre_pokemon:"");
}


void segunda_etapa(char* comando, bool* salir){
    if (*salir) return;
    pokedex_t* pokedex = pokedex_prender();
    interfaz_ayuda();
    do{
        ingreso_comando(comando);
        switch (*comando){
            case AYUDA:
                interfaz_ayuda();
                break;
            case AVISTAR:
                pokedex_avistar(pokedex,AVISTAMIENTO);
                break;
            case SALIR:
                *salir = true;
                break;
            case EVOLUCUIONAR:
                pokedex_evolucionar(pokedex,EVOLUCIONES);
                break;
            case CAPTURADOS:
                pokedex_ultimos_capturados(pokedex);
                break;
            case VISTOS:
                pokedex_ultimos_vistos(pokedex);
                break;
            case INFORMACION_ESPECIE:
                informacion_pokedex(pokedex,false);
                break;
            case INFORMACION_POKEMON:
                informacion_pokedex(pokedex,true);
                break;    
            default:
                interfaz_error_segunda_etapa();
                break;
        }
    } while (!*salir);
}



int main(){
    bool salir_pokedex = false;
    char primeros_comandos[] = {'I','S','H','#'};
    char comando =  '_';
    iniciar_interfaz();
    ingreso_comando(&comando);
    primera_etapa(&comando,primeros_comandos,&salir_pokedex);
    segunda_etapa(&comando,&salir_pokedex);
    interfaz_salida();
}