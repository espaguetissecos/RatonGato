#ifndef ELEMENT_H
#define	ELEMENT_H

#include <stdio.h>
#include "types.h"

typedef struct _Element Element;

/* Inicializa un elemento reservando memoria, devolviendo el elemento si lo ha hecho correctamente o NULL si no. */
Element* iniElement();

/* Establece el valor del campo info */
Element* setElementInfo(Element* e, void* p);

/* Devuelve el valor del campo info */
void* getElementInfo(Element* e);

/* Copia el elemento fuente src en el destino dst. Los elementos de entrada son el elemento a copiar y el elemento donde va a ser copiado. Devuelve un puntero al elemento copiado (= la dirección del mismo) o NULL en caso de error. */
Element* copyElement (Element* dst, const Element* src);

/* Compara dos elementos, devolviendo un número negativo si ele1 < ele2,  un número positivo si ele1>ele2 o cero si ele1==ele2 */
int cmpElement (const Element* ele1, const Element* ele2);

/* Imprime en un fichero ya abierto el elemento y devolverá el número de caracteres escritos. */
int printElement (FILE* pf, const Element* ele);

/* Elimina un elemento. */
void freeElement (Element* ele);

#endif
