#include "pokedex.h"
#include "lista.h"
#include "abb.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define IGUAL 0
#define MAYOR 1
#define MENOR -1
#define MODO_LECTURA "r"
#define MODO_CREAR "w"
#define EXITO 0
#define ERROR -1
#define FORMATO_ESPECIA "%d;%[^;];%[^;];"
#define FORMATO_POKEMON "%[^;];%d;%c\n"
#define FORMATO_EVOLUCIONES "%d;%[^;];%d;%[^;];%[^\n]\n"
#define NO_CAPTURADO 'N'
#define SI_CAPTURADO 'S'
#define INFORMACIO_POKEMONES "pokedex1.txt"

/** Función para el abb**/
void pokemones_destruir(lista_t* pokemones){
    if (lista_vacia(pokemones)) return;
    free(lista_primero(pokemones));
    lista_desencolar(pokemones);    
}

void destructor_especia(void* elemento){
    pokemones_destruir(((especie_pokemon_t*)elemento)->pokemones);
    lista_destruir(((especie_pokemon_t*)elemento)->pokemones);
    free(elemento);
}

int comparador_especie_pokemon(void* especie_pokemon1 ,void* especie_pokemon2){
    especie_pokemon_t* aux1 = (especie_pokemon_t*)especie_pokemon1;
    especie_pokemon_t* aux2 = (especie_pokemon_t*)especie_pokemon2;

    if(aux2->numero == aux1->numero) return IGUAL;

    return (aux1->numero < aux2->numero)? MENOR:MAYOR;
}

/* Funciones aux*/

void mostrar_pokemon(particular_pokemon_t* pokemon){
    printf("%s\n",pokemon->nombre);
    printf("%i\n",pokemon->nivel);
    printf("%s\n",(pokemon->capturado)?"Capturado":"No Capturado");
}


void mostrar(especie_pokemon_t* especie){
    printf("%i\n",especie->numero);
    printf("%s\n",especie->nombre);
    printf("%s\n",especie->descripcion);
}

especie_pokemon_t* crear_especie(){
    especie_pokemon_t* especie = malloc(sizeof(especie_pokemon_t));

    if (!especie) return NULL;
    
    especie->pokemones= lista_crear();

    if(!especie->pokemones){
        free(especie);
        especie = NULL;
    }
    
    return especie;
}

especie_pokemon_t* copia_especia(especie_pokemon_t* especie){
    especie_pokemon_t* copia =  crear_especie();
    
    if (!copia) return NULL;

    strcpy(copia->descripcion,especie->descripcion);
    strcpy(copia->nombre,especie->nombre);
    copia->numero = especie->numero;
    
    if(ERROR == lista_insertar(copia->pokemones,lista_primero(especie->pokemones)))
        destructor_especia(copia);        

    return copia; 
}

particular_pokemon_t* crear_pokemon2(FILE* arch_pokemon){
    particular_pokemon_t* pokemon =  malloc(sizeof(particular_pokemon_t));
    char capturado = '_';
    if(!pokemon) return NULL;

    fscanf(arch_pokemon,"%[^;];%i;%c\n",pokemon->nombre,&(pokemon->nivel),&capturado);
    pokemon->capturado = (capturado == 'S')? true:false;
    return pokemon;
}

especie_pokemon_t* crear_especie2(FILE* arch_pokemon){
    especie_pokemon_t* especie = crear_especie();

    if(!especie) return NULL;

    fscanf(arch_pokemon,especie->nombre,&(especie->numero),especie->descripcion);

    return especie;
}

int  cargar_pokemones(FILE* archivo, lista_t* lista_pokemones , pokedex_t* pokedex){
    char tipo = '_';
    int estado = EXITO;
    fscanf(archivo,"%c;",&tipo);
    while (!feof(archivo) && (tipo == 'P') && (estado == EXITO)){
        particular_pokemon_t* pokemon = crear_pokemon2(archivo);
        
        if(!pokemon){
            estado = ERROR;
            continue;
        }
    
        estado = lista_insertar(lista_pokemones,pokemon);
        if (estado == EXITO) estado = vistos_o_capturados(pokedex,lista_pokemones);
        
        fscanf(archivo,"%c;",&tipo);
    }
    return estado;    
}

int lectura_pokedex(FILE* archivo, pokedex_t* pokedex){
    char tipo = '_';
    int estado = EXITO;
    fscanf(archivo,"%c;",&tipo);

    while (!feof(archivo) && (estado == EXITO) && tipo == 'E'){
        especie_pokemon_t* especie =  crear_especie2(archivo);

        if(!especie){
            estado = ERROR;
            continue;
        }

        if(ERROR == cargar_pokemones(archivo, especie->pokemones,pokedex)){
            destructor_especia(especie);
            estado = ERROR;
        }

        if (estado == EXITO) arbol_insertar(pokedex->pokemones,especie);
    }
    return estado;
}

int cargar_especie_en_pokedex(pokedex_t* pokedex, especie_pokemon_t* especie){
    especie_pokemon_t* especie_copia = copia_especia(especie);
    int estado = (especie_copia)? arbol_insertar(pokedex->pokemones,especie_copia):ERROR;
    if(estado == ERROR) destructor_especia(especie_copia);
    
    return estado;
}

int cargar_especie(pokedex_t* pokedex, especie_pokemon_t* especie){
    especie_pokemon_t* especie_aux = arbol_buscar(pokedex->pokemones ,especie);

    int estado = (especie_aux)? lista_insertar(especie_aux->pokemones,lista_primero(especie->pokemones)):
        cargar_especie_en_pokedex(pokedex,especie); 
    
    return  estado;
}

int vistos_o_capturados(pokedex_t* pokedex,lista_t* lista_pokemones){
    particular_pokemon_t* pokemon = lista_ultimo(lista_pokemones);
    lista_t* aux = (pokemon->capturado)? pokedex->ultimos_capturados:pokedex->ultimos_vistos;  
    int estado = (pokemon->capturado)? lista_encolar(aux, pokemon):lista_apilar(aux, pokemon);
    
    return estado;
}

int cargar_pokedex(pokedex_t* pokedex, especie_pokemon_t* especie){
    int estado = cargar_especie(pokedex, especie);
    
    if (estado == ERROR) return estado;

    estado = vistos_o_capturados(pokedex,especie->pokemones);
    lista_desencolar(especie->pokemones);
    return estado;
}

int lectura_pokemon(FILE* pokemones, especie_pokemon_t* especie){
    particular_pokemon_t* pokemon = malloc(sizeof(particular_pokemon_t));
    char capturado = '_';

    if(!pokemon) return ERROR;

    fscanf(pokemones, FORMATO_POKEMON, pokemon->nombre,&(pokemon->nivel),&capturado);
    pokemon->capturado=(capturado == SI_CAPTURADO)? true:false;
    mostrar_pokemon(pokemon);
    printf("---------------------------------\n");
    lista_insertar(especie->pokemones,pokemon);

    return EXITO;
}

/*Revisar name*/
int lectura_especia(FILE* avistamiento, pokedex_t* pokedex,especie_pokemon_t* especie){
    
    fscanf(avistamiento, FORMATO_ESPECIA, &(especie->numero),especie->nombre,especie->descripcion);
    int estado = lectura_pokemon(avistamiento,especie);
    
    return estado;
}

int lectura_avistamiento(FILE* avistamiento, pokedex_t* pokedex){
    especie_pokemon_t* especie = crear_especie();
    int estado = (especie)? EXITO:ERROR;
    
    while(!feof(avistamiento) && estado == EXITO){
        estado = lectura_especia(avistamiento, pokedex, especie);
        
        if(estado == ERROR) continue;    

        estado = cargar_pokedex(pokedex,especie);
    }
    
    destructor_especia(especie);
    return estado;
}

size_t posicion_pokemon_buscado(lista_t* pokemones, especie_pokemon_t* pokemon_buscado){
    lista_iterador_t* lista_pokemones = lista_iterador_crear(pokemones);
    size_t posicion_pokemon = 0;
    bool pokemon_encontrado = false;
    
    while (lista_iterador_tiene_siguiente(lista_pokemones) || !pokemon_encontrado){
        particular_pokemon_t* pokemon = lista_iterador_siguiente(lista_pokemones);
        
        if (strcmp(pokemon_buscado->nombre, pokemon->nombre) == 0)  
            pokemon_encontrado = pokemon->capturado;

        posicion_pokemon++;
    }
    
    lista_iterador_destruir(lista_pokemones);
    posicion_pokemon--;

    return posicion_pokemon;
}

particular_pokemon_t* evolucion_pokemon(pokedex_t* pokedex,especie_pokemon_t* especie){
    especie_pokemon_t* especie_pokemon  = arbol_buscar(pokedex->pokemones,especie);
    size_t posicion_pokemon = posicion_pokemon_buscado(especie_pokemon->pokemones,especie);
    particular_pokemon_t* pokemon_buscado = lista_elemento_en_posicion(especie_pokemon->pokemones,posicion_pokemon);
    lista_elemento_en_posicion(especie_pokemon->pokemones,posicion_pokemon);
    return pokemon_buscado;    
}

int lectura_evolucion(FILE* evoluciones, pokedex_t* pokedex, especie_pokemon_t* especie){
    especie_pokemon_t evolucion;

    fscanf(evoluciones,FORMATO_EVOLUCIONES,&(evolucion.numero),evolucion.nombre,&(especie->numero),especie->nombre,especie->descripcion);
    
    return lista_insertar(especie->pokemones,evolucion_pokemon(pokedex, &evolucion));
}

int lectura_evoluciones(FILE* evoluciones, pokedex_t* pokedex){
    especie_pokemon_t* especie = crear_especie();
    int estado = (especie)? EXITO:ERROR;
    
    while(!feof(evoluciones) && estado == EXITO){
        estado = lectura_evolucion(evoluciones, pokedex, especie);
        
        if(estado == ERROR) continue;    
    
        estado = cargar_pokedex(pokedex, especie);
    }
    
    destructor_especia(especie);
    fclose(evoluciones);
    return estado;
}

void info(lista_t* pokemones, FILE* arch){
    if (lista_vacia(pokemones)) return;
    particular_pokemon_t* pokemon = lista_primero(pokemones);
    char caturado = (pokemon->capturado)? 'S':'N';
    fprintf(arch,"%c;%s;%i;%c\n",'P',pokemon->nombre,pokemon->nivel,caturado);
    lista_desencolar(pokemones);
    info(pokemones, arch);
    lista_encolar(pokemones, pokemon);
}


bool crear_archivo_p(void* dato,void* arch){
    especie_pokemon_t* especie = (especie_pokemon_t*)dato;
    fprintf(((FILE*)arch),"%c;%s;%i;%s",'E',especie->nombre,especie->numero,especie->descripcion);
    info(especie->pokemones, arch);    
    return false;
}

/** Función Generales **/
pokedex_t* pokedex_crear(char entrenador[MAX_NOMBRE]){
    pokedex_t* pokedex = malloc(sizeof(pokedex_t));

    if(!pokedex) return NULL;

    strcpy(pokedex->nombre_entrenador, entrenador);
    pokedex->ultimos_capturados = lista_crear();
    pokedex->ultimos_vistos = lista_crear();
    pokedex->pokemones = arbol_crear(comparador_especie_pokemon, destructor_especia);

    return pokedex;
}

int pokedex_avistar(pokedex_t *pokedex, char ruta_archivo[MAX_RUTA]){
    if (!pokedex) return ERROR;

    FILE* pokemones = fopen(ruta_archivo, MODO_LECTURA);

    if(!pokemones) return ERROR;

    return lectura_avistamiento(pokemones, pokedex);
}

int pokedex_evolucionar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]){
    FILE* evoluciones = fopen(ruta_archivo, MODO_LECTURA);

    if (!evoluciones) return ERROR;
    
    return lectura_evoluciones(evoluciones, pokedex);
}

void pokedex_ultimos_capturados(pokedex_t* pokedex){
    if(lista_vacia(pokedex->ultimos_capturados)) return;

    particular_pokemon_t* pokemon = lista_ultimo(pokedex->ultimos_capturados);
    lista_desapilar(pokedex->ultimos_capturados);
    mostrar_pokemon(pokemon);
    pokedex_ultimos_capturados(pokedex);

    lista_apilar(pokedex->ultimos_capturados,pokemon);
}

void pokedex_ultimos_vistos(pokedex_t* pokedex){
    if(lista_vacia(pokedex->ultimos_vistos)) return;

    particular_pokemon_t* pokemon = lista_primero(pokedex->ultimos_vistos);
    lista_desencolar(pokedex->ultimos_vistos);
    mostrar_pokemon(pokemon);
    pokedex_ultimos_vistos(pokedex);
    lista_encolar(pokedex->ultimos_vistos, pokemon);
}

void pokedex_informacion(pokedex_t* pokedex, int numero_pokemon, char nombre_pokemon[MAX_NOMBRE]){
    especie_pokemon_t especie_buscado;
    especie_buscado.numero = numero_pokemon;
    strcpy(especie_buscado.nombre, nombre_pokemon);
    especie_pokemon_t* especie_encontrada = arbol_buscar(pokedex->pokemones, &especie_buscado);    
    if(!especie_encontrada) return;
    size_t posicion_pokemon = posicion_pokemon_buscado(especie_encontrada->pokemones,&especie_buscado);
    particular_pokemon_t* pokemon_buscado = lista_elemento_en_posicion(especie_encontrada->pokemones,posicion_pokemon);
    mostrar_pokemon(pokemon_buscado);
}

void pokedex_destruir(pokedex_t* pokedex){
    lista_destruir(pokedex->ultimos_capturados);
    lista_destruir(pokedex->ultimos_vistos);
    arbol_destruir(pokedex->pokemones);
    free(pokedex);
}

int pokedex_apagar(pokedex_t* pokedex){
    FILE* archivo_pokedex = fopen(INFORMACIO_POKEMONES, MODO_CREAR);
    
    if (!archivo_pokedex) return ERROR;
    abb_con_cada_elemento(pokedex->pokemones,ABB_RECORRER_POSTORDEN,crear_archivo_p,archivo_pokedex);
    pokedex_destruir(pokedex);
    return EXITO;
}

pokedex_t* pokedex_prender(){
    FILE* archivo_pokedex = fopen(INFORMACIO_POKEMONES,MODO_LECTURA);
    if(!archivo_pokedex) return NULL;

    pokedex_t* pokedex = pokedex_crear("");

    if(!pokedex) return NULL;

    fscanf(archivo_pokedex,"%[^\n]\n", pokedex->nombre_entrenador);
    lectura_pokedex(archivo_pokedex,pokedex);

    return pokedex;
}
