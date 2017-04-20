#ifndef LIST_H
#define	LIST_H

#include "types.h"
#include "element.h"

typedef struct _List List;

/* FUNCIONES PÚBLICAS DECLARADAS EN list.h Y DEFINIDAS EN list.c  */

/*------------------------------------------------------------------
Nombre: iniList
Descripcion: Inicializa una lista
Entrada: la lista a inicializar
Salida: puntero a la lista creada o NULL si ha habido error
------------------------------------------------------------------*/
List* iniList();

/*------------------------------------------------------------------
Nombre: insertFirstElement
Descripcion: inserta un elemento al principio de la lista
Entrada: un elemento y la lista donde insertarlo
Salida: puntero a la lista modificada o NULL si ha habido error
------------------------------------------------------------------*/
List* insertFirstElement (List* list, Element *elem);

/*------------------------------------------------------------------
Nombre: insertLastElement
Descripcion: inserta un elemento al final de la lista
Entrada: un elemento y la lista donde insertarlo
Salida: puntero a la lista modificada o NULL si ha habido error
------------------------------------------------------------------*/
List* insertLastElement  (List* list, Element *elem);

/*------------------------------------------------------------------
Nombre: insertLastElement
Descripcion: inserta un elemento en orden en la final
Entrada: un elemento y la lista donde insertarlo
Salida: puntero a la lista modificada o NULL si ha habido error
------------------------------------------------------------------*/
List* insertInOrderElemList (List *list, const Element *pElem);

/*------------------------------------------------------------------
Nombre: extractFirstElement
Descripcion: Extrae primer elemento de la lista
Entrada: la lista de donde extraerlo
Salida: puntero a la lista creada o NULL si ha habido error
------------------------------------------------------------------*/
Element* extractFirstElement (List* list);

/*------------------------------------------------------------------
Nombre: extractLastElement
Descripcion: Extrae el último elemento de la lista
Entrada: la lista de donde extraerlo
Salida: puntero a la lista creada o NULL si ha habido error
------------------------------------------------------------------*/
Element* extractLastElement (List* list);

/*------------------------------------------------------------------
Nombre: isEmptyList
Descripcion: Comprueba si la lista está vacía
Entrada: la lista
Salida: TRUE si la lista está vacía y FALSE en caso contrario
------------------------------------------------------------------*/
BOOL isEmptyList(const List* list);

/*------------------------------------------------------------------
Nombre: sizeOfList
Descripcion: Devuelve el número de elementos de la lista
Entrada: la lista
Salida: el número de elementos de la lista
------------------------------------------------------------------*/
int sizeOfList(const List* list);

/*------------------------------------------------------------------
Nombre: printList
Descripcion: imprime toda la lista
Entrada: lista a imprimir y fichero donde imprimirla
Salida: número de caracteres escritos
------------------------------------------------------------------*/
int printList(FILE *fd, const List* list);

/*------------------------------------------------------------------
Nombre: freeList
Descripcion: Libera la lista
Entrada: la lista
Salida: puntero a la lista creada o NULL si ha habido error
------------------------------------------------------------------*/
void freeList(List* list);

#endif