
#define MAXQUEUE 5

#include "queue.h"

struct _Queue {
    Element** front;
    Element** rear;
    Element* datos[MAXQUEUE];
};

/**------------------------------------------------------------------
Nombre: iniQueue
Descripcion: Inicializa la cola
Entrada: la cola a inicializar
Salida: referencia a la cola inicializada o NULL en caso de error
------------------------------------------------------------------*/
Queue* iniQueue() {

    Queue *pq = NULL;
    pq = (Queue*) malloc(sizeof (Queue));
    if (pq == NULL) {
        return NULL;
    }
    pq->rear = pq->front= &pq->datos[0];
    return pq;

}

/**------------------------------------------------------------------
Nombre: isEmptyQueue
Descripcion: Comprueba si la cola está vacía
Entrada: la cola
Salida: TRUE si la cola está vacía y FALSE en caso contrario
------------------------------------------------------------------*/
BOOL isEmptyQueue(const Queue *q) {

    if (q->rear == q->front) 
        return TRUE;
    
    return FALSE;

}

/**------------------------------------------------------------------
Nombre: isEmptyQueue
Descripcion: Comprueba si la cola está llena
Entrada: la cola
Salida: TRUE si está llena o FALSE si no lo está.
__------------------------------------------------------------------*/
BOOL isFullQueue(const Queue* q) {

    Element** aux = NULL;

    if (q->rear == q->datos + MAXQUEUE -1)
        aux = (Element**)q->datos;
    else
        aux = q->rear + 1;

    if (aux == q->front) 
        return TRUE; 
    
    return FALSE;

}

/**------------------------------------------------------------------
Nombre: insertQueue
Descripcion: inserta un elemento en la cola
Entrada: un elemento y la cola donde insertarlo
Salida: referencia a la cola o NULL en caso de error
------------------------------------------------------------------*/
Queue* insertQueue(Queue *q, const Element *pElem) {

    if (isFullQueue(q) == TRUE)
           return NULL;
    
    *q->rear = iniElement();
    
    copyElement(*q->rear, pElem);
    
    if (q->rear == q->datos + MAXQUEUE - 1)
        q->rear = q->datos;
    else
        q->rear++;

    return q;

}

/**------------------------------------------------------------------
Nombre: extractQueue
Descripcion: Extrae un elemento de la cola
Entrada: la cola de donde extraerlo el elemento
Salida: referencia al elemento o NULL en caso de error
------------------------------------------------------------------*/
Element* extractQueue(Queue *q) {

    Element* aux;
    
    if (isEmptyQueue(q) == TRUE)
        return NULL;
    
    aux = *q->front;
    
    if (q->front  == q->datos + MAXQUEUE -1)
        q->front = q->datos;
    else
        q->front ++;
        
    return aux;
}

/**------------------------------------------------------------------
Nombre: sizeOfQueue
Descripcion: Devuelve el número de elementos de la cola
Entrada: la cola
Salida: el número de elementos de la cola
------------------------------------------------------------------*/
int sizeOfQueue(const Queue *q) {

    int size;

    if (!q)
        return -1;

    size = q->rear - q->front;
    
    if(size < 0){
        size = MAXQUEUE - size; 
    }

    return size;
}

/**------------------------------------------------------------------
Nombre: freeQueue
Descripcion: Libera la cola
Entrada: la cola
------------------------------------------------------------------*/
void freeQueue(Queue *q) {
    if (q != NULL) {
        while (isEmptyQueue(q) == FALSE) {
            freeElement(extractQueue(q));
        }
        free(q);
        q = NULL;
    }

}

/**------------------------------------------------------------------
Nombre: printQueue
Descripcion: imprime toda la cola
Entrada: cola a imprimir y fichero donde imprimirla
Salida: número de caracteres escritos
------------------------------------------------------------------*/
int printQueue(FILE *pf, const Queue *q){

    Element** aux = q->front;
    int impreso = 0;
    
    impreso += fprintf(pf, "Hay %d elementos:\n", sizeOfQueue(q));
    
    for(aux = q->front;aux != q->rear;){
        impreso += printElement(pf, *aux);
        if(aux == q->datos + MAXQUEUE - 1)
            aux = (Element**)q->datos;
        else
            aux++;
        
    }

    return impreso;
}