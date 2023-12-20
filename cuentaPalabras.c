#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "lista.h"
#include "multiset.h"
#include "errores.h"

/** \brief comparar_Elementos(elemento_t *elemA, elemento_t *elemB).
 *
 * \return	ELEM1_MENOR_QUE_ELEM2 si a<b
 * \return	ELEM1_MAYOR_QUE_ELEM2 si a>b
 * \return	ELEM1_IGUAL_QUE_ELEM2 si a=b
 *
 * Compara dos elementos.
 */

comparacion_resultado_t comparar_Elementos(elemento_t *elemA, elemento_t *elemB){
    comparacion_resultado_t retorno;
    int valorElemA = elemA->a , valorElemB = elemB->a;
    if(valorElemA < valorElemB){
        retorno = ELEM1_MENOR_QUE_ELEM2;
    }else{
        if(valorElemA > valorElemB){
            retorno = ELEM1_MAYOR_QUE_ELEM2;
        }else{
            if(valorElemA == valorElemB){
                retorno = ELEM1_IGUAL_QUE_ELEM2;
            }
        }
    }
    return retorno;
}

static void ayudaCuentaPalabras(){
        salto();

    printf("Cuenta las palabras en los archivos .txt de un directorio.");
        salto();
    printf("Uso: cuentapalabras [directorio de entrada]");
        salto();
    printf("Opciones:");
        salto();
    printf("-h \t\t Mostrar esta ayuda por pantalla.");
        salto();
    printf("-3 \t\t Test cuentaPalabras.");
        salto();
}

/** \brief cargarArbolConPalabras(FILE *archivo, multiset_t *arbol).
 *
 * Carga el multiset con las palabras del archivo
 */

static void cargarArbolConPalabras(FILE *archivo, multiset_t *arbol){
    char *palabra = malloc(26*sizeof(char));
    while(!feof(archivo)){
        fscanf(archivo, "%26s[^\n] ", palabra);
        multiset_insertar(arbol,palabra);
    }
    free(palabra);
}

/** \brief abrirArchivo(char *nombreArchivo,char *modoApertura).
 *
 * \exception ARCHIVO_ERROR_APERTURA
 * \return	-40 Si no se pudo abrir el archivo
 * \return	puntero al archivo abierto
 *
 * Abre el archivo [nombreArchivo] con un formato en especifico
 * de apertura [modoApertura].
 */

static FILE *abrirArchivo(char *nombreArchivo,char *modoApertura){
    printf("Funcion abrir archivo %s",nombreArchivo);
    FILE *archivo = fopen(nombreArchivo, modoApertura);
    if (archivo == NULL) {
        printf("Error --> En apertura de archivo :: %s", nombreArchivo);
            salto();
        exit(ARCHIVO_ERROR_APERTURA);
    }
    printf("Apertura exitosa :: %s", nombreArchivo);
        salto();
    return archivo;
}

/** \brief copiar_lista_a_archivo(lista_t *lista, FILE *archivo).
 *
 * Copia el contenido del archivo en la lista
 */

static void copiar_lista_a_archivo(lista_t *lista, FILE *archivo){
    elemento_t *tmpElem = NULL;
    for (int pos = 0; pos < lista_cantidad(lista); pos++) {
        tmpElem = lista_elemento(lista, pos);
        fprintf(archivo, "  %d   %s\n", tmpElem->a, tmpElem->b);
    }
    free(tmpElem);
}


/** \brief imprimirArchivo(FILE *archivo).
 *
 * Imprimi el contenido del archivo
 */

static void imprimirArchivo(FILE *archivo){
    int contador = 1;
    char *linea = (char *)malloc(sizeof(char));
    while(fgets(linea,1000,archivo) != NULL && contador < 1000){
        printf("%s",linea);
        contador++;
    }
    free(linea);
}

/** \brief cargarListaConArchivosyPalabras(FILE *archivo,char *nombre,multiset_t *tmpArbol).
 *
 * Carga una lista con archivos y su contenido
 */

static void cargarListaConArchivosyPalabras(FILE *archivo,char *nombre,multiset_t *tmpArbol){
     fprintf(archivo,"  %s\n", nombre);
     lista_t *tmpLista = lista_crear();
     *tmpLista = multiset_elementos(tmpArbol,NULL);
     lista_ordenar(tmpLista,comparar_Elementos);
     copiar_lista_a_archivo(tmpLista,archivo);
     free(tmpLista);
}

static void cargarDirectorio(){
    char cadena[] = "\\Carpeta_de_trabajo\\";
    //char cadena[] = " ";
    struct dirent *entrada;
    DIR *directorio;
    char ruta[256];
    char *rutaAux = (char *)malloc(sizeof(char));
    int cantidadCaracteresRutaDefault = 1;

    if(getcwd(ruta,sizeof(ruta)) != NULL){
        cantidadCaracteresRutaDefault += strlen(ruta);
        strcat(ruta,cadena);

        directorio = opendir(ruta);
        printf("El directorio es %s",ruta);
            salto();
        printf("y su contenido es >>> ");
            salto();
        if(directorio != NULL){
            FILE *archivo;
            entrada = readdir(directorio);
            multiset_t *arbol = multiset_crear();
            strcpy(rutaAux,ruta);

            while(entrada != NULL){
                // Verifica que el archivo a procesar no es invalido
                if ((strcmp(entrada->d_name, ".") != 0) && (strcmp(entrada->d_name, "..") != 0)){
                    printf("%s\n",entrada->d_name);
                    //Apertura de archivos en modo lectura y cargo las palabras en el arbol
                    if (strlen(entrada->d_name) >= 4 && strcmp(entrada->d_name + strlen(entrada->d_name) - 4, ".txt") == 0) {
                         //Caso que solo este dentro del directorio proyectoC
                        if(cantidadCaracteresRutaDefault == strlen(ruta)){
                            archivo = abrirArchivo(entrada->d_name,"r");
                                cargarArbolConPalabras(archivo,arbol);
                            fclose(archivo);
                            archivo = abrirArchivo("cadauno.out","a+");
                                cargarListaConArchivosyPalabras(archivo,entrada->d_name,arbol);
                            fclose(archivo);
                        }else{
                            //Caso que solo este dentro de un subdirectorio del directorio proyectoC
                            archivo = abrirArchivo(strcat(rutaAux,entrada->d_name),"r");
                                cargarArbolConPalabras(archivo,arbol);
                            fclose(archivo);
                            archivo = abrirArchivo("cadauno.out","a+");
                                cargarListaConArchivosyPalabras(archivo,entrada->d_name,arbol);
                            fclose(archivo);
                            strcpy(rutaAux,ruta);
                        }
                    }
                }
                 entrada = readdir(directorio);
            }
                    salto();
            printf("Procesando texto");
                    salto();
             lista_t *lista = lista_crear();
             *lista = multiset_elementos(arbol,comparar_Elementos);
            lista_ordenar(lista,comparar_Elementos);

            archivo = abrirArchivo("cadauno.out","r");
            imprimirArchivo(archivo);
                fclose(archivo);
                salto();
            archivo = abrirArchivo("totales.out","w+");
            copiar_lista_a_archivo(lista,archivo);
                rewind(archivo);
            imprimirArchivo(archivo);
            fclose(archivo);
            closedir(directorio);

            free(lista);
            free(arbol);
            free(archivo);
        }else{
            printf("Error --> error en ruta.");
            exit(DIRECTORIO_ACCESO_INVALIDO);
        }
    }else{
        exit(DIRECTORIO_ACCESO_INVALIDO);
    }
    free(entrada);
    free(directorio);
}

int main(int argc, char* argv[]){

        salto();
    printf("Contenido de los argumentos >> ");
        salto();

    for (int i=0; i<argc; i++){
        printf("Argumento %d: %s\n", i, argv[i]);
    }
        salto();

    if(argc <= 1 || strcmp(argv[1], "-h") == 0){
            ayudaCuentaPalabras();
    }else{
        if(strcmp(argv[1], "-3") == 0){
             cargarDirectorio();
        }

    }
    salto();
    return EXITO;
}
