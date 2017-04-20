/* 
 * File:   types.h
 */
#ifndef _TIPOS_H
#define _TIPOS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX 20

/* Constantes y tipos de retorno de funciones */
typedef enum {
    FALSE = 0, TRUE = 1
} BOOL;

typedef enum {
    ERROR = -1, OK = 0
} STATUS;

#endif
