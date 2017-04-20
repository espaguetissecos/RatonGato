#ifndef QUEUE_H
#define	QUEUE_H


#include "element.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct _Queue Queue;

/**------------------------------------------------------------------
Nombre: iniQueue
Descripcion: Inicializa la cola
Entrada: la cola a inicializar
Salida: referencia a la cola inicializada o NULL en caso de error
------------------------------------------------------------------*/
Queue* iniQueue();

/**------------------------------------------------------------------
Nombre: isEmptyQueue
Descripcion: Comprueba si la cola está vacía
Entrada: la cola
Salida: TRUE si la cola está vacía y FALSE en caso contrario
------------------------------------------------------------------*/
BOOL isEmptyQueue(const Queue *q);

/**------------------------------------------------------------------
Nombre: isEmptyQueue
Descripcion: Comprueba si la cola está llena
Entrada: la cola
Salida: TRUE si está llena o FALSE si no lo está.
__------------------------------------------------------------------*/
BOOL isFullQueue (const Queue* queue);

/**------------------------------------------------------------------
Nombre: insertQueue
Descripcion: inserta un elemento en la cola
Entrada: un elemento y la cola donde insertarlo
Salida: referencia a la cola o NULL en caso de error
------------------------------------------------------------------*/
Queue* insertQueue(Queue *q, const Element *pElem);

/**------------------------------------------------------------------
Nombre: extractQueue
Descripcion: Extrae un elemento de la cola
Entrada: la cola de donde extraerlo el elemento
Salida: referencia al elemento o NULL en caso de error
------------------------------------------------------------------*/
Element* extractQueue(Queue *q);

/**------------------------------------------------------------------
Nombre: sizeOfQueue
Descripcion: Devuelve el número de elementos de la cola
Entrada: la cola
Salida: el número de elementos de la cola
------------------------------------------------------------------*/
int sizeOfQueue(const Queue *q);

/**------------------------------------------------------------------
Nombre: freeQueue
Descripcion: Libera la cola
Entrada: la cola
------------------------------------------------------------------*/
void freeQueue(Queue *q);

/**------------------------------------------------------------------
Nombre: printQueue
Descripcion: imprime toda la cola
Entrada: cola a imprimir y fichero donde imprimirla
Salida: número de caracteres escritos
------------------------------------------------------------------*/
int printQueue(FILE *pf, const Queue *q);

#endif

