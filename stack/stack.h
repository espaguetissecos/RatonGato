/* 
 * File:   stack.h
 */
#ifndef _STACK_H
#define _STACK_H
 
/*! Includes the ELESTACK type*/
#include "elestack.h"
 
#define MAXSTACK 10

typedef struct _Stack Stack;


/**------------------------------------------------------------------
Nombre: iniStack
Descripcion: Inicializa la pila
Salida: NULL si ha habido error o la pila si ha ido bien
------------------------------------------------------------------*/
Stack* iniStack ();
 
/**------------------------------------------------------------------
Nombre: isFullStack
Descripcion: Comprueba si la pila esta llena
Entrada: pila a comprobar
Salida: TRUE si está llena o FALSE si no lo esta
------------------------------------------------------------------*/
BOOL isFullStack (const Stack* stc);
 
/**------------------------------------------------------------------
Nombre: isEmptyStack
Descripcion: Comprueba si la pila esta vacia
Entrada: pila a comprobar
Salida: TRUE si está vacia o FALSE si no lo esta
------------------------------------------------------------------*/
BOOL isEmptyStack (const Stack* stc);
 
/**------------------------------------------------------------------
Nombre: push
Descripcion: inserta un elemento en la pila
Entrada: un elemento y la pila donde insertarlo
Salida: NULL si no logra insertarlo o la pila si lo logra
------------------------------------------------------------------*/
Stack* push (Stack *stc, const EleStack* ele);
 
/**------------------------------------------------------------------
Nombre: pop
Descripcion: extrae un elemento en la pila
Entrada: la pila de donde extraerlo
Salida: NULL si no logra extraerlo o el elemento extraido si lo logra
------------------------------------------------------------------*/
EleStack* pop (Stack*stc);
 
/**------------------------------------------------------------------
Nombre: topStack
Descripcion: copia un elemento sin modificar el top
Entrada: la pila de donde copiarlo
Salida: NULL si no logra copiarlo o el elemento si lo logra
------------------------------------------------------------------*/
EleStack* topStack (const Stack* stc);
 
/**------------------------------------------------------------------
Nombre: printStack
Descripcion: imprime toda la pila
Entrada: pila a imprimir y fichero donde imprimirla
------------------------------------------------------------------*/
int printStack (FILE* pf, const Stack* stc);
 
/**------------------------------------------------------------------
Nombre: freeStack
Descripcion: elimina la pila
Entrada: la pila a eliminar
------------------------------------------------------------------*/
void freeStack (Stack* stc);
 
#endif
