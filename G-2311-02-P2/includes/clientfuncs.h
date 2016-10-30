/*************************************************************
* Proyecto: REDES II
* Fichero: cabeceras.h
* Fecha: 04/02/2016
* Descripcion: Declaracion de las cabeceras y definiciones del proyecto
* Integrantes: Francisco Andreu Sanz, Javier Martínez Hernández
*************************************************************/

#ifndef CLIENTFUNCS_H_
#define CLIENTFUNCS_H_ 

#include "cabeceras.h"

char* CrearMensaje(long option, char * command);

void ServerReply(char* command, int sockfd);

#endif /* CABECERAS_H_ */