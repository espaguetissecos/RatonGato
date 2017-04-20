#include "element.h"

struct _Element {
char *definition;
};


/* Inicializa un elemento reservando memoria, devolviendo el elemento si lo ha hecho correctamente o NULL si no. */
Element* iniElement(){

    Element* e = (Element*)malloc(sizeof(Element));
    if (!e)
        return e;
    
    e->definition = NULL;
    
    return e;

}

/* Establece el valor del campo info */
Element* setElementInfo(Element* e, void* p){
    if (!p || !e)
        return NULL;
    
    if(e->definition != NULL)
        free(e->definition);
    
    e->definition = (char*)malloc(sizeof(char)* strlen((char*)p) + 1);
    
    if(e->definition == NULL)
        return e;
    
    strcpy(e->definition, p);
    
    e->definition[strlen(p)] = '\0';
    
    return e;
}

/* Devuelve el valor del campo info */
void* getElementInfo(Element* e){
    if (!e)
        return NULL;
    
    return e->definition;
}

/* Copia el elemento fuente src en el destino dst. Los elementos de entrada son el elemento a copiar y el elemento donde va a ser copiado. Devuelve un puntero al elemento copiado (= la dirección del mismo) o NULL en caso de error. */
Element* copyElement (Element* dst, const Element* src){

    if (!dst || !src)
        return NULL;
    
    setElementInfo(dst, src->definition);

    return dst;

}

/* Compara dos elementos, devolviendo un número negativo si ele1 < ele2,  un número positivo si ele1>ele2 o cero si ele1==ele2 */
int cmpElement (const Element* ele1, const Element* ele2){
    if (!ele1 || !ele2)
        return -2;
    
    if (0 > strcmp(ele1->definition, ele2->definition)){
        return 1;
    }
    
    else if (0 == strcmp(ele1->definition, ele2->definition)){
        return 0;
    }
    
    else{
        return -1;
    }
}

/* Imprime en un fichero ya abierto el elemento y devolverá el número de caracteres escritos. */
int printElement (FILE* pf, const Element* ele){
    
    if (!pf || !ele)
        return -1;
    return fprintf(pf, "%s ", ele->definition);
}

/* Elimina un elemento. */
void freeElement (Element* ele){
    if(!ele)
        return;
    if (ele->definition != NULL)
        free(ele->definition);
    free(ele);

}