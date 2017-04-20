/* 
 * File:   tree.h
 * Author: kiko
 *
 * Created on 25 de marzo de 2015, 3:23
 */

#include "eleab.h"

#ifndef TREE_H
#define	TREE_H


typedef struct _Arbol Arbol;


Arbol* iniAB ();

BOOL isEmptyAB (const Arbol *pa);

void destroyAB (Arbol *pa);

STATUS insertAB (Arbol *pa, const EleAB *po); 

EleAB * lookAB (const Arbol *pa, const EleAB *po);
 
STATUS preOrder (FILE *f, const Arbol *pa);

STATUS inOrder (FILE *f, const Arbol *pa);

STATUS postOrder (FILE *f, const Arbol *pa);

int abDepth (const Arbol *pa);

int numNodes (const Arbol *pa);

STATUS abMirror (Arbol *pa);

void printfDepthLookNodeAB(FILE *f, Arbol * pa);

void printfDepthLookNodeABmirror(FILE *f, Arbol * pa);

char* lookABDic(Arbol *pa, int *num);

char* lookABDicChar(Arbol *pa, int *num);

#endif	/* TREE_H */

