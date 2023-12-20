#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "errores.h"


void salto(){
    printf("\n\n");
}

struct celda {
    elemento_t *elem;
    struct celda *siguiente;
};

/** \brief *lista_crear.
 *
 * \exception LST_ERROR_MEMORIA
 *
 * \return	lista vacia Si termina con exito.
 *
 * \return	-3 Si hay un error de memoria.
 *
 * Crea una lista vacia y la devuelve.
 */

lista_t *lista_crear(){
    lista_t *listaSE = (lista_t*)malloc(sizeof(lista_t));

    if(listaSE == NULL){
        printf("Error --> No fue posible reservar espacio en memoria.");
        exit(LST_ERROR_MEMORIA);
    }
    listaSE -> primera = NULL;
    listaSE -> cantidad = 0;
    return listaSE;
}

/** \brief lista_insertar(lista_t *l, elemento_t elem, unsigned int pos).
 *
 * \exception LST_POSICION_INVALIDA, LST_ERROR_MEMORIA
 *
 * \return	 0 Si termina con existe.
 * \return	-3 Si es una posicion invalida.
 * \return	-3 Si hay un error de memoria.
 *
 * Inserta el elemento elem en la posicion pos de la lista.
 */

int lista_insertar(lista_t *l, elemento_t elem, unsigned int pos){
    //Verifico que sea una posicion valida
    if(pos > l -> cantidad){
        printf("Error --> Posicion invalida.");
        exit(LST_POSICION_INVALIDA);
    }
    //Creo una celda para el elemento a insertar
    celda_t *nueva_celda = (celda_t*)malloc(sizeof(celda_t));

    if(nueva_celda == NULL){
        printf("Error --> No fue posible reservar espacio en memoria para crear una celda.");
        exit(LST_ERROR_MEMORIA);
    }

    //Creo un nuevo elemento y le asigno el valor
    nueva_celda -> elem = (elemento_t*)malloc(sizeof(elemento_t));

    if(nueva_celda -> elem == NULL){
        printf("Error --> No fue posible reservar espacio en memoria para crear un elemento.");
        free(nueva_celda);
        exit(LST_ERROR_MEMORIA);
    }

    *(nueva_celda->elem) = elem;

    //Inserto en el principio de la lista
    if(pos == 0 || l -> primera == NULL){
        nueva_celda -> siguiente = l -> primera;
        l -> primera = nueva_celda;
    }else{
        //Encontrar la posicion a insertar
        celda_t *tmp_celda_anterior = l -> primera;
        unsigned int posTmp = 0;

        while(tmp_celda_anterior -> siguiente != NULL && posTmp < pos - 1){
            tmp_celda_anterior = tmp_celda_anterior -> siguiente;
            posTmp++;
        }
        //Inserto la celda en la posicion
        nueva_celda -> siguiente = tmp_celda_anterior -> siguiente;
        tmp_celda_anterior -> siguiente = nueva_celda;
    }
    l -> cantidad++;
    return EXITO;
}

/** \brief *lista_eliminar(lista_t *l, unsigned int pos).
 *
 * \exception LST_POSICION_INVALIDA, LST_VACIA
 *
 * \return	elem Si termina con existe.
 * \return	-3 Si es una posicion invalida.
 * \return	-5 Si la lista esta vacia.
 *
 * Elimina el elemento de la posicion pos de la lista.
 */

elemento_t *lista_eliminar(lista_t *l, unsigned int pos){
    //Si la lista esta vacia o es una posicion Invalida error
    if(lista_vacia(*l) || pos >= l -> cantidad){
        if(lista_vacia(*l)){
            printf("Error eliminar --> lista vacia.");
            exit(LST_VACIA);
        }else{
            printf("Error eliminar --> posicion invalida 1.");
            exit(LST_POSICION_INVALIDA);
        }
    }
    elemento_t *elem_retornar = NULL;
    celda_t *celda_eliminar;
    //Eliminar el primer elemento
    if(pos == 0){
        celda_eliminar = l -> primera;
        elem_retornar = celda_eliminar -> elem;
        l -> primera = celda_eliminar -> siguiente;
    }else{
        celda_t *tmp_celda_anterior = l -> primera;
        unsigned int posTmp = 0;

        while(posTmp < pos - 1){
            tmp_celda_anterior = tmp_celda_anterior -> siguiente;
            posTmp++;
        }
        celda_eliminar = tmp_celda_anterior -> siguiente;
        elem_retornar = celda_eliminar -> elem;
        tmp_celda_anterior -> siguiente = celda_eliminar -> siguiente;
    }
    free(celda_eliminar);
    l -> cantidad--;
    return elem_retornar;
}

/** \brief *lista_elemento(lista_t *l, unsigned int pos).
 *
 * \exception LST_POSICION_INVALIDA, LST_VACIA
 *
 * \return	 elem Si termina con existe.
 * \return	-3 Si es una posicion invalida.
 * \return	-5 Si la lista esta vacia.
 *
 * Devuelve un puntero al elemento que ocupa la posicion pos de la lista.
 */

elemento_t *lista_elemento(lista_t *l, unsigned int pos){
    if(lista_vacia(*l) || pos >= l -> cantidad){
        if(lista_vacia(*l)){
            printf("Error lista_elemento --> lista vacia.");
            exit(LST_VACIA);
        }else{
            printf("Error lista_elemento --> posicion invalida 1.");
            exit(LST_POSICION_INVALIDA);
        }
    }
    celda_t *tmp_celda = l -> primera;
    unsigned int posTmp = 0;

    while(tmp_celda != NULL && posTmp < pos){
        tmp_celda = tmp_celda -> siguiente;
        posTmp++;
    }

    if(tmp_celda == NULL){
        printf("Error lista_elemento --> posicion invalida 2.");
        exit(LST_POSICION_INVALIDA);
    }
    return tmp_celda -> elem;
}

//Metodo de ordenamiento
static void swap(elemento_t **elemA, elemento_t **elemB){
    elemento_t *tmp = *elemA;
    *elemA = *elemB;
    *elemB = tmp;
}

static void bubbleSortPro(lista_t *l, funcion_comparacion_t comparar){
    int huboCambio = true;
    celda_t *tmp_celdaActual;
    celda_t *tmp_celdaSiguiente = NULL;

    while(huboCambio) {
        huboCambio = false;
        tmp_celdaActual = l -> primera;

        while (tmp_celdaActual -> siguiente != tmp_celdaSiguiente) {
            if (comparar(tmp_celdaActual -> elem, tmp_celdaActual -> siguiente -> elem) == ELEM1_MAYOR_QUE_ELEM2) {
                swap(&(tmp_celdaActual -> elem), &(tmp_celdaActual -> siguiente -> elem));
                huboCambio = true;
            }
            tmp_celdaActual = tmp_celdaActual -> siguiente;
        }
            tmp_celdaSiguiente = tmp_celdaActual;
    }
}


/** \brief lista_ordenar(lista_t *l, funcion_comparacion_t comparar).
 *
 * \exception LST_COMPONENTE_NULA, LST_VACIA
 *
 * \return	-5 Si la lista esta vacia.
 * \return	-6 Si el elemento de la celda es NULL.
 *
 * Dada la lista l y la funcion comparar ordena la lista de acuerdo al
 * criterio de dicha funcion.
 */

int lista_ordenar(lista_t *l, funcion_comparacion_t comparar){
    if(l->cantidad > 0 && l->cantidad <= 1){
        printf("La lista ya esta ordenada");
        exit(EXITO);
    }else{
        if(lista_vacia(*l) || l -> primera == NULL){
            if(l -> primera == NULL){
                printf("Error -> Componente nula.");
                exit(LST_COMPONENTE_NULA);
            }else{
                if(lista_vacia(*l)){
                    printf("Error -> Lista vacia.");
                    exit(LST_VACIA);
                }
            }
        }else{
            //Ordeno de forma ascendente
            bubbleSortPro(l,comparar);
        }
    }
    return EXITO;
}


unsigned int lista_cantidad(lista_t *l){
    return l -> cantidad;
}

int lista_vacia(lista_t lista){
    return lista.cantidad == 0;
}
