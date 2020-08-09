#include <stdio.h>
#include "interfaz_pokedex.h"

#define ANSI_COLOR_BLUE    "\x1b[34m"      
#define ANSI_COLOR_ROJO    "\x1b[1m\x1b[31m"
#define ANSI_COLOR_VERDE    "\x1b[1m\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"



void iniciar_interfaz(){
printf("%s",ANSI_COLOR_ROJO);
printf("                  ##############                     \n");
printf("                ##################                   \n");
printf("              ##########  #########                  \n");
printf("%s",ANSI_COLOR_BLUE);
printf("-~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~-\n");
printf("|                    POKEDEX                        |\n");
printf("|  La Pokedex te permitira  guardar y ver los datos |\n");
printf("|  de todos los pokemones que estaras encontrado en |\n");
printf("|           tu caminino como entrenador             |\n");
printf("|                                                   |\n");
printf("|          comando para I  iniciar pokedex          |\n");
printf("|          comando para H  ver mas comandos         |\n");
printf("|            comando para S  para salir             |\n");
printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
printf("%s",ANSI_COLOR_RESET);
printf("              ##########  #########              \n");
printf("               ###################                 \n ");
printf("                ###############                    \n");
}

void  interfaz_ayuda(){
    printf("%s",ANSI_COLOR_ROJO);
printf("                  ##############                     \n");
printf("                ##################                   \n");
printf("              ##########  #########                  \n");
printf("%s",ANSI_COLOR_BLUE);
printf("-~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~-\n");
printf("|        Comandos validos para la POKEDEX           |\n");
printf("|                                                   |\n");
printf("|           Comando G  guardar pokedex              |\n");
printf("|           comando A  avistar Pokemon              |\n");
printf("|         comando E  evolucionar Pokemon            |\n");
printf("|      comando C para ver pokemones Capturas        |\n");
printf("|       comando V para ver pokemones vistos         |\n");
printf("|    comando  M para ver informacion de especie     |\n");
printf("|    comando  P para ver informacion de especie     |\n");
printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
printf("%s",ANSI_COLOR_RESET);
printf("              ##########  #########              \n");
printf("               ###################                  \n ");
printf("                 ###############                    \n");
}

void  interfaz_error(){
    printf("%s",ANSI_COLOR_ROJO);
printf("                  ##############                     \n");
printf("                ##################                   \n");
printf("              ##########  #########                  \n");
printf("%s",ANSI_COLOR_BLUE);
printf("-~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~-\n");
printf("|             %s COMANDO INVALIDO %s                    |\n",ANSI_COLOR_ROJO, ANSI_COLOR_BLUE);
printf("|                                                   |\n");
printf("|       Para ver Comados validos presione H         |\n");
printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
printf("%s",ANSI_COLOR_RESET);
printf("              ##########  #########               \n");
printf("               ###################                  \n ");
printf("                 ###############                    \n");
}


bool comandos_interfaz_validos(char comando);
