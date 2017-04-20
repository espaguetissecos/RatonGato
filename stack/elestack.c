
#include "elestack.h"


/**
 * @brief EleStack definition
 *
 */
struct _EleStack {
    float* info;
};

/**------------------------------------------------------------------
 Nombre: iniEleStack
 Descripcion: Inicializa un elemento de pila
 Salida: Puntero al elemento inicializado o NULL en caso de error
 ------------------------------------------------------------------*/
EleStack* iniEleStack(){
    
    EleStack* e= (EleStack*)malloc(sizeof(EleStack));
    
    if(!e)
        return NULL;
    
    e->info = (float*)malloc(sizeof(float));
    
    if (!e->info)
        return NULL;
    
    return e;
    
};

/**------------------------------------------------------------------
 Nombre: setEleStackInfo
 Descripcion: Modifica los datos de un elemento a otro (de info a e).
 Entrada: El elemento a modificar y el elemento de donde se guardan los nuevos datos.
 Salida: El elemento a modificar o NULL si ha habido error.
 ------------------------------------------------------------------*/
EleStack* setEleStackInfo(EleStack* e, void* info){
    
    if (e == NULL || info == NULL)
        return NULL;
    
    e->info = (float*)info;
    
    return e;
}

/**------------------------------------------------------------------
 Nombre: getEleStackInfo
 Descripcion: Devuelve el elemento que contiene Elestack.
 Entrada: El elemento.
 Salida: Devuelve el elemento que contiene Elestack o NULL si ha habido error.
 ------------------------------------------------------------------*/
void* getEleStackInfo(EleStack* e){
    
    if (!e)
        return NULL;
    
    return (void*)e->info;
    
}

/**------------------------------------------------------------------
 Nombre: copyEleStack
 Descripcion: Copia el elemento fuente src en el destino dst.
 Entrada: el elemento a copiar y el elemento donde va a ser copiado.
 Salida: Devuelve un puntero al elemento copiado (= la dirección del mismo) o NULL en caso de error.
 ------------------------------------------------------------------*/
EleStack* copyEleStack(EleStack* dst, const EleStack* src){
    
    if (src == NULL)
        return NULL;
    
    dst = (EleStack*)src;
    
    return dst;
    
}


/**------------------------------------------------------------------
 Nombre: isEqualEleStack
 Descripcion: Compara dos elementos
 Entrada: dos elementos a comparar
 Salida: Devuelve TRUE en caso de ser iguales y si no FALSE
 ------------------------------------------------------------------*/
BOOL isEqualEleStack(const EleStack* ele1, const EleStack* ele2){
    
    if (ele1 == NULL || ele2 == NULL)
        return -1;
    
    if (ele1->info == ele2->info) {
        return TRUE;
    }
    
    return FALSE;
}


/**------------------------------------------------------------------
 Nombre: printEleStack
 Descripcion: Imprime en un fichero ya abierto el elemento
 Entrada: Fichero en el que se imprime y el elemento a imprimir
 Salida: Devuelve el número de caracteres escritos.
 ------------------------------------------------------------------*/
int printEleStack(FILE* pf, const EleStack* ele){
    
    if (pf == NULL || ele == NULL)
        return 0;
    
    return fprintf(pf, "%f", *ele->info);
}

/**------------------------------------------------------------------
 Nombre: freeEleStack
 Descripcion: Elimina un elemento.
 Entrada: Elemento a destruir.
 ------------------------------------------------------------------*/
void freeEleStack(EleStack* ele){
    
    if (!ele)
        return;
    
    free(ele);
    ele = NULL;
    
    return;
    
};

BOOL operateEleStack(EleStack* e1, EleStack* e2, char* operation){
    
    float *f1, *f2, aux;
    int i;
    
    if (e2 == NULL || e1 == NULL || operation == NULL)
        return FALSE;
    
    if ( strcmp(operation, "*") == 0){
        
        f1 = (float*)getEleStackInfo(e2);
        f2 = (float*)getEleStackInfo(e1);
        *f2 = *f2 * *f1;
        
        e1 = setEleStackInfo(e1, f2);
    }
    
    else if ( strcmp(operation, "/") == 0){
        
        f1 = (float*)getEleStackInfo(e2);
        f2 = (float*)getEleStackInfo(e1);
        *f2 = *f1 / *f2;
        
        e1 = setEleStackInfo(e1, f2);
    }
    
    else if ( strcmp(operation, "+") == 0){
        
        f1 = (float*)getEleStackInfo(e2);
        f2 = (float*)getEleStackInfo(e1);
        *f2 = *f2 + *f1;
        
        e1 = setEleStackInfo(e1, f2);
    }
    
    else if ( strcmp(operation, "-") == 0){
        
        f1 = (float*)getEleStackInfo(e2);
        f2 = (float*)getEleStackInfo(e1);
        *f2 = *f1 - *f2;
        
        e1 = setEleStackInfo(e1, f2);
    }
    
    else if ( strcmp(operation, "e") == 0){
        
        f1 = (float*)getEleStackInfo(e2);
        f2 = (float*)getEleStackInfo(e1);
        aux = *f1;
        for (i=0; i<*f2 - 1; i++) {
            *f1 *= aux;
        }
        
        e1 = setEleStackInfo(e1, f1);
    }
    
    return TRUE;
    
}
