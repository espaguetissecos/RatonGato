#include "stack.h"

struct _Stack {
    EleStack **top;
    EleStack* item[MAXSTACK];
};

/**------------------------------------------------------------------
 Nombre: iniStack
 Descripcion: Inicializa la pila
 Salida: NULL si ha habido error o la pila si ha ido bien
 ------------------------------------------------------------------*/
Stack* iniStack (){
    
    Stack* s = (Stack*)malloc(sizeof(Stack));
    
    if (!s)
        return s;
    
    s->top = NULL;
    
    return s;
}

/**------------------------------------------------------------------
 Nombre: isFullStack
 Descripcion: Comprueba si la pila esta llena
 Entrada: pila a comprobar
 Salida: TRUE si está llena o FALSE si no lo esta
 ------------------------------------------------------------------*/
BOOL isFullStack (const Stack* stc){
    
    if (!stc)
        return -1;
    
    if(stc->top == &stc->item[MAXSTACK-1])
        return TRUE;
    
    return FALSE;
}

/**------------------------------------------------------------------
 Nombre: isEmptyStack
 Descripcion: Comprueba si la pila esta vacia
 Entrada: pila a comprobar
 Salida: TRUE si está vacia o FALSE si no lo esta
 ------------------------------------------------------------------*/
BOOL isEmptyStack (const Stack* stc){
    
    if (!stc)
        return -1;
    
    if (stc->top == NULL)
        return TRUE;
    return FALSE;
}

/**------------------------------------------------------------------
 Nombre: push
 Descripcion: inserta un elemento en la pila
 Entrada: un elemento y la pila donde insertarlo
 Salida: NULL si no logra insertarlo o la pila si lo logra
 ------------------------------------------------------------------*/
Stack* push (Stack *stc, const EleStack* ele){
    
    if (!stc || !ele || isFullStack(stc) == TRUE)
        return NULL;
    
    if (stc->top == NULL) {
        stc->top = &stc->item[0];
        *stc->top = copyEleStack(*stc->top, ele);
    }
    
    else{
        stc->top++;
        *stc->top = copyEleStack(*stc->top, ele);
    }
    
    return stc;
    
}

/**------------------------------------------------------------------
 Nombre: pop
 Descripcion: extrae un elemento en la pila
 Entrada: la pila de donde extraerlo
 Salida: NULL si no logra extraerlo o el elemento extraido si lo logra
 ------------------------------------------------------------------*/
EleStack* pop (Stack*stc){
    EleStack *e;
    
    if(!stc)
        return NULL;
    
    if((stc->top - &stc->item[0]) == 0){
        e = *stc->top;
        stc->top = NULL;
    }
    
    else{
        e = *stc->top;
        stc->top--;
    }
    return e;
    
}

/**------------------------------------------------------------------
 Nombre: topStack
 Descripcion: copia un elemento sin modificar el top
 Entrada: la pila de donde copiarlo
 Salida: NULL si no logra copiarlo o el elemento si lo logra
 ------------------------------------------------------------------*/
EleStack* topStack (const Stack* stc){
    
    EleStack *e;
    
    if(!stc)
        return NULL;
    
    return copyEleStack(e, *stc->top);
    
}

/**------------------------------------------------------------------
 Nombre: printStack
 Descripcion: imprime toda la pila
 Entrada: pila a imprimir y fichero donde imprimirla
 ------------------------------------------------------------------*/
int printStack (FILE* pf, const Stack* stc){
    
    int i, num = 0;
    long bucle = (stc->top - &stc->item[0]);
    
    if(bucle < 0)
        bucle = -1;
    if(!pf || !stc)
        return 0;
    
    num += fprintf(pf, "Hay %ld elementos\n", bucle + 1);
    
    for(i=0;i<bucle + 1;i++){
        num += printEleStack(pf, stc->item[i]);
    }
    
    return num;
    
}

/**------------------------------------------------------------------
 Nombre: freeStack
 Descripcion: elimina la pila
 Entrada: la pila a eliminar
 ------------------------------------------------------------------*/
void freeStack (Stack* stc){
    int i;
    if(!stc)
        return;
    
    long bucle = (stc->top - &stc->item[0]);
    
    if(bucle < 0)
        bucle = -1;
    
    for(i=0;i<bucle + 1;i++)
        freeEleStack(stc->item[i]);
    
    free(stc);
    stc = NULL;
    
    return;
}

