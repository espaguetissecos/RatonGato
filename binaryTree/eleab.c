#include "eleab.h"

struct _EleAB {
    int* info;
};


/* Inicializa un elemento reservando memoria, devolviendo el elemento si lo ha hecho correctamente o NULL si no. */
EleAB* iniEleAB(){

    EleAB* e = (EleAB*)malloc(sizeof(EleAB));
    if (!e)
        return e;
    e->info = (int*)malloc(sizeof(int));
    if(!e->info){
        free(e);
        e = NULL;
    }
    
    return e;

}

/* Establece el valor del campo info */
EleAB* setEleABInfo(EleAB* e, void* p){
    if (!p || !e)
        return NULL;
    
    *e->info = *(int*)p;
    
    return e;
}

/* Devuelve el valor del campo info */
void* getEleABInfo(EleAB* e){
    if (!e)
        return NULL;
    
    return (void*)e->info;
}

/* Copia el elemento fuente src en el destino dst. Los elementos de entrada son el elemento a copiar y el elemento donde va a ser copiado. Devuelve un puntero al elemento copiado (= la dirección del mismo) o NULL en caso de error. */
EleAB* copyEleAB (EleAB* dst, const EleAB* src){

    if (!dst || !src)
        return NULL;
    
    *dst->info = *src->info;

    return dst;

}

/* Compara dos elementos, devolviendo un número negativo si ele1 < ele2,  un número positivo si ele1>ele2 o cero si ele1==ele2 */
int cmpEleAB (const EleAB* ele1, const EleAB* ele2){
    if (!ele1 || !ele2)
        return -2;
    
    if (*ele1->info == *ele2->info)
        return 0;
    else if (*ele1->info < *ele2->info)
        return -1;
    else
        return 1;
    
}

/* Imprime en un fichero ya abierto el elemento y devolverá el número de caracteres escritos. */
int printEleAB (FILE* pf, const EleAB* ele){
    
    if (!pf || !ele)
        return -1;
    return fprintf(pf,"%d ", *ele->info);
}

/* Elimina un elemento. */
void freeEleAB (EleAB* ele){
    if(!ele)
        return;
    free(ele->info);
    free(ele);

}
