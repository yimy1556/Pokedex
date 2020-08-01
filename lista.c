#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define CANT_INICIAL 0
#define ERROR -1
#define EXITO 0
#define INICIO 1
#define ANTE_ULTIMO 2
#define UN_ELEMENTO 1
#define POSICION_INICIAL 0
#define REMOVE 'R'
#define PROC_ELEMENTO 'P'

typedef struct nodo{
    void* dato;
    struct nodo* sig_nodo;
}nodo_t;

struct lista{
    nodo_t* primera_nodo;
    nodo_t* ultimo_nodo;
    size_t cantidad;
};

struct lista_iterador{
    nodo_t* nodo_actual;
};

/*
 * Crea un nodo con un elementon guardado.
 * y con la dirrecion de otro nodo.
 * PRE: Recive elemento a guardar y una dirrecion de otro nodo. 
 * POST: Devuelve nodo_t* si se puso crear un nodo.
 * delo contrario devuelve NULL.
*/
nodo_t* nodo_crear(void* elemento, nodo_t* sig_nodo){
    
    nodo_t* nodo = malloc(sizeof(nodo_t));

    if(!nodo) return NULL;
    
    nodo->dato = elemento;
    nodo->sig_nodo= sig_nodo;
    
    return nodo;
}

/*
 * Busca el nodo el la posicion recivida.
 * PRE: Recive la nodo_t**  y una posicion.
 * POST: devuelve nodo_t** de la posicion pedida. 
 */
nodo_t** posicionador(nodo_t** nodo, size_t posicion){

    if(posicion == CANT_INICIAL)
        return nodo;      
    
    posicion--;
     return posicionador(&((*nodo)->sig_nodo), posicion);
}

/*
 * Inserta un elemento en la posicion indicada de la lista, 
 * PRE: ----
 * POST: Devuelve 0 si pudo insertar o -1 si no pudo.
 */
int insertar_en_posicion(lista_t *lista, void *elemento, size_t posicion){

    nodo_t** posicion_a_insertar = posicionador(&(lista->primera_nodo), posicion);   
    nodo_t* nodo_aux = nodo_crear(elemento, *posicion_a_insertar);
    
    if (!nodo_aux) return ERROR;

    *posicion_a_insertar = nodo_aux; 
    lista->cantidad++;    

    return EXITO;
}

/*
 * borra de la lista el elemento que se encuentra en la posición
 * indicada.
 * PRE: ---
 * POST: Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int borrar_de_posicion(lista_t* lista, size_t posicion){

    nodo_t** nodo_aux = posicionador(&(lista->primera_nodo),posicion);
    nodo_t* nodo_a_eliminar = *nodo_aux;
    *nodo_aux = nodo_a_eliminar->sig_nodo;
    lista->cantidad--;
    
    free(nodo_a_eliminar);
    return  EXITO;
}

/*
 * Destruye todos los nodos.
 * PRE:----
 * POST:----
 */
void destruir_nodos(nodo_t* nodos){
   
    if(!nodos)return;
    
    destruir_nodos(nodos->sig_nodo);
    free(nodos);    
}

/// Operaciones De lista. 

lista_t* lista_crear(){

    lista_t* lista = malloc(sizeof(lista_t));
    
    lista->primera_nodo = NULL ;
    lista->ultimo_nodo = NULL;
    lista->cantidad = CANT_INICIAL;
    return lista;
}

int lista_insertar(lista_t *lista, void *elemento){
    
    if(!lista) return ERROR;

    nodo_t* nodo = nodo_crear(elemento, NULL); 
    
    if(!nodo) return ERROR;
   
    (lista_vacia(lista))? (lista->primera_nodo = nodo):
        (lista->ultimo_nodo->sig_nodo = nodo);
    
    lista->ultimo_nodo = nodo;
    lista->cantidad++;
            
    return EXITO;
}

int lista_insertar_en_posicion(lista_t *lista, void *elemento, size_t posicion){ 

    if(!lista) return ERROR;

    int estado = (posicion >= lista_elementos(lista))?lista_insertar(lista,elemento)
        :insertar_en_posicion(lista, elemento, posicion);        

    return estado;
}

int lista_borrar(lista_t* lista){
    
    if (!lista || lista_vacia(lista)) return ERROR;
    
    bool un_elemento = lista_elementos(lista) == UN_ELEMENTO;
    size_t ante_ultimo = (un_elemento)? 0:lista->cantidad - ANTE_ULTIMO; 
    nodo_t** nodo_aux = posicionador(&(lista->primera_nodo), ante_ultimo);
    nodo_t* nodo_a_borrar = (un_elemento)? *nodo_aux:(*nodo_aux)->sig_nodo; 
   
    (*nodo_aux)->sig_nodo = NULL;
    free(nodo_a_borrar);
    lista->primera_nodo = (un_elemento)? NULL: lista->primera_nodo;
    lista->ultimo_nodo = (un_elemento)? NULL: *nodo_aux;
    lista->cantidad--;
    
    return EXITO;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){

    if(!lista) return ERROR;

    int estado = (++posicion >= lista_elementos(lista))? 
        lista_borrar(lista):borrar_de_posicion(lista, --posicion);

    return estado;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){ 
    
    bool posicion_valida = (posicion < lista_elementos(lista));
    
    if (!lista || !posicion_valida) return NULL;
    
    nodo_t** nodo_aux = posicionador(&(lista->primera_nodo), posicion);

    return (*nodo_aux)->dato;
}

void* lista_ultimo(lista_t* lista){

    return (lista_vacia(lista))?  NULL:lista->ultimo_nodo->dato;

}

bool lista_vacia(lista_t* lista){

    return (CANT_INICIAL == lista_elementos(lista));
}

size_t lista_elementos(lista_t* lista){

    return (lista)? lista->cantidad:CANT_INICIAL;    
}

int lista_apilar(lista_t* lista, void* elemento){

    return lista_insertar(lista, elemento);
}

int lista_desapilar(lista_t* lista){

    return lista_borrar(lista);
}

void* lista_tope(lista_t* lista){

    return lista_ultimo(lista);

}

int lista_encolar(lista_t* lista ,void* elemento){

    return lista_insertar(lista, elemento);

}

int lista_desencolar(lista_t* lista){

    return lista_borrar_de_posicion(lista, POSICION_INICIAL);
}

void* lista_primero(lista_t* lista){

    return lista_elemento_en_posicion(lista, POSICION_INICIAL);
}

void lista_destruir(lista_t* lista){

    if(!lista) return;
    
    destruir_nodos(lista->primera_nodo);
    free(lista);
}

/// Iterador Externo 

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    
    if (!lista) return NULL;
    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
    if(!iterador) return  NULL;
    iterador->nodo_actual = lista->primera_nodo;

    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){

    return (iterador && iterador->nodo_actual);

}

void* lista_iterador_siguiente(lista_iterador_t* iterador){

    if (!lista_iterador_tiene_siguiente(iterador)) return NULL;
    
    void* elemento = iterador->nodo_actual->dato;
    iterador->nodo_actual = iterador->nodo_actual->sig_nodo;
    
    return elemento; 
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    free(iterador);

}

/// iterador Interno de la lista

void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*, void*), void *contexto){
    
    if (!lista || !funcion || !(lista->primera_nodo)) return;

    nodo_t* nodo_aux = lista->primera_nodo;
    lista->primera_nodo = nodo_aux->sig_nodo;
    funcion(nodo_aux->dato, contexto);
    lista_con_cada_elemento(lista, funcion, contexto);
    lista->primera_nodo = nodo_aux;
}
