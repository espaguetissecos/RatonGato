#include "abdb.h"
#include "list.h"

typedef struct _NodeAB {
    EleAB *info;
    struct _NodeAB *left, *right;
} NodeAB;

struct _Arbol {
    NodeAB *root;
};
/*Inicializa el nodo con un valor.*/
NodeAB * iniNodeAB(EleAB * elem) {

    NodeAB* n = (NodeAB*) malloc(sizeof (NodeAB));
    if (!n)
        return n;

    n->info = iniEleAB();
    n->info = copyEleAB(n->info, elem);
    n->left = NULL;
    n->right = NULL;

    return n;
}
/*Imprime el elemento contenido en el nodo.*/
int printNodeAB(FILE* pf, const NodeAB *pn) {
    if (!pn || !pf)
        return -1;

    else
        return printEleAB(pf, pn->info);
}
/* Recorre el nodo raiz, luego la parte izquierda y luego la derecha. Auxiliar para hacer recursividad.*/
STATUS preOrderNodeAB(FILE *f, const NodeAB* n) {
    if (f == NULL || n == NULL)
        return ERROR;

    int num = 0;
    num += printNodeAB(f, n);
    num += preOrderNodeAB(f, n->left);
    num += preOrderNodeAB(f, n->right);

    return num;

}
/* Recorre la parte izda. luego el nodo raiz y luego la parte derecha. Auxiliar para hacer recursividad. */
int inOrderNodeAB(FILE *f, const NodeAB* n) {
    if (f == NULL || n == NULL)
        return ERROR;

    int num = 0;
    num += inOrderNodeAB(f, n->left);
    num += printNodeAB(f, n);
    num += inOrderNodeAB(f, n->right);
    return num;
}
/* Recorre los elementos superiores al pasado por arg. Auxiliar para hacer recursividad. */
int inOrderSup(FILE *f, const NodeAB* n, EleAB * elem) {
    if (f == NULL || n == NULL)
        return ERROR;
    int num = 0;
    num += inOrderSup(f, n->left, elem);
    if (0 < cmpEleAB(n->info, elem))
        num += printNodeAB(f, n);
    num += inOrderSup(f, n->right, elem);
    return num;
}
/*Recorre la parte izquierda, luego la derecha, y luego el nodo raiz. Auxiliar para hacer recursividad.*/
int postOrderNodeAB(FILE *f, const NodeAB* n) {
    if (f == NULL || n == NULL)
        return ERROR;

    int num = 0;
    
    num += postOrderNodeAB(f, n->left);
    num += postOrderNodeAB(f, n->right);
    num += printNodeAB(f, n);

    return num;
}
/*Devuelve la profundidad del arbol. Auxiliar para hacer recursividad.*/
int depthNodeAB(const NodeAB* n) {
    if (n == NULL)
        return -1;

    int rDepth = 0, lDepth = 0;

    lDepth = depthNodeAB(n->left);
    rDepth = depthNodeAB(n->right);

    if (lDepth > rDepth)
        return (lDepth + 1);
    else return (rDepth + 1);
}
/* Devuelve el numero de nodos del árbol recursivamente. Auxiliar para hacer recursividad. */
int numNodesNodeAB(const NodeAB* n) {
    if (n == NULL)
        return 0;

    return 1 + numNodesNodeAB(n->left) + numNodesNodeAB(n->right);
}
/*Devuelve el arbol espejo a partir de un nodo raiz dado.  Auxiliar para hacer recursividad.*/
STATUS mirrorNodeAB(NodeAB* n) {
    NodeAB * temp;

    if (n == NULL)
        return ERROR;
    else {
        mirrorNodeAB(n->left);
        mirrorNodeAB(n->right);

        temp = n->left;
        n->left = n->right;
        n->right = temp;
        return OK;
    }
}
/*Busca un elemento en el arbol.  Auxiliar para hacer recursividad.*/
EleAB * lookNodeAB(NodeAB* n, EleAB* p) {
    if (n == NULL)
        return NULL;
    int cmp;
    cmp = cmpEleAB(p, n->info);
    if (cmp == 0)
        return n->info;
    else if (cmp < 0)
        return lookNodeAB(n->left, p);
    else
        return lookNodeAB(n->right, p);
}
/*Busca un elemento en el arbol espejo.  Auxiliar para hacer recursividad.*/
EleAB * lookNodeABmirror(NodeAB* n, EleAB* p) {
    if (n == NULL)
        return NULL;
    int cmp;
    cmp = cmpEleAB(p, n->info);
    if (cmp == 0)
        return n->info;
    else if (cmp > 0)
        return lookNodeABmirror(n->left, p);
    else
        return lookNodeABmirror(n->right, p);
}
/*Destruye un arbol recurriendo el nodo.  Auxiliar para hacer recursividad.*/
void destroyNodeAB(NodeAB *pn) {
    if (!pn)
        return;

    if (pn->left) {
        destroyNodeAB(pn->left);
    }

    if (pn->right) {
        destroyNodeAB(pn->right);
    }


    freeEleAB(pn->info);
    free(pn);

    return;
}
/*Inserta un elemento en el arbol donde corresponde.  Auxiliar para hacer recursividad.*/
NodeAB* insertElemNodeAB(NodeAB* n, EleAB* ele) {
    if (ele == NULL)
        return n;

    int cmp;

    if (!n) {
        n = iniNodeAB(ele);
        return n;
    }

    cmp = cmpEleAB(ele, n->info);

    if (cmp < 0) {
        n->left = insertElemNodeAB(n->left, ele);
        return n;
    }
    else if (cmp > 0) {
        n->right = insertElemNodeAB(n->right, ele);
        return n;
    }
    else
        return n;
}
/*Inicializa un arbol vacio.*/
Arbol* iniAB() {

    Arbol* a = (Arbol*) malloc(sizeof (Arbol));

    if (!a)
        return a;

    a->root = NULL;

    return a;
}
/*True si el arbol esta vacio, false en otro caso.*/
BOOL isEmptyAB(const Arbol *pa) {

    if (!pa)
        return -1;

    if (pa->root == NULL)
        return TRUE;

    return FALSE;

}
/*Destruye el arbol llamando a su funcion auxiliar.*/
void destroyAB(Arbol *pa) {

    if (!pa)
        return;

    destroyNodeAB(pa->root);

    free(pa);

    return;

}
/*Inserta un elemento en el arbol llamando a su funcion auxiliar.*/
STATUS insertAB(Arbol *pa, const EleAB *po) {

    if (!pa || !po)
        return ERROR;

    pa->root = insertElemNodeAB(pa->root, (EleAB*) po);

    return OK;

}
/*Busca un elemento en el arbol llamando a su funcion auxiliar.*/
EleAB * lookAB(const Arbol *pa, const EleAB *po) {
    if (!pa || !po || (isEmptyAB(pa) == TRUE))
        return NULL;

    EleAB * node = NULL;

    node = lookNodeAB(pa->root, (EleAB*) po);

    return node;
}
/*Busca un elemento en el arbol espejo llamando a su funcion auxiliar.*/
EleAB * lookABmirror(const Arbol *pa, const EleAB *po) {
    if (!pa || !po || (isEmptyAB(pa) == TRUE))
        return NULL;

    EleAB * node = NULL;

    node = lookNodeABmirror(pa->root, (EleAB*) po);

    return node;
}
/*Recorre el arbol en modo preorden llamando a su funcion auxiliar.*/
int preOrderAB(FILE *f, const Arbol *pa) {
    if (f == NULL || pa == NULL)
        return ERROR;

    return preOrderNodeAB(f, pa->root);
}
/*Recorre el arbol en modo orden medio llamando a su funcion auxiliar.*/
int inOrderAB(FILE *f, const Arbol *pa) {
    if (f == NULL || pa == NULL)
        return ERROR;

    return inOrderNodeAB(f, pa->root);
}
/*Recorre el arbol en modo orden medio superior llamando a su funcion auxiliar.*/
int inOrderSupAB(FILE *f, const Arbol* a, EleAB * elem) {
    if (f == NULL || a == NULL)
        return ERROR;
    
    return inOrderSup(f, a->root, elem);
}
/*Recorre el arbol en modo orden posterior llamando a su funcion auxiliar.*/
int postOrderAB(FILE *f, const Arbol *pa) {
    if (f == NULL || pa == NULL)
        return ERROR;

    return postOrderNodeAB(f, pa->root);
}
/*Devuelve la profundidad del arbol llamando a su funcion auxiliar.*/
int depthAB(const Arbol *pa) {
    if (pa == NULL)
        return -1;
    return depthNodeAB(pa->root);
}
/*Devuelve el numero de nodos de un arbol llamando a su funcion auxiliar.*/
int numNodesAB(const Arbol *pa) {
    if (pa == NULL)
        return -1;
    return numNodesNodeAB(pa->root);
}
/*Devuelve el arbol espejo llamando a su funcion auxiliar.*/
STATUS mirrorAB(Arbol *pa) {
    if (pa == NULL)
        return ERROR;
    return mirrorNodeAB(pa->root);
}
