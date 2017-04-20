#include "list.h"

typedef struct _Node {
    Element* data;
    struct _Node *next;
} NODE;

struct _List {
    NODE *node;
};

/* FUNCIONES PÚBLICAS DECLARADAS EN list.h Y DEFINIDAS EN list.c  */

/*------------------------------------------------------------------
 Nombre: iniNode
 Descripcion: Reserva memoria e inicializa un nodo
 Salida: el nodo inicializado
 ------------------------------------------------------------------*/
NODE* iniNode() {
    NODE* n = (NODE*) malloc(sizeof (NODE));
    if (!n)
        return NULL;

    n->data = NULL;
    n->next = NULL;
    return n;
}

/*------------------------------------------------------------------
 Nombre: freeNode
 Descripcion: Libera un nodo de la lista
 Entrada: la lista 
 ------------------------------------------------------------------*/
void freeNode(NODE* node) {
    if (node) {
        freeElement(node->data);
        free(node);
    }

    node = NULL;
}

/*------------------------------------------------------------------
Nombre: iniList
Descripcion: Inicializa una lista
Entrada: la lista a inicializar
Salida: puntero a la lista creada o NULL si ha habido error
------------------------------------------------------------------*/
List* iniList() {
    List *pl = NULL;
    pl = (List *) malloc(sizeof (List));
    if (!pl) return NULL;
    pl->node = NULL;
    return pl;
}

/*------------------------------------------------------------------
Nombre: insertFirstElement
Descripcion: inserta un elemento al principio de la lista
Entrada: un elemento y la lista donde insertarlo
Salida: puntero a la lista modificada o NULL si ha habido error
------------------------------------------------------------------*/
List* insertFirstElement(List* list, Element *elem) {
    NODE *pn = NULL;
    if (!list || !elem) return NULL;
    pn = iniNode();
    if (!pn) {
        return NULL;
    }
    pn->data = iniElement();
    if (!pn->data) {
        freeNode(pn);
        return NULL;
    }
    copyElement(pn->data, elem);

    pn->next = list->node;
    list->node = pn;
    return list;
}

/*------------------------------------------------------------------
Nombre: insertLastElement
Descripcion: inserta un elemento al final de la lista
Entrada: un elemento y la lista donde insertarlo
Salida: puntero a la lista modificada o NULL si ha habido error
------------------------------------------------------------------*/
List* insertLastElement(List* list, Element *elem) {
    NODE *pn = NULL, *qn = NULL;
    if (!list || !elem)
        return NULL;
    pn = iniNode();
    if (!pn) {
        return NULL;
    }
    pn->data = iniElement();
    if (!pn->data) {
        freeNode(pn);
        return NULL;
    }
    copyElement(pn->data, elem);
    if (isEmptyList(list) == TRUE) {
        list->node = pn;
        return list;
    }
    for (qn = list->node; qn->next != NULL; qn = qn->next);
    qn->next = pn;
    return list;
}

/*------------------------------------------------------------------
Nombre: insertLastElement
Descripcion: inserta un elemento en orden en la final
Entrada: un elemento y la lista donde insertarlo
Salida: puntero a la lista modificada o NULL si ha habido error
------------------------------------------------------------------*/
List* insertInOrderElemList(List *list, const Element *pElem) {
    Element* aux_elem;
    NODE* pn, *qn;

    if (isEmptyList(list) == TRUE) {/*Cuando no hay ningun elemento*/
        return insertFirstElement(list, (Element*) pElem); /*Insertamos al principio*/
    } else {
        if (sizeOfList(list) == 1) {/*Cuando hay un elemento*/
            if (cmpElement(pElem, list->node->data) <= 0) {/*Si el elemento es igual o menor que el primero lo insertamos al principio*/
                return insertFirstElement(list, (Element*) pElem);

            } else {
                return insertLastElement(list, (Element*) pElem); /*Si no al final*/
            }
        }
        else {/*Si hay mas de un elemento*/

            if (cmpElement(pElem, list->node->data) <= 0) {/*Si es menor o igual al primero lo insertamos al principio*/
                return insertFirstElement(list, (Element*) pElem);
            }
            else {
                qn = list->node;
                while (qn->next != NULL) {/*Minetras el siguiente sea diferente de NULL*/
                    if (cmpElement(pElem, qn->data) >= 0 && cmpElement(pElem, qn->next->data) >= 0) {/*Accedemos al siguiente si el elemento es mayor/igual 
                                                                    que el actual y mayor/igual que el siguiente*/
                        qn = qn->next;
                    } else {/*Si encontramos un hueco lo insertamos delante del actual y detras del siguiente*/
                        pn = iniNode();
                        if (pn == NULL) {
                            return list;
                        }

                        pn->data = iniElement();
                        copyElement(pn->data, pElem);
                        pn->next = qn->next;
                        qn->next = pn;
                        return list;
                    }
                }

                return insertLastElement(list, (Element*) pElem); /*Si no hay ningun elemento mayor lo insertamos al final*/

            }
        }
    }
}

/*------------------------------------------------------------------
Nombre: extractFirstElement
Descripcion: Extrae primer elemento de la lista
Entrada: la lista de donde extraerlo
Salida: puntero a la lista creada o NULL si ha habido error
------------------------------------------------------------------*/
Element* extractFirstElement(List* list) {
    NODE *pn = NULL;
    Element *pe = NULL;
    if (!list || isEmptyList(list) == TRUE) {
        return NULL;
    }
    pn = list->node;
    pe = iniElement();
    pe = copyElement(pe, pn->data);
    if (!pe) {
        return NULL;
    }
    list->node = pn->next;
    freeNode(pn);

    return pe;
}

/*------------------------------------------------------------------
Nombre: extractLastElement
Descripcion: Extrae el último elemento de la lista
Entrada: la lista de donde extraerlo
Salida: puntero a la lista creada o NULL si ha habido error
------------------------------------------------------------------*/
Element* extractLastElement(List* list) {
    NODE *pn = NULL;
    Element *pe = NULL;
    if (!list || isEmptyList(list) == TRUE)
        return NULL;

    if (!list->node->next) {
        pe = iniElement();
        pe = copyElement(pe, list->node->data);
        if (!pe)
            return NULL;
        freeNode(list->node);
        list->node = NULL;
        return pe;
    }

    for (pn = list->node; pn->next->next != NULL; pn = pn->next);
    pe = iniElement();
    pe = copyElement(pe, pn->next->data);
    if (!pe)
        return NULL;
    freeNode(pn->next);
    pn->next = NULL;

    return pe;
}

/*------------------------------------------------------------------
Nombre: isEmptyList
Descripcion: Comprueba si la lista está vacía
Entrada: la lista
Salida: TRUE si la lista está vacía y FALSE en caso contrario
------------------------------------------------------------------*/
BOOL isEmptyList(const List* list) {
    if (!list) return TRUE;
    if (!list->node) return TRUE;

    return FALSE;
}

/*------------------------------------------------------------------
Nombre: sizeOfList
Descripcion: Devuelve el número de elementos de la lista
Entrada: la lista
Salida: el número de elementos de la lista
------------------------------------------------------------------*/
int sizeOfList(const List* list) {
    int i = 1;
    NODE* n;
    if (isEmptyList(list) == TRUE || !list) {
        return 0;
    }

    if (!list->node->next) {
        return 1;
    }

    for (n = list->node; n->next->next != NULL; n = n->next) {

        i++;
    }
    return i + 1;
}

/*------------------------------------------------------------------
Nombre: printList
Descripcion: imprime toda la lista
Entrada: lista a imprimir y fichero donde imprimirla
Salida: número de caracteres escritos
------------------------------------------------------------------*/
int printList(FILE *fd, const List* list) {
    if(fd == NULL || isEmptyList(list) == TRUE){
        return 0;
    }
    
    NODE* n = NULL;
    int num, i, nelem;

    n = list->node;
    
    nelem = sizeOfList(list);

    num = fprintf(fd, "\nHay %d elementos: \n", nelem);

    if (isEmptyList(list) == TRUE || !list) {
        return num;
    }


    if (!list->node->next) {
        num += printElement(fd, list->node->data);
        return num;
    }

    for (i = 0; i < nelem; n = n->next, i++) {

        num += printElement(fd, n->data);
    }

    return num;

}

/*------------------------------------------------------------------
Nombre: freeList
Descripcion: Libera la lista
Entrada: la lista
Salida: puntero a la lista creada o NULL si ha habido error
------------------------------------------------------------------*/
void freeList(List* list) {
    if (!list) return;
    while (isEmptyList(list) == FALSE) {
        freeElement(extractFirstElement(list));
    }
    free(list);
}