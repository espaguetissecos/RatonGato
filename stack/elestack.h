/*
 * File:   elestack.h
 */
#ifndef _ELESTACK_H
#define _ELESTACK_H


/*! Includes the STATUS and BOOL types*/
#include "types.h"


/**
 * @brief EleStack definition
 *
 */
typedef struct _EleStack EleStack;

/**------------------------------------------------------------------
 Nombre: iniEleStack
 Descripcion: Inicializa un elemento de pila
 Salida: Puntero al elemento inicializado o NULL en caso de error
 ------------------------------------------------------------------*/
EleStack* iniEleStack();

/**------------------------------------------------------------------
 Nombre: setEleStackInfo
 Descripcion: Modifica los datos de un elemento a otro (de info a e).
 Entrada: El elemento a modificar y el elemento de donde se guardan los nuevos datos.
 Salida: El elemento a modificar o NULL si ha habido error.
 ------------------------------------------------------------------*/
EleStack* setEleStackInfo(EleStack* e, void* info);

/**------------------------------------------------------------------
 Nombre: getEleStackInfo
 Descripcion: Devuelve el elemento que contiene Elestack.
 Entrada: El elemento.
 Salida: Devuelve el elemento que contiene Elestack o NULL si ha habido error.
 ------------------------------------------------------------------*/
void* getEleStackInfo(EleStack* e);

/**------------------------------------------------------------------
 Nombre: copyEleStack
 Descripcion: Copia el elemento fuente src en el destino dst.
 Entrada: el elemento a copiar y el elemento donde va a ser copiado.
 Salida: Devuelve un puntero al elemento copiado (= la dirección del mismo) o NULL en caso de error.
 ------------------------------------------------------------------*/
EleStack* copyEleStack(EleStack* dst, const EleStack* src);


/**------------------------------------------------------------------
 Nombre: isEqualEleStack
 Descripcion: Compara dos elementos
 Entrada: dos elementos a comparar
 Salida: Devuelve TRUE en caso de ser iguales y si no FALSE
 ------------------------------------------------------------------*/
BOOL isEqualEleStack(const EleStack* ele1, const EleStack* ele2);


/**------------------------------------------------------------------
 Nombre: printEleStack
 Descripcion: Imprime en un fichero ya abierto el elemento
 Entrada: Fichero en el que se imprime y el elemento a imprimir
 Salida: Devuelve el número de caracteres escritos.
 ------------------------------------------------------------------*/
int printEleStack(FILE* pf, const EleStack* ele);

/**------------------------------------------------------------------
 Nombre: freeEleStack
 Descripcion: Elimina un elemento.
 Entrada: Elemento a destruir.
 ------------------------------------------------------------------*/
void freeEleStack(EleStack* ele);

/**------------------------------------------------------------------
 Nombre: operateEleStack
 Descripcion: Elimina un elemento.
 Entrada: Elemento a destruir.
 ------------------------------------------------------------------*/
BOOL operateEleStack(EleStack* e2, EleStack* e1, char* operation);


#endif  /* ELESTACK_H */
