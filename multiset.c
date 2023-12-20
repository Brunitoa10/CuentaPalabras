#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "multiset.h"
#include "lista.h"
#include "errores.h"

struct trie{
    int cantidad; // Cantidad de veces que aparece esa palabra en el multiset
    struct trie *siguiente[26];
};


/** \brief *multiset_crear().
 *
 * \exception TMS_ERROR_MEMORIA
 *
 * \return	 arbolMultiset Si termina con existe.
 * \return	-10 Si es una posicion invalida.
 *
 *
 * Crea un multiset vacio de palabras y lo devuelve.
 */

multiset_t *multiset_crear(){
    multiset_t *arbolMultiSet = (multiset_t*)malloc(sizeof(struct trie));
    if(arbolMultiSet == NULL){
        printf("Error crear arbol --> No se pudo reservar memoria.");
        exit(TMS_ERROR_MEMORIA);
    }
    //Inicializo los bloques de memoria contiguos en NULL
    arbolMultiSet -> cantidad = 0;
    for(int pos = 0; pos < 26; pos++){
        arbolMultiSet -> siguiente[pos] = NULL;
    }
    return arbolMultiSet;
}

/** \brief multiset_insertar(multiset_t *m, char *s).
 *
 * \exception TMS_ERROR_NULO,TMS_ERROR_PALABRA_INVALIDA
 *
 * \return  -11 Si es un arbol nulo
 * \return	-12 Si es una palabra invalida.
 *
 * Inserta la palabra s al multiset m.
 */

void multiset_insertar(multiset_t *m, char *s){
    if(m == NULL || s == NULL){
        if(m == NULL){
            printf("Error insertar --> multiset NULL.");
                salto();
            exit(TMS_ERROR_NULO);
        }else{
            printf("Error insertar --> palabra invalida.");
                salto();
            exit(TMS_ERROR_PALABRA_INVALIDA);
        }
    }
    int longitudPalabra = strlen(s);
    int indicePalabra = 0;
    multiset_t *nodo_actual = m;

    for(int pos = 0; pos < longitudPalabra; pos++){
        indicePalabra = s[pos] - 'a';
        //Si no existe el nodo para la letra
        if(nodo_actual->siguiente[indicePalabra] == NULL){
            multiset_t *tmp_nodo = multiset_crear();
            nodo_actual -> siguiente[indicePalabra] = tmp_nodo;
        }
        nodo_actual = nodo_actual -> siguiente[indicePalabra];
    }
    nodo_actual->cantidad++;
}

void destruir(multiset_t *tmpNodo){
    for(int pos = 0; tmpNodo != NULL && pos < 26; pos++){
        destruir(tmpNodo->siguiente[pos]);
        tmpNodo->cantidad--;
    }
    free(tmpNodo);
}

/** \brief multiset_eliminar(multiset_t **m).
 *
 * \exception TMS_ERROR_NULO
 *
 * \return  -11 Si es un arbol nulo
 *
 * Elimina el multiset m liberando el espacio de memoria reservado. Luego de la invocacion m debe valer NULL..
 */

void multiset_eliminar(multiset_t **m){
    if(m == NULL || *m == NULL){
        if(*m == NULL){
            printf("Error -> El valor apuntado por el multiset es invalido.");
        }else{
            printf("Error --> Puntero invalido.");
        }
        salto();
        exit(TMS_ERROR_NULO);
    }
    //Le paso la raiz
    destruir(*m);
    *m = NULL;
}

/** \brief multiset_cantidad(multiset_t *m, char *s).
 *
 * \exception TMS_ERROR_NULO,TMS_ERROR_PALABRA_INVALIDA
 *
 * \return  -11 Si es un arbol nulo
 * \return	-12 Si es una palabra invalida.
 *
 * Devuelve la cantidad de repeticiones de la palabra s en el multiset m.
 */

int multiset_cantidad(multiset_t *m, char *s){
    if(m == NULL || s == NULL){
        if(m == NULL){
            printf("Error --> El valor apuntado por el arbol es nulo.");
                salto();
            exit(TMS_ERROR_NULO);
        }else{
            printf("Error --> El valor apuntado por la palabra es nulo.");
                salto();
            exit(TMS_ERROR_PALABRA_INVALIDA);
        }
    }

    multiset_t *tmpArbol = m;
    int longitudPalabra = strlen(s);
    int indicePalabra = 0;
    int cantApariciones = 0;
    int corte = false;

    for (int pos = 0; !corte && pos < longitudPalabra; pos++) {
        indicePalabra = s[pos] - 'a';

        if (tmpArbol -> siguiente[indicePalabra] == NULL) {
            corte = true;
        }else{
            tmpArbol = tmpArbol -> siguiente[indicePalabra];
            cantApariciones = tmpArbol->cantidad;
        }
    }
    return cantApariciones;
}

void insertarPreOrdenAux(multiset_t *arbol, lista_t *lista, char *palabra,int nivel) {
    elemento_t elemAux;
    if (arbol->cantidad > 0) {
        palabra[nivel] = '\0';
        elemAux.a = arbol->cantidad;
        elemAux.b = strdup(palabra);
        lista_insertar(lista,elemAux,lista_cantidad(lista));
    }

    for (int pos = 0; pos < 26; pos++) {
        if (arbol->siguiente[pos] != NULL) {
            palabra[nivel] = 'a' + pos;
            insertarPreOrdenAux(arbol->siguiente[pos],lista,palabra, nivel+1);
        }
    }
}

lista_t multiset_elementos(multiset_t *m, comparacion_resultado_t (*f)(elemento_t*,elemento_t*)){
    lista_t *lista_retorno = lista_crear();
    char *palabra = malloc(sizeof(char));

    insertarPreOrdenAux(m,lista_retorno,palabra,0);
   //lista_ordenar(lista_retorno,f);//problemas jeje

    return *lista_retorno;
}
