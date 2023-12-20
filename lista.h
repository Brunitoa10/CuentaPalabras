#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED


enum {true = 1, false = 0};

typedef enum {
    ELEM1_MAYOR_QUE_ELEM2,
    ELEM1_IGUAL_QUE_ELEM2,
    ELEM1_MENOR_QUE_ELEM2
} comparacion_resultado_t;

struct elemento {
    int a;
    char *b;
};

typedef struct elemento elemento_t;

typedef comparacion_resultado_t (funcion_comparacion_t) (
    elemento_t *elem1, elemento_t *elem2
);

typedef struct celda celda_t;

struct lista {
    celda_t* primera; //puntero a la primera celda de la lista
    int cantidad; // cantidad de elementos de la lista
};

typedef struct lista lista_t;

//Crea una lista vacia y la devuelve.
extern lista_t *lista_crear();

//Inserta el elemento elem en la posicion pos de la lista.
extern int lista_insertar(lista_t *l, elemento_t elem, unsigned int pos);

//Elimina el elemento de la posicion pos de la lista.
extern elemento_t *lista_eliminar(lista_t *l, unsigned int pos);

//Devuelve un puntero al elemento que ocupa la posicion pos de la lista.
extern elemento_t *lista_elemento(lista_t *l, unsigned int pos);

//Dada la lista l y la funcion comparar ordena la lista de acuerdo al criterio de dicha funcion.
extern int lista_ordenar(lista_t *l, funcion_comparacion_t comparar);

//Devuelve la cantidad de elementos de la lista l.
extern unsigned int lista_cantidad(lista_t *l);

// Devuelve verdadero (distinto a 0) si la lista esta vacia, y falso (igual a 0) si la lista contiene al menos un elemento.
extern int lista_vacia(lista_t lista);

//Borrar
extern void salto();

#endif // LISTA_H_INCLUDED
