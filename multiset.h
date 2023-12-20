#ifndef MULTISET_H_INCLUDED
#define MULTISET_H_INCLUDED
#include "lista.h"


struct trie;
typedef struct trie multiset_t;

//Crea un multiset vacio de palabras y lo devuelve.
extern multiset_t *multiset_crear();

//Inserta la palabra s al multiset m.
extern void multiset_insertar(multiset_t *m, char *s);

//Devuelve la cantidad de repeticiones de la palabra s en el multiset m.
extern int multiset_cantidad(multiset_t *m, char *s);

//Devuelve una lista de tipo lista t con todos los elementos del multiset m y la cantidad de apariciones de cada uno
extern lista_t multiset_elementos(multiset_t *m, comparacion_resultado_t (*f)(elemento_t*, elemento_t*));

//Elimina el multiset m liberando el espacio de memoria reservado. Luego de la invocaci´on m debe valer NULL.
extern void multiset_eliminar(multiset_t **m);

#endif // MULTISET_H_INCLUDED
