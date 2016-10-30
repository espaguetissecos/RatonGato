#include "cabeceras.h"
#include "clientfuncs.h"

int sockfd = 0;
pthread_t thread1, thread2, thread3;
int flag = 0;

/**
 * @page func_accept  func_accept
 *
 * @brief Realiza el accept para el envio de ficheros.
 *
 * @section SYNOPSIS
 * 
 *	 #include  <cabeceras.h>
 *       #include  <clientfuncs.h>
 *
 * 	 void* func_accept ( void* thr_info )
 * 
 * @section descripcion DESCRIPCIÓN
 * 
 * Esta funcion genera y envia al servidor el mensaje de envio de informacion.
 * 
 * Recibe como parametros thr_info, donde se encuentra la informacion para
 * el envio.
 * 
 * @section retorno RETORNO
 * 
 * No retorna nada.
 *
 * @section authors AUTOR
 * Francisco Andreu Sanz (francisco.andreu@estudiante.uam.es)
 * Javier Martínez Hernández (javier.maritnez@estudiante.uam.es)
 *<hr>
 */
void* func_accept(void* thr_info) {

    char buff[1025], *buff2;
    fd_set readfds;
    int max_sd, activity, new_socket, i;
    struct ifreq ifr;

    thread_info* info = (thread_info*) thr_info;

    
    
    int socket_arch = info->sd;
    struct sockaddr_in address = info->address;
    int addrlen = info->addrlen;
    long datalen= info->datalen;
    

    FD_ZERO(&readfds);

    FD_SET(socket_arch, &readfds);
    max_sd = socket_arch;



    //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR)) {
        printf("select error");
    }

    
    if (FD_ISSET(socket_arch, &readfds)) {
        if ((new_socket = accept(socket_arch, (struct sockaddr *) &address, (socklen_t*) & addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }


    }

    shutdown(socket_arch, 2);
    buff2 = (char*) malloc(sizeof (info->data));
    
    strcpy(buff2, info->data);
    i = datalen;
    
    /*Si ocupa mas de 1024 particionamso*/
    while (i > 1024) {
        strncpy(buff, buff2, 1024);
        
        
        send(new_socket, buff, strlen(buff), 0);
        
        *buff2 += strlen(buff);
        
        
        i -= strlen(buff);
    }
    strcpy(buff, buff2);
    send(new_socket, buff, strlen(buff), 0);
    shutdown(new_socket, 1);
    pthread_exit(NULL);

}
  /**
 * @page IRCInterface_SendFile  IRCInterface_SendFile
 *
 * @brief Genera mensajes para enviar al servidor.
 *
 * @section SYNOPSIS
 * 
 *	 #include  <cabeceras.h>
 *       #include  <clientfuncs.h>
 *
 *       boolean IRCInterface_SendFile ( char* filename , char*  nick , char*  data , long unsigned int length) 
 *  
 * @section descripcion DESCRIPCIÓN
 * 
 * Llamada por el botón "Enviar Archivo". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función como todos los demás callbacks bloquea el interface
 * y por tanto es el programador el que debe determinar si crea un nuevo hilo para enviar el archivo o
 * no lo hace.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * @section retorno RETORNO
   * 
 * Devuelve TRUE si se ha establecido la comunicación y FALSE en caso contrario.
 *
 * @section authors AUTOR
 *
 * Francisco Andreu Sanz (francisco.andreu@estudiante.uam.es)
 * Javier Martínez Hernández (javier.maritnez@estudiante.uam.es)
 *<hr>
 */
boolean IRCInterface_SendFile(char *filename, char *nick, char *data, long unsigned int length) {

    int opt = TRUE;
    int new_socket, client_socket[30], max_clients = 30, i, valread, sd;
    int fd;
    struct ifreq ifr;
    char *comando, aux [1024];
    int socket_arch = 0;
    struct sockaddr_in address;

    thread_info* info = NULL;

    info = (thread_info*)malloc(sizeof(thread_info));
    info->datalen = length;
    info->data = (char *) malloc(length + 1);
    strcpy(info->data, data);

    if ((socket_arch = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        exit(EXIT_FAILURE);
    }
    if (setsockopt(socket_arch, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof (opt)) < 0) {
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //ind the socket to localhost port 8888
    if (bind(socket_arch, (struct sockaddr *) &address, sizeof (address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(socket_arch, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof (address);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ - 1);
    ioctl(socket_arch, SIOCGIFADDR, &ifr);    
    
    sprintf(aux, "\001FSEND %s %s %ld %s %ld", filename, nick, length, inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr), length);

    IRCMsg_Privmsg(&comando, NULL, nick, aux);
    send(sockfd, comando, strlen(comando), 0);
    IRCInterface_PlaneRegisterInMessage(comando);

    info->address = address;
    info->addrlen = addrlen;
    info->sd = socket_arch;

    pthread_create(&thread1, NULL, func_accept, (void*) info);

    return TRUE;
}
   /**
 * @page func_ping  func_ping
 *
 * @brief Realiza el envio del mensaje /PING de manera periodica al servidor.
 *
 * @section SYNOPSIS
 * 
 *	 #include  <cabeceras.h>
 *       #include  <clientfuncs.h>
 *
 * 	 void* func_ping ( void* thr_info )
 * 
 * @section descripcion DESCRIPCIÓN
 * 
 * Esta funcion genera y envia al servidor el mensaje /PING de manera periodica mediante
 * el uso de hilos.
 * 
 * Recibe como parametros thr_info, donde se encuentra la informacion para
 * el envio.
 * 
 * @section retorno RETORNO
 * 
 * No retorna nada.
 *
 * @section authors AUTOR
 * 
 * Francisco Andreu Sanz (francisco.andreu@estudiante.uam.es)
 * Javier Martínez Hernández (javier.maritnez@estudiante.uam.es)
 *<hr>
 */
void* func_ping(void* thr_info) {

    thread_info* info = (thread_info*) thr_info;
    int socket = info->sd;
    char* command, *server, *nick, *user, *realname, *password;
    int port, ssl;
    IRCInterface_GetMyUserInfoThread(&nick, &user, &realname, &password, &server, &port, &ssl);
    free(nick);
    free(user);
    free(realname);
    free(info);

    while (TRUE) {
        if (flag == 1)
            IRCInterface_DisconnectServer(server, port);

        IRCMsg_Ping(&command, NULL, server, NULL);
        flag++;
        if (send(sockfd, command, strlen(command), 0) < 0) {
            return NULL;
        }

        IRCInterface_PlaneRegisterOutMessageThread(command);
        sleep(20);
    }
}
  /**
 * @page func_recv  func_recv
 *
 * @brief Hilo de recepcion de mensajes del server.
 *
 * @section SYNOPSIS
 * 
 *	 #include  <cabeceras.h>
 *       #include  <clientfuncs.h>
 *
 * 	 void* func_recv ( void* thr_info )
 * 
 * @section descripcion DESCRIPCIÓN
 * 
 * Esta funcion se encarga de recibir y procesar los mensajes recibidos que son
 * enviados por el servidor.
 * 
 * Recibe como parametros thr_info, donde se encuentra la informacion para
 * el envio.
 * 
 * @section retorno RETORNO
 * 
 * No retorna nada.
 *
 * @section authors AUTOR
 * 
 * Francisco Andreu Sanz (francisco.andreu@estudiante.uam.es)
 * Javier Martínez Hernández (javier.maritnez@estudiante.uam.es)
 *<hr>
 */
void* func_recv(void* thr_info) {

    thread_info* info = (thread_info*) thr_info;
    int socket = info->sd;
    int tid;
    long value;

    char buff[8092] = "";
    tid = pthread_self();
    char* next;
    char* resultado;
    int valread;

    while (TRUE) {
        valread = read(sockfd, buff, 8192);
        if (valread < 0) {
            pthread_exit(NULL);
        }

        buff[valread] = '\0';
        next = IRC_UnPipelineCommands(buff, &resultado, NULL);
        ServerReply(resultado, socket);
        free(resultado);
        while (next != NULL) {
            next = IRC_UnPipelineCommands(NULL, &resultado, next);
            if (resultado != ((void *) 0)) {
                ServerReply(resultado, socket);
            }
            free(resultado);
        }
    }
}
/** 
 * \defgroup IRCInterfaceCallbacks Callbaks del interfaz
 *
 */

/** 
 * \addtogroup IRCInterfaceCallbacks
 * Funciones que van a ser llamadas desde el interface y que deben ser implementadas por el usuario.
 * Todas estas funciones pertenecen al hilo del interfaz.
 *
 * El programador puede, por supuesto, separar todas estas funciones en múltiples ficheros a
 * efectos de desarrollo y modularización.
 *
 * <hr>
 */

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateProtectTopic IRCInterface_ActivateProtectTopic
 *
 * @brief Llamada por el botón de activación de la protección de tópico.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateProtectTopic (char * channel)
 * 
 * @description 
 * Llamada por el botón de activación de la protección de tópico.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateProtectTopic(char * channel) {
    char *msg = NULL, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE +t", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    msg = CrearMensaje(option, resultado);
    if (msg == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_SetProtectTopic();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateProtectTopic IRCInterface_DeactivateProtectTopic
 *
 * @brief Llamada por el botón de desactivación de la protección de tópico.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateProtectTopic (char * channel)
 * 
 * @description 
 * Llamada por el botón de desactivación de la protección de tópico.
 * 
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateProtectTopic(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE -t", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_UnsetProtectTopic();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateExternalMessages IRCInterface_ActivateExternalMessages
 *
 * @brief Llamada por el botón de activación de la recepción de mensajes externos.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateExternalMessages (char * channel)
 * 
 * @description 
 * Llamada por el botón de activación de la recepción de mensajes externos.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateExternalMessages(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE +n", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_SetExternalMessages();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateExternalMessages IRCInterface_DeactivateExternalMessages
 *
 * @brief Llamada por el botón de desactivación de la recepción de mensajes externos.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateExternalMessages (char * channel)
 * 
 * @description 
 * Llamada por el botón de desactivación de la recepción de mensajes externos.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateExternalMessages(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE -n", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_UnsetExternalMessages();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateSecret IRCInterface_ActivateSecret
 *
 * @brief Llamada por el botón de activación de canal secreto.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateSecret (char * channel)
 * 
 * @description 
 * Llamada por el botón de activación de canal secreto.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateSecret(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE +s", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_SetSecret();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateSecret IRCInterface_DeactivateSecret
 *
 * @brief Llamada por el botón de desactivación de canal secreto.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateSecret (char * channel)
 * 
 * @description 
 * Llamada por el botón de desactivación de canal secreto.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateSecret(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE -s", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_UnsetSecret();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateInvite IRCInterface_ActivateInvite
 *
 * @brief Llamada por el botón de activación de canal de sólo invitación.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateInvite (char * channel)
 * 
 * @description 
 * Llamada por el botón de activación de canal de sólo invitación.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateInvite(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE +i", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_SetInvite();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateInvite IRCInterface_DeactivateInvite
 *
 * @brief Llamada por el botón de desactivación de canal de sólo invitación.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateInvite (char * channel)
 * 
 * @description 
 * Llamada por el botón de desactivación de canal de sólo invitación.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateInvite(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE -i", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_UnsetInvite();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivatePrivate IRCInterface_ActivatePrivate
 *
 * @brief Llamada por el botón de activación del modo privado.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivatePrivate (char * channel)
 * 
 * @description 
 * Llamada por el botón de activación del modo privado.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivatePrivate(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE +p", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_SetPrivate();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivatePrivate IRCInterface_DeactivatePrivate
 *
 * @brief Llamada por el botón de desactivación del modo privado.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivatePrivate (char * channel)
 * 
 * @description 
 * Llamada por el botón de desactivación del modo privado.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivatePrivate(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE -p", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_UnsetPrivate();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateModerated IRCInterface_ActivateModerated
 *
 * @brief Llamada por el botón de activación de la moderación del canal.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateModerated (char * channel)
 * 
 * @description 
 * Llamada por el botón de activación de la moderación del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateModerated(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE +m", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_SetModerated();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateModerated IRCInterface_DeactivateModerated
 *
 * @brief Llamada por el botón de desactivación  de la moderación del canal.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateModerated (char * channel)
 * 
 * @description 
 * Llamada por el botón de desactivación  de la moderación del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateModerated(char * channel) {
    char *msg, *resultado;
    long option;

    IRC_UnPipelineCommands("/MODE -m", &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
        //sin return escribimos en registro mejor
    }
    IRCInterface_UnsetModerated();
    IRCInterface_PlaneRegisterOutMessage(msg);
    free(msg);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateChannelKey IRCInterface_ActivateChannelKey
 *
 * @brief Llamada por el botón de activación de la clave del canal.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateChannelKey (char * channel, char * key)
 * 
 * @description 
 * Llamada por el botón de activación de la clave del canal. El segundo parámetro es
 * la clave del canal que se desea poner. Si es NULL deberá impedirse la activación
 * con la función implementada a tal efecto.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateChannelKey(char * channel, char *key) {
    char *msg, *resultado, *comando;
    long option;
    if (channel && key) {
        IRCMsg_Mode(&comando, NULL, channel, "+k", key);

        send(sockfd, comando, strlen(comando), 0);


        IRCInterface_PlaneRegisterOutMessage(comando);

        IRCInterface_SetChannelKey(key);
    }
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateChannelKey IRCInterface_DeactivateChannelKey
 *
 * @brief Llamada por el botón de desactivación de la clave del canal.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateChannelKey (char * channel)
 * 
 * @description 
 * Llamada por el botón de desactivación de la clave del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateChannelKey(char * channel) {
    char *comando;

    if (channel) {
        IRCMsg_Mode(&comando, NULL, channel, "-k", NULL);
        send(sockfd, comando, strlen(comando), 0);

        IRCInterface_PlaneRegisterOutMessage(comando);

        IRCInterface_UnsetChannelKey();

    } else {
        return;
    }
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateNicksLimit IRCInterface_ActivateNicksLimit
 *
 * @brief Llamada por el botón de activación del límite de usuarios en el canal.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateNicksLimit (char * channel, int * limit)
 * 
 * @description 
 * Llamada por el botón de activación del límite de usuarios en el canal. El segundo es el
 * límite de usuarios que se desea poner. Si el valor es 0 se sobrentiende que se desea eliminar
 * este límite.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateNicksLimit(char * channel, int limit) {
    char *comando;

    char aux[10];
    if (channel && limit > 0) {
        sprintf(aux, "%d", limit);
        IRCMsg_Mode(&comando, NULL, channel, "+l", aux);

        send(sockfd, comando, strlen(comando), 0);
        IRCInterface_PlaneRegisterOutMessage(comando);

        IRCInterface_SetNicksLimit(limit);
    } else {
        return;
    }
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateNicksLimit IRCInterface_DeactivateNicksLimit
 *
 * @brief Llamada por el botón de desactivación de la protección de tópico.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateNicksLimit (char * channel)
 * 
 * @description 
 * Llamada por el botón de desactivación  del límite de usuarios en el canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateNicksLimit(char * channel) {
    char *comando;
    if (channel) {

        IRCMsg_Mode(&comando, NULL, channel, "-l", NULL);
        send(sockfd, comando, strlen(comando), 0);

        IRCInterface_PlaneRegisterOutMessage(comando);
        IRCInterface_UnsetNicksLimit();
    } else {
        return;
    }
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_NewTopicEnter IRCInterface_NewTopicEnter
 *
 * @brief Llamada cuando se pulsa la tecla ENTER en el campo de tópico.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_NewTopicEnter (char * topicdata)
 * 
 * @description 
 * Llamada cuando se pulsa la tecla ENTER en el campo de tópico.
 * Deberá intentarse cambiar el tópico del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_NewTopicEnter(char *topicdata) {
    char *comando;
    if (!topicdata)
        return;

    IRCMsg_Topic(&comando, NULL, IRCInterface_ActiveChannelName(), topicdata);
    send(sockfd, comando, strlen(comando), 0);

    IRCInterface_PlaneRegisterOutMessage(comando);
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_NewCommandText IRCInterface_NewCommandText
 *
 * @brief Llamada la tecla ENTER en el campo de texto y comandos.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_NewCommandText (char * command)
 * jo
 * @description 
 * Llamada de la tecla ENTER en el campo de texto y comandos. El texto deberá ser
 * enviado y el comando procesado por las funciones de "parseo" de comandos de
 * usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_NewCommandText(char *command) {
    //Necesito uno para identificar lo que mando
    char *msg, *resultado;
    long option;
    thread_info* info = NULL;

    IRC_UnPipelineCommands(command, &resultado, NULL);
    option = IRCUser_CommandQuery(resultado);
    if ((msg = CrearMensaje(option, resultado)) == NULL)
        return;
    if (strcmp(msg, "QUIT") == 0) {
        IRCInterface_DisconnectServer(NULL, 0);
    } else {
        //Sacar info desde hilo? thread1?
        if (send(sockfd, msg, strlen(msg), 0) < 0) {
            //sin return escribimos en registro mejor
        }
        //IRCInterface_PlaneRegisterInMessageThread(command);
        IRCInterface_PlaneRegisterOutMessage(msg);
        free(msg);
    }
    //Aqui mediante la opcion llamaria a otra funcion para hacer el cambio que sea
    //dependiendo de la respuesta del server

}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_GiveOp IRCInterface_GiveOp
 *
 * @brief Llamada por el botón "Op".
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_GiveOp (char * channel, char *nick)
 * 
 * @description 
 * Llamada por el botón "Op". Previamente debe seleccionarse un nick del
 * canal para darle "op" a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_GiveOp(char *channel, char *nick) {
    char *comando;
    if (channel && nick) {
        IRCMsg_Mode(&comando, NULL, channel, "+o", nick);
        send(sockfd, comando, strlen(comando), 0);
        IRCInterface_PlaneRegisterOutMessage(comando);
    } else {
        return;
    }
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_TakeOp IRCInterface_TakeOp
 *
 * @brief Llamada por el botón "Quitar Op". 
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_TakeOp (char *channel, char *nick)
 * 
 * @description 
 * Llamada por el botón "Quitar Op". Previamente debe seleccionarse un nick del
 * canal para quitarle "op" a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_TakeOp(char *channel, char *nick) {
    char *comando;
    if (channel && nick) {
        IRCMsg_Mode(&comando, NULL, channel, "-o", nick);
        send(sockfd, comando, strlen(comando), 0);
        IRCInterface_PlaneRegisterOutMessage(comando);
    } else {
        return;
    }
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_GiveVoice IRCInterface_GiveVoice
 *
 * @brief Llamada por el botón "Dar voz".
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_GiveVoice (char * channel, char *nick)
 * 
 * @description 
 * Llamada por el botón "Dar voz". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_GiveVoice(char *channel, char *nick) {
    char *comando;
    if (channel && nick) {
        IRCMsg_Mode(&comando, NULL, channel, "+v", nick);
        send(sockfd, comando, strlen(comando), 0);
        IRCInterface_PlaneRegisterOutMessage(comando);
    } else {
        return;
    }
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_TakeVoice IRCInterface_TakeVoice
 *
 * @brief Llamada por el botón "Quitar voz". 
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_TakeVoice (char * channel, char *nick)
 * 
 * @description 
 * Llamada por el botón "Quitar voz". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_TakeVoice(char *channel, char *nick) {
    char *comando;
    if (channel && nick) {
        IRCMsg_Mode(&comando, NULL, channel, "-v", nick);
        send(sockfd, comando, strlen(comando), 0);
        IRCInterface_PlaneRegisterOutMessage(comando);
    } else {
        return;
    }
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_BanNick IRCInterface_BanNick
 *
 * @brief Llamada por el botón "Banear".
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_BanNick (char * channel, char *nick)
 * 
 * @description 
 * Llamada por el botón "Banear". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_BanNick(char *channel, char *nick) {
    char *comando;
    if (channel && nick) {
        IRCMsg_Mode(&comando, NULL, channel, "+b", nick);
        send(sockfd, comando, strlen(comando), 0);
        IRCInterface_PlaneRegisterOutMessage(comando);
    } else {
        return;
    }
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_KickNick IRCInterface_KickNick
 *
 * @brief Llamada por el botón "Echar".
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_KickNick (char * channel, char *nick)
 * 
 * @description 
 * Llamada por el botón "Echar". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * No devuelve ningún valor ni código de error.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_KickNick(char *channel, char *nick) {

    if (!nick || !channel)
        return;
    char *command = NULL;

    IRCMsg_Kick(&command, NULL, channel, nick, NULL);
    IRCInterface_PlaneRegisterOutMessage(command);
    send(sockfd, command, strlen(command), 0);

}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_SendFile IRCInterface_SendFile
 *
 * @brief Llamada por el botón "Enviar Archivo".
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_SendFile (char * filename, char *nick, char *data, long unsigned int length)
 * 
 * @description 
 * Llamada por el botón "Enviar Archivo". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función como todos los demás callbacks bloquea el interface
 * y por tanto es el programador el que debe determinar si crea un nuevo hilo para enviar el archivo o
 * no lo hace.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * Devuelve TRUE si se ha establecido la comunicación y FALSE en caso contrario.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_StartAudioChat IRCInterface_StartAudioChat
 *
 * @brief Llamada por el botón "Iniciar" del diálogo de chat de voz.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_StartAudioChat (char *nick)
 * 
 * @description 
 * Llamada por el botón "Iniciar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función como todos los demás callbacks bloquea el interface
 * y por tanto para mantener la funcionalidad del chat de voz es imprescindible crear un hilo a efectos
 * de comunicación de voz.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * Devuelve TRUE si se ha establecido la comunicación y FALSE en caso contrario.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

boolean IRCInterface_StartAudioChat(char *nick) {

    
    return TRUE;
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_StopAudioChat IRCInterface_StopAudioChat
 *
 * @brief Llamada por el botón "Parar" del diálogo de chat de voz.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_StopAudioChat (char *nick)
 * 
 * @description 
 * Llamada por el botón "Parar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función sólo para la comunicación que puede ser reiniciada. 
 * Evidentemente tiene que actuar sobre el hilo de chat de voz.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * Devuelve TRUE si se ha parado la comunicación y FALSE en caso contrario.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

boolean IRCInterface_StopAudioChat(char *nick) {
    return TRUE;
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ExitAudioChat IRCInterface_ExitAudioChat
 *
 * @brief Llamada por el botón "Cancelar" del diálogo de chat de voz.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ExitAudioChat (char *nick)
 * 
 * @description 
 * Llamada por el botón "Parar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función cierrala comunicación. Evidentemente tiene que
 * actuar sobre el hilo de chat de voz.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * Devuelve TRUE si se ha cerrado la comunicación y FALSE en caso contrario.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

boolean IRCInterface_ExitAudioChat(char *nick) {
    return TRUE;
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DisconnectServer IRCInterface_DisconnectServer
 *
 * @brief Llamada por los distintos botones de desconexión.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DisconnectServer (char * server, int port)
 * 
 * @description 
 * Llamada por los distintos botones de desconexión. Debe cerrar la conexión con el servidor.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * Devuelve TRUE si se ha cerrado la conexión y FALSE en caso contrario.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

boolean IRCInterface_DisconnectServer(char *server, int port) {


    char* command;
    void* res;
    IRCMsg_Quit(&command, NULL, "Leaving");

    if (send(sockfd, command, strlen(command), 0) < 0) {
        return FALSE;
    }
    IRCInterface_PlaneRegisterOutMessage(command);
    pthread_cancel(thread1);

    /*
            pthread_join(thread1, &res);

            if (res == PTHREAD_CANCELED)
                printf("hilo cancelado\n");
            else
                printf("hilo no cancelado\n");
        
     */
    shutdown(sockfd, 2);
    sockfd = 0;

    return TRUE;
}

/**
 * \ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_Connect IRCInterface_Connect
 *
 * @brief Llamada por los distintos botones de conexión.
 *
 * @synopsis
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_Connect (char *nick, char * user, char * realname, char * password, char * server, int port, boolean ssl)
 * 
 * @description 
 * Llamada por los distintos botones de conexión. Si implementará la comunicación completa, incluido
 * el registro del usuario en el servidor.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 *
 * Esta función debe ser implementada por el alumno.
 *
 * @return
 * Devuelve IRC_OK si todo ha sido correcto, IRCERR_NOSSL
 * si el valor de SSL es TRUE y no se puede activar la conexión SSL pero sí una conexión no protegida.
 * Devolverá IRCERR_NOCONNECT en caso de que no se pueda realizar la comunicación.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

long IRCInterface_Connect(char *nick, char *user, char *realname, char *password, char *server, int port, boolean ssl) {
    if (nick == NULL || user == NULL || realname == NULL || server == NULL)
        return -1;

    int portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *servidor;
    char *hostname;
    char* command;
    thread_info* info = NULL;
    char *msgNick, *msgUser, *msgPass;
    char buff[1024];

    hostname = server;
    portno = port;

    /* socket: creamos socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creando el socket");
        return IRCERR_NOCONNECT;
    }
    /* gethostbyname: get the server's DNS entry */
    servidor = gethostbyname(hostname);
    if (servidor == NULL) {
        fprintf(stderr, "ERROR, no existe el servidor %s\n", hostname);
        return IRCERR_NOCONNECT;
    }

    /* construimos la conex. internet del server */
    bzero((char *) &serveraddr, sizeof (serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *) servidor->h_addr,
            (char *) &serveraddr.sin_addr.s_addr, servidor->h_length);
    serveraddr.sin_port = htons(portno);

    /* connect: creamos una comunicacion con el server */
    if (connect(sockfd, (struct sockaddr*) &serveraddr, sizeof (serveraddr)) < 0) {
        perror("ERROR conectando");
        return IRCERR_NOCONNECT;
    }

    IRCMsg_Nick(&msgNick, NULL, nick, NULL);
    IRCMsg_User(&msgUser, NULL, user, server, realname);
    IRCMsg_Pass(&msgPass, NULL, password);
    IRC_PipelineCommands(&command, msgNick, msgUser, msgPass, NULL);

    if (send(sockfd, command, strlen(command), 0) < 0) {
        return IRCERR_NOCONNECT;
    }
    IRCInterface_PlaneRegisterOutMessage(command);
    free(command);

    info = (thread_info*) malloc(sizeof (thread_info));
    info->sd = sockfd;
    pthread_create(&thread2, NULL, func_ping, (void *) info);

    info = (thread_info*) malloc(sizeof (thread_info));
    info->sd = sockfd;
    pthread_create(&thread1, NULL, func_recv, (void *) info);

    return IRC_OK;
}


/***************************************************************************************************/
/***************************************************************************************************/
/**                                                                                               **/
/** MMMMMMMMMM               MMMMM           AAAAAAA           IIIIIII NNNNNNNNNN          NNNNNN **/
/**  MMMMMMMMMM             MMMMM            AAAAAAAA           IIIII   NNNNNNNNNN          NNNN  **/
/**   MMMMM MMMM           MM MM            AAAAA   AA           III     NNNNN NNNN          NN   **/
/**   MMMMM  MMMM         MM  MM            AAAAA   AA           III     NNNNN  NNNN         NN   **/
/**   MMMMM   MMMM       MM   MM           AAAAA     AA          III     NNNNN   NNNN        NN   **/
/**   MMMMM    MMMM     MM    MM           AAAAA     AA          III     NNNNN    NNNN       NN   **/
/**   MMMMM     MMMM   MM     MM          AAAAA       AA         III     NNNNN     NNNN      NN   **/
/**   MMMMM      MMMM MM      MM          AAAAAAAAAAAAAA         III     NNNNN      NNNN     NN   **/
/**   MMMMM       MMMMM       MM         AAAAA         AA        III     NNNNN       NNNN    NN   **/
/**   MMMMM        MMM        MM         AAAAA         AA        III     NNNNN        NNNN   NN   **/
/**   MMMMM                   MM        AAAAA           AA       III     NNNNN         NNNN  NN   **/
/**   MMMMM                   MM        AAAAA           AA       III     NNNNN          NNNN NN   **/
/**  MMMMMMM                 MMMM     AAAAAA            AAAA    IIIII   NNNNNN           NNNNNNN  **/
/** MMMMMMMMM               MMMMMM  AAAAAAAA           AAAAAA  IIIIIII NNNNNNN            NNNNNNN **/
/**                                                                                               **/
/***************************************************************************************************/

/***************************************************************************************************/



int main(int argc, char *argv[]) {
    /* La función IRCInterface_Run debe ser llamada al final      */
    /* del main y es la que activa el interfaz gráfico quedándose */
    /* en esta función hasta que se pulsa alguna salida del       */
    /* interfaz gráfico.                                          */

    IRCInterface_Run(argc, argv);

    return 0;
}

