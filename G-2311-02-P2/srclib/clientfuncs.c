#include "cabeceras.h"
#include "clientfuncs.h"

pthread_t thread1;
char* gethost;


/**
 * @page func_who  func_who
 *
 * @brief Genera mensaje /WHO para eenviar al servidor de manera reiterada.
 *
 * @section SYNOPSIS
 * 
 *	 #include  <cabeceras.h>
 *       #include  <clientfuncs.h>
 *
 * 	 void* func_who ( void* thr_info )
 * 
 * @section descripcion DESCRIPCIÓN
 * 
 * Esta funcion genera un hilo para realizar de manera reiterada un envio del comando /WHO
 * sobre los canales abiertos hasta que el canal se cierre.
 * 
 * En cualquier caso se necesita la respuesta del servidor.
 *
 * @section retorno RETORNO
 * 
 * No devuelve nada la funcion.
 *
 * @section authors AUTOR
 * Francisco Andreu Sanz (francisco.andreu@estudiante.uam.es)
 * Javier Martínez Hernández (javier.maritnez@estudiante.uam.es)
 *<hr>
 */
void* func_who(void* thr_info) {
    char* comando;
    char* channel = (char*) thr_info;
    extern int sockfd;

    while (TRUE) {
        sleep(20);
        if (IRCInterface_QueryChannelExistThread(channel) == 0) {
            //free(channel);
            pthread_exit(NULL);
        }

        if (IRCMsg_Who(&comando, NULL, channel, NULL) == IRC_OK) {
            IRCInterface_PlaneRegisterOutMessageThread(comando);
        } else {
            return NULL;
        }
        if (send(sockfd, comando, strlen(comando), 0) < 0) {
            return NULL;
        }
        //free(channel);
        //free(comando);
    }
}

/**
 * @page m_free  m_free
 *
 * @brief Libera la memoria de varias variables.
 *
 * @section SYNOPSIS
 * 
 *	 #include  <cabeceras.h>
 *       #include  <clientfuncs.h>
 *
 * 	 void m_free ( int n,  char* var,... )
 * 
 * @section descripcion DESCRIPCIÓN
 * 
 * Esta funcion libera las variables que se pasan por parametro si estas son distintas de
 * NULL.
 * 
 * Recibe como parametros el numero de variables que vamos a liberar y las variables.
 * 
 * @section retorno RETORNO
 * 
 * No devuelve nada la funcion.
 *
 * @section authors AUTOR
 * Francisco Andreu Sanz (francisco.andreu@estudiante.uam.es)
 * Javier Martínez Hernández (javier.maritnez@estudiante.uam.es)
 *<hr>
 */
void mfree(int n, ...) {
    va_list ap;
    char *p = NULL;
    register int i;
    va_start(ap, n);
    for (i = 0; i < n; ++i) {
        p = (char *) va_arg(ap, char *);
        if (p != NULL) free(p);
    }
    va_end(ap);
}

/**
 * @page CrearMensaje  CrearMensaje
 *
 * @brief Genera mensajes para enviar al servidor.
 *
 * @section SYNOPSIS
 * 
 *	 #include  <cabeceras.h>
 *       #include  <clientfuncs.h>
 *
 * 	 char* CrearMensaje ( long option, char*  command  )
 * 
 * @section descripcion DESCRIPCIÓN
 * 
 * Esta funcion genera un mensaje para enviar al servidor, el mensaje depende de
 * la opcion que se pasa mediante el uso de switch para construirlo.
 * 
 * Recibe como parametros option, la opcion para decidir el mensaje que generamos
 * y command, el comando introducido por el usuario desde la ventana de mensaje o 
 * mediante el uso de botones de la interfaz.
 * 
 * @section retorno RETORNO
 * 
 * Retorna el mensaje que deseamos enviar al servidor.
 *
 * @section authors AUTOR
 * Francisco Andreu Sanz (francisco.andreu@estudiante.uam.es)
 * Javier Martínez Hernández (javier.maritnez@estudiante.uam.es)
 *<hr>
 */
char* CrearMensaje(long option, char * command) {
    long salida;
    int maxrepl;
    char *channel = NULL;
    char *password = NULL;
    char *entrada = NULL;
    char *comando = NULL;
    char *msg = NULL;
    char *user = NULL;
    char *nick = NULL;
    char *target = NULL;
    char *realname = NULL;
    char *server = NULL;
    int port, ssl;
    char *targetserver, *nombreComando, **arrayNicks;
    int *numNicks;

    switch (option) {
        case UNAMES:
            printf("Estoy en UNAMES\n");
            salida = IRCUserParse_Names(command, &channel, &targetserver);
            if (salida == IRC_OK) {
                IRCMsg_Names(&comando, NULL, channel, targetserver);
                //return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                //return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                //return NULL;
            } else {
                //COMANDO NO EXISTE
                //return NULL;
            }
            //mfree(2,channel,targetserver);
            return comando;
            break;
        case UHELP:
            printf("Estoy en UHELP\n");
            salida = IRCUserParse_Help(command, &nombreComando);
            if (salida == IRC_OK) {
                //NO TENEMOS FUNCION DE CREACION PARA HELP. PODEMOS USAR GENERALCOMMAND
                //no ecribe en reg plano. Solo sacamos desde aqui por pantalla los comandos que disponemos.
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case ULIST:
            printf("Estoy en ULIST\n");
            char *search;
            salida = IRCUserParse_List(command, &channel, &search);
            if (salida == IRC_OK) {
                //NO TENEMOS FUNCION DE CREACION PARA HELP. PODEMOS USAR GENERALCOMMAND
                IRCMsg_List(&comando, NULL, channel, search);
                //return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                //return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                //return NULL;
            } else {
                //COMANDO NO EXISTE
                //return NULL;
            }
            //mfree(2,channel,search);
            return comando;
            break;
        case UJOIN:
            printf("Estoy en JOIN\n");
            salida = IRCUserParse_Join(command, &channel, &password);
            if (salida == IRC_OK) {
                IRCMsg_Join(&comando, NULL, channel, password, NULL);
                //return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                //return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                //return NULL;
            } else {
                //COMANDO NO EXISTE
                //return NULL;
            }
            //mfree(2,channel,password);
            return comando;
            break;
        case UPART:
            printf("Estoy en UPART\n");
            char * channelActual;
            int valor;
            channelActual = IRCInterface_ActiveChannelName();
            salida = IRCUserParse_Part(command, &msg);
            if (salida == IRC_OK) {
                if (msg != NULL) {
                    valor = IRCInterface_QueryChannelExist(msg);
                    if (valor == 0) {
                        IRCInterface_WriteChannel(channelActual, NULL, " Uso: PART [<canal>], abandona el canal, por omisión el canal actual");
                        return NULL;
                    }
                } else {
                    msg = channelActual;
                }
                IRCMsg_Part(&comando, NULL, msg, "Saliendo");
                //IRCInterface_RemoveChannelThread(channel);
                //return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                //return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                //return NULL;
            } else {
                //COMANDO NO EXISTE
                //return NULL;
            }
            return comando;
            break;
        case ULEAVE:
            printf("Estoy en ULEAVE\n");
            channelActual = IRCInterface_ActiveChannelName();
            salida = IRCUserParse_Leave(command, &msg);
            if (salida == IRC_OK) {
                if (msg != NULL) {
                    valor = IRCInterface_QueryChannelExist(msg);
                    if (valor == 0) {
                        IRCInterface_WriteChannel(channelActual, NULL, " Uso: LEAVE [<canal>], abandona el canal, por omisión el canal actual");
                        return NULL;
                    }
                } else {
                    IRCInterface_WriteChannel(channelActual, NULL, " Uso: LEAVE [<canal>], abandona el canal, por omisión el canal actual");
                    return NULL;
                }
                IRCMsg_Part(&comando, NULL, msg, "Saliendo");
                //return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                //return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                //return NULL;
            } else {
                //COMANDO NO EXISTE
                //return NULL;
            }
            return comando;
            break;
        case UQUIT:
            printf("Estoy en UQUIT\n");
            boolean desc;
            salida = IRCUserParse_Quit(command, &channel);
            if (salida == IRC_OK) {
                IRCMsg_Quit(&comando, NULL, channel);
                IRCInterface_PlaneRegisterOutMessage(comando);
                IRCInterface_DisconnectServer(NULL, 0);
                IRCInterface_RemoveAllChannels();
                IRCInterface_WriteSystem("*","Has sido desconectado del servidor");
                return comando;
//                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UNICK:
            printf("Estoy en UNICK\n");
            char *newnick;
            salida = IRCUserParse_Nick(command, &newnick);
            if (salida == IRC_OK) {
                //IRCInterface_ChangeNick("JAVIer",newnick);
                IRCMsg_Nick(&comando, NULL, newnick, NULL);
                //return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                //return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                //return NULL;
            } else {
                //COMANDO NO EXISTE
                //return NULL;
            }
            //free(newnick);
            return comando;
            break;
        case UAWAY:
            printf("Estoy en UAWAY\n");
            salida = IRCUserParse_Away(command, &msg);
            if (salida == IRC_OK) {
                if (msg == NULL)
                    msg = "Estoy ocupado";
                IRCMsg_Away(&comando, NULL, msg);
                //return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                //return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                //return NULL;
            } else {
                //COMANDO NO EXISTE
                //return NULL;
            }
            //free(msg);
            return comando;
            break;
        case UWHOIS:
            printf("Estoy en UWHOIS\n");
            salida = IRCUserParse_Whois(command, &msg);
            if (salida == IRC_OK) {
                IRCMsg_Whois(&comando, NULL, NULL, msg);
                //return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                //return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                //return NULL;
            } else {
                //COMANDO NO EXISTE
                //return NULL;
            }
            //free(msg);
            return comando;
            break;
        case UINVITE:
            printf("Estoy en UINVITE\n");
            salida = IRCUserParse_Invite(command, &nick, &channel);
            if (salida == IRC_OK) {
                IRCMsg_Invite(&comando, NULL, nick, channel);
                //return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                //return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                //return NULL;
            } else {
                //COMANDO NO EXISTE
                //return NULL;
            }
            //mfree(2,nick,channel);
            return comando;
            break;
        case UKICK:
            printf("Estoy en UKICK\n");
            char *host;
            channel = IRCInterface_ActiveChannelName();
            salida = IRCUserParse_Kick(command, &nick, &msg);
            //IRCInterface_GetNickChannelInfo(nick, channel, &user, &realname, &host);
            if (salida == IRC_OK) {
                IRCMsg_Kick(&comando, NULL, channel, nick, msg);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UTOPIC:
            printf("Estoy en UTOPIC\n");
            char *topic;
            channel = IRCInterface_ActiveChannelName();
            salida = IRCUserParse_Topic(command, &topic);
            if (salida == IRC_OK) {
                IRCMsg_Topic(&comando, NULL, channel, topic);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UME:
            printf("Estoy en UME\n");
            char * conectadoChan = NULL;
            //IRCInterface_GetNickChannelInfo(nick, channel, &user, &realname, &host);
            conectadoChan = IRCInterface_ActiveChannelName();
            salida = IRCUserParse_Me(command, &msg);
            if (salida == IRC_OK) {
                if (strcmp(conectadoChan, "System") != 0) {
                    IRCMsg_Privmsg(&comando, NULL, conectadoChan, msg);
                    return comando;
                } else {
                    IRCInterface_WriteSystem(NULL, "No ha entrado en ningún canal. Intente /join #<canal>");
                }

            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UMSG:
            printf("Estoy en UMSG\n");
            salida = IRCUserParse_Msg(command, &target, &msg);
            if (salida == IRC_OK) {
                IRCMsg_Privmsg(&comando, NULL, target, msg);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UQUERY:
            printf("Estoy en UQUERY\n");
            salida = IRCUserParse_Query(command, &nick, &msg);
            if (salida == IRC_OK) {
                //Es interna, solo abre una ventana nueva a ese usuario.
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UNOTICE:
            printf("Estoy en UNOTICE\n");
            salida = IRCUserParse_Notice(command, &target, &msg);
            if (salida == IRC_OK) {
                IRCMsg_Notice(&comando, NULL, target, msg);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UNOTIFY:
            printf("Estoy en UMOTIFY\n");
            char **nickArray;
            //falta lo del target?? esta bien??
            salida = IRCUserParse_Notify(command, &nickArray, numNicks);
            if (salida == IRC_OK) {
                //sin mensaje
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UIGNORE:
            printf("Estoy en UIGNORE\n");
            char **query;
            salida = IRCUserParse_Ignore(command, &query, numNicks);
            if (salida == IRC_OK) {
                //nose q es target
                IRCMsg_Stats(&comando, NULL, *query, target);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UPING:
            printf("Estoy en UPING\n");
            char *server2;
            salida = IRCUserParse_Ping(command, &user);
            if (salida == IRC_OK) {
                //Como saco el valor del server?
                IRCMsg_Ping(&comando, NULL, server, NULL);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UWHO:
            printf("Estoy en UWHO\n");
            char *mask;
            //falta lo del target?? esta bien??
            salida = IRCUserParse_Who(command, &mask);
            if (salida == IRC_OK) {
                IRCMsg_Who(&comando, NULL, mask, NULL);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UWHOWAS:
            printf("Estoy en UWHOWAS\n");

            salida = IRCUserParse_WhoWas(command, &nick, &maxrepl);
            if (salida == IRC_OK) {
                IRCMsg_Whowas(&comando, NULL, nick, 0, NULL);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UISON:
            printf("Estoy en UISON\n");
            salida = IRCUserParse_Ison(command, &nickArray, numNicks);
            if (salida == IRC_OK) {
                //general mensaje con todos los nicks, como añado todos?
                IRCMsg_Ison(&comando, NULL, "TODOS LOS NICKS", NULL);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UCYCLE:
            printf("Estoy en UCYCLE\n");
            char **channelArray;
            int *numChannels;
            salida = IRCUserParse_Cycle(command, &channelArray, numChannels);
            if (salida == IRC_OK) {
                //No tenemos mensaje, que hara?
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UMOTD:
            printf("Estoy en UMOTD\n");
            salida = IRCUserParse_Motd(command, &server);
            if (salida == IRC_OK) {
                //general mensaje con todos los nicks, como añado todos?
                IRCMsg_Motd(&comando, NULL, server);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case URULES:
            printf("Estoy en URULES\n");
            char *comn;
            salida = IRCUserParse_Rules(command, &comn);
            if (salida == IRC_OK) {
                //sin mensaje para rules
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case ULUSERS:
            printf("Estoy en ULUSERS\n");
            salida = IRCUserParse_Motd(command, &server);
            if (salida == IRC_OK) {
                //general mensaje con todos los nicks, como añado todos?
                IRCMsg_Lusers(&comando, NULL, NULL, server);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UVERSION:
            printf("Estoy en UVERSION\n");
            salida = IRCUserParse_Version(command, &server);
            if (salida == IRC_OK) {
                //general mensaje con todos los nicks, como añado todos?
                IRCMsg_Version(&comando, NULL, server);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UADMIN:
            printf("Estoy en UADMIN\n");
            salida = IRCUserParse_Admin(command, &server);
            if (salida == IRC_OK) {
                //general mensaje con todos los nicks, como añado todos?
                IRCMsg_Admin(&comando, NULL, server);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UUSERHOST:
            printf("Estoy en UUSERHOST\n");
            salida = IRCUserParse_Userhost(command, &nick);
            if (salida == IRC_OK) {
                //faltarian parametros pero el xchat no los pasa
                //IRCMsg_Userhost(&comando, NULL, nick);
                //return comando;
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UKNOCK:
            printf("Estoy en UKNOCK\n");
            char *channel;
            salida = IRCUserParse_Knock(command, &channel, &msg);
            if (salida == IRC_OK) {
                //No esta esta orden
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UVHOST:
            printf("Estoy en UVHOST\n");
            char *login, *pass;
            salida = IRCUserParse_Vhost(command, &login, &pass);
            if (salida == IRC_OK) {
                //No tenemos el comando en xchat
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UMODE:
            printf("Estoy en UMODE\n");
            char *mode, *filter;
            salida = IRCUserParse_Mode(command, &mode, &filter);
            channel = IRCInterface_ActiveChannelName();
            if (salida == IRC_OK) {
                //general mensaje con todos los nicks, como añado todos?
                //IRCInterface_GetNickChannelInfo(nick, channel, &user, &realname, &host);
                IRCMsg_Mode(&comando, NULL, channel, mode, NULL);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UTIME:
            printf("Estoy en UTIME\n");
            salida = IRCUserParse_Time(command, &server);
            if (salida == IRC_OK) {
                //general mensaje con todos los nicks, como añado todos?
                //IRCInterface_GetNickChannelInfo(nick, channel, &user, &realname, &host);
                IRCMsg_Time(&comando, NULL, server);
                return comando;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UBOTMOTD:
            printf("Estoy en UBOTMOTD\n");
            salida = IRCUserParse_BotMotd(command, &server);
            if (salida == IRC_OK) {
                //sin mensaje
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UIDENTIFY:
            printf("Estoy en UIDENTIFY\n");
            //No tenemos nada
            break;
        case UDNS:
            printf("Estoy en UDNS\n");
            //No tenemos mas que el parse
            break;
        case UUSERIP:
            printf("Estoy en UUSERIP\n");
            salida = IRCUserParse_UserIp(command, &nick);
            if (salida == IRC_OK) {
                //Comando desconocido en xchat
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case USTATS:
            printf("Estoy en USTATS\n");
            //No implementamos
            break;
        case UCTCP:
            printf("Estoy en UCTCP\n");
            salida = IRCUserParse_CTCP(command, &msg);
            if (salida == IRC_OK) {
                //sin mesaje
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UDCC:
            printf("Estoy en UDCC\n");
            salida = IRCUserParse_DCC(command, &msg);
            if (salida == IRC_OK) {
                //sin mesaje
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case UCHAT:
            printf("Estoy en UCHAT\n");
            salida = IRCUserParse_Chat(command, &msg);
            if (salida == IRC_OK) {
                //sin mesaje
                channel = IRCInterface_ActiveChannelName();
                if (channel != NULL) {
                    IRCMsg_Privmsg(&comando, NULL, channel, msg);
                    IRCInterface_GetMyUserInfo(&nick, &user, &realname, &password, &server, &port, &ssl);
                    IRCInterface_WriteChannel(channel, nick, command);
                    //IRCInterface_WriteSystem(NULL, msg);
                    return comando;
                }
                return NULL;
            } else if (salida == IRCERR_NOSTRING) {
                //CADENA NULA
                return NULL;
            } else if (salida == IRCERR_NOUSERCOMMAND) {
                //NO EMPIEZA CON /
                return NULL;
            } else {
                //COMANDO NO EXISTE
                return NULL;
            }
            break;
        case IRCERR_NOUSERCOMMAND:
            //sin mesaje
            channel = IRCInterface_ActiveChannelName();
            if (channel != NULL) {
                IRCMsg_Privmsg(&comando, NULL, channel, command);
                //Como obtener mi nick?
                IRCInterface_GetMyUserInfo(&nick, &user, &realname, &password, &server, &port, &ssl);
                IRCInterface_WriteChannel(channel, nick, command);
                //IRCInterface_WriteSystem(NULL, msg);
                return comando;
            }
            break;
        default:
            //sin mesaje
            channel = IRCInterface_ActiveChannelName();
            if (channel != NULL) {
                IRCMsg_Privmsg(&comando, NULL, channel, command);
                //Como obtener mi nick?
                IRCInterface_GetMyUserInfo(&nick, &user, &realname, &password, &server, &port, &ssl);
                IRCInterface_WriteChannel(channel, nick, command);
                //IRCInterface_WriteSystem(NULL, msg);
                return comando;
            }
            break;

    }

}
/**
 * @page ServerReply  ServerReply
 *
 * @brief Parsea la respuesta del servidor y realiza la accion necesaria.
 *
 * @section SYNOPSIS
 * 
 *	 #include  <cabeceras.h>
 *       #include  <clientfuncs.h>
 *
 * 	 void ServerReply ( char* command, int  sockfd  )
 * 
 * @section descripcion DESCRIPCIÓN
 * 
 * Esta funcion genera parsea la respuesta del servidor y realiza la accion necesaria
 * dependiendo de la respuesta de este en la interfaz.
 * 
 * Recibe como parametros command, la respuesta del servidor que debemos interpretar y
 * sockfd, el socket al cual debemos enviar un mensaje si es necesario.
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
void ServerReply(char* command, int sockfd) {

    long option = NULL;
    char *channel = NULL;
    char *nick = NULL;
    char *prefix = NULL;
    char *key = NULL;
    char* msg = NULL;
    char* comando = NULL;
    char* mode = NULL;
    long modelong = 0;
    char* timedate = NULL;
    char* servername = NULL;
    char* versionname = NULL;
    int nusers = 0;
    int ninvisibles = NULL;
    int nservers = NULL;
    int nclientes = NULL;
    char* msgtarget = NULL;
    char* host = NULL;
    char *server = NULL;
    char *nick2 = NULL;
    char *type = NULL;
    char *user = NULL;
    int hopcount = NULL;
    char *realname = NULL;
    char* server2 = NULL;
    char *name = NULL;
    char *nickt=NULL,*usert=NULL,*hostt=NULL,*servert=NULL;
    char * password;
    int port,ssl;
    char* username=NULL;
    char buff[200];

    option = IRC_CommandQuery(command);


    switch (option) {

        case JOIN:
        {
            long mode = 1L;
            char cambio[120]="";
            char buff[100]="";
            nickstate ns=0;
            IRCInterface_PlaneRegisterInMessageThread(command);
            if (IRCParse_Join(command, &prefix, &channel, &key, &msg) == IRC_OK) {
                //if(key==NULL){
                if(!prefix)
                    break;
                
                IRCParse_ComplexUser(prefix,&nickt,&usert,&hostt,&servert);
                
                IRCInterface_GetMyUserInfoThread(&nick, &user, &realname, &password, &server, &port, &ssl);
                
                if(strcmp(nickt,nick)!=0){
                    sprintf(cambio, "%s (~%s@%s) ha entrado %s", nickt,nickt,servert,msg);
                }else{
                    sprintf(cambio, "Has entrado el canal %s", msg);
                }
                if(IRCInterface_QueryChannelExistThread(msg)!=0){
                    IRCInterface_AddNickChannel(msg,nick,user,realname,server,ns);
                    //IRCInterface_AddNewChannelThread(msg, mode);
                }else{
                    IRCInterface_AddNewChannelThread(msg, mode);
                    strcpy(buff, msg);
                    pthread_create(&thread1, NULL, func_who, (void *) buff);
                }
                IRCInterface_WriteChannelThread(msg, NULL, cambio);
            //}else{
                //  IRCInterface_AddNewChannel(channel,IRCMODE_CHANNELPASSWORD);
                //}
                //if (IRCMsg_Mode(&comando, NULL, msg, NULL, NULL) == IRC_OK) {
                  //  IRCInterface_PlaneRegisterOutMessage(comando);
                    //if (send(sockfd, comando, strlen(comando), 0) >= 0) {
                        if (IRCMsg_Who(&comando, NULL, msg, NULL) == IRC_OK) {
                            IRCInterface_PlaneRegisterOutMessageThread(comando);
                            send(sockfd, comando, strlen(comando), 0);
                        }
                    //}
               // }
            }
            /*Enviamos MODE canal*/
            mfree(3,msg,key,channel);
            mfree(9,nickt,usert,hostt,servert,nick,user,realname,password,server);
            break;
        }
        case NICK:
        {
            char cambio[100]="";
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_Nick(command, &prefix, &nick, &msg);
            if(!prefix)
                    break;
                
            IRCParse_ComplexUser(prefix,&nickt,&usert,&hostt,&servert);
                
            sprintf(cambio, "Usted es ahora conocido como %s", msg);
            IRCInterface_WriteSystemThread(NULL, cambio);
            IRCInterface_ChangeNick(nickt, msg);
            mfree(6,nick,msg,nickt,usert,hostt,servert);
            break;
        }


        case PING:
        {
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_Ping(command, &prefix, &server, &host, &msg);
            IRCMsg_Pong(&comando, prefix, server, host, msg);
            mfree(3,msg,host,prefix);
            break;
        }

        case WHO:
        {
            IRCInterface_PlaneRegisterInMessageThread(command);
            if (IRCParse_RplWhoReply(command, &prefix, &nick, &channel, &user, &host, &server, &nick2, &type, &msg, &hopcount, &realname) == IRC_OK) {
                IRCInterface_AddNickChannel(channel, nick2, user, realname, host, NONE);
            }
            mfree(8,nick,channel,user,host,server,nick2,type,msg,realname);
            break;
        }
        case PRIVMSG:
        {
            char *filename = NULL, *hostname_destino = NULL;
            char* nick2=NULL; 
            unsigned long length, port;

            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_Privmsg(command, &prefix, &msgtarget, &msg);

            if (msg[0] == 1) {
                if (sscanf(msg, "\001FSEND %ms %ms %li %ms", &filename, &nick2, &length, &hostname_destino) > 0) {
                    struct sockaddr_in Direccion;
                    int sockval, conectar;
                    struct hostent *host;
                    char buffer[1024]="";
                    struct ifreq ifr;
                    FILE *salida;


                    if(IRCInterface_RecibirDialogThread(nick2, filename) == TRUE) {
        
                        if ((sockval = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                            syslog(LOG_ERR, "Error creating soc");
                            exit(EXIT_FAILURE);
                        }




                        host = gethostbyname(hostname_destino);
                        Direccion.sin_family = AF_INET;
                        Direccion.sin_addr.s_addr = ((struct in_addr*) (host->h_addr))->s_addr;
                        Direccion.sin_port = htons(PORT);
                        conectar = connect(sockval, (struct sockaddr *) &Direccion, sizeof (Direccion));
                        salida = fopen(filename, "w");
                        do {
                            recv(sockval, buffer, 8192, 0);
                            fprintf(salida, "%s", buffer);
                            length -= strlen(buffer);
                        } while (length > 0);


                        fclose(salida);


                        //IRCInterface_ErrorDialog("Listo Recibir");
                        fflush(stdout);
                        close(sockval);
                        fflush(stdout);
                    }

                }
            } else {

                IRCParse_ComplexUser(prefix, &nick, &user, &host, &server);
                IRCInterface_WriteChannelThread(msgtarget, nick, msg);
            }
            break;
        }

        case PONG:
        {
            extern int flag;

            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_Pong(command, &prefix, &server, &server2, &msg);
            flag--;
            mfree(3,server,server2,msg);
            break;
        }
        case PART:
        {
            char cambio[100] = "";
            nickstate ns;
            int num_Nicks;
            IRCInterface_PlaneRegisterInMessageThread(command);
            if (IRCParse_Part(command, &prefix, &channel, &msg) == IRC_OK) {
                //IRCInterface_RemoveChannelThread (channel);
                if(!prefix)
                    break;
                
                IRCParse_ComplexUser(prefix,&nickt,&usert,&hostt,&servert);
                
                IRCInterface_GetMyUserInfoThread(&nick, &user, &realname, &password, &server, &port, &ssl);
                
                if(strcmp(nickt,nick)!=0){
                    IRCInterface_DeleteNickChannelThread(channel,nickt);
                    sprintf(cambio, "%s (~%s@%s) ha abandonado %s (%s)", nickt,nickt,servert,channel,msg);
                }else{
                    IRCInterface_DeleteNickChannelThread(channel,nick);
                    IRCInterface_RemoveChannelThread(channel);
                    sprintf(cambio, "Has abandonado el canal %s %s", channel, msg);
                }
                
                IRCInterface_WriteChannelThread(channel, NULL, cambio);
            }
            mfree(2,channel,msg);
            mfree(9,nickt,usert,hostt,servert,nick,user,realname,password,server);
            break;
        }
        case TOPIC:
        {
            char cambio[100] = "";
            char *next;
            IRCInterface_PlaneRegisterInMessageThread(command);
            if (IRCParse_Topic(command, &prefix, &channel, &msg) == IRC_OK) {
                //CUAL ES LA FUNCION PARA CAMBIAR EL TOPIC EN INTERFAZ?
                //solo falta ponerlo para que lo cambie.
                next = strtok(prefix, "!");
                sprintf(cambio, "%s ha cambiado el topic a: %s", next, msg);
                IRCInterface_WriteChannelThread(channel, nick, cambio);
                mfree(3,channel,msg,next);
            }
            break;
        }
        case KICK:
        {
            char cambio[100] = "";
            nickstate ns;
            int num_Nicks;
            char* user1;
            IRCInterface_PlaneRegisterInMessageThread(command);
            if (IRCParse_Kick(command, &prefix, &channel, &user, &msg) == IRC_OK) {
                //IRCInterface_RemoveChannelThread (channel);
                if(!prefix)
                    break;
                
                IRCParse_ComplexUser(prefix,&nickt,&usert,&hostt,&servert);
                
                IRCInterface_GetMyUserInfoThread(&nick, &user1, &realname, &password, &server, &port, &ssl);
                
                if(strcmp(nickt,user)!=0){
                    IRCInterface_DeleteNickChannel(channel,user);
                    sprintf(cambio, "%s ha echado a %s de %s (%s)", nick,user,channel,msg);
                    IRCInterface_WriteChannelThread(channel, NULL, cambio);
                }else{
                    IRCInterface_DeleteNickChannel(channel,user);
                    sprintf(cambio, "Se le ha echado de %s por %s (%s)", channel,nick, msg);
                    IRCInterface_RemoveChannelThread(channel);
                    IRCInterface_WriteSystemThread("*",cambio);
                }
                
            }
            mfree(3,channel,msg,user);
            mfree(9,nickt,usert,hostt,servert,nick,user,realname,password,server);
            break;
        }
        case AWAY:
        {
            break;
        }

        case QUIT:
        {
            break;
        }

        case MODE:
            IRCParse_Mode(command, &prefix, &channel, &mode, &usert);
            IRCParse_ComplexUser(prefix, &nick, &username, &msg, &server);
            if (strstr(mode, "+l")) {

                sprintf(buff, "%s establece limite del canal a %s", nick, usert);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "-l")) {

                sprintf(buff, "%s quita limite del canal", nick);
                IRCInterface_WriteChannelThread(channel, "*", buff);
            } else if (strstr(mode, "+k")) {

                sprintf(buff, "%s establece contraseña del canal como %s", nick, usert);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "-k")) {

                sprintf(buff, "%s quita contraseña del canal", nick);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "+m")) {

                sprintf(buff, "%s establece el modo  +m %s", nick, channel);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "-m")) {

                sprintf(buff, "%s establece el modo  -m %s", nick, channel);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "+i")) {

                sprintf(buff, "%s establece el modo  +i %s", nick, channel);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "-i")) {

                sprintf(buff, "%s establece el modo  -i %s", nick, channel);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "+s")) {

                sprintf(buff, "%s establece el modo  +s %s", nick, channel);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "-s")) {

                sprintf(buff, "%s establece el modo  -s %s", nick, channel);
                IRCInterface_WriteChannelThread(channel, "*", buff);
            } else if (strstr(mode, "+n")) {

                sprintf(buff, "%s establece el modo  +n %s", nick, channel);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "-n")) {

                sprintf(buff, "%s establece el modo  -n %s", nick, channel);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "+t")) {

                sprintf(buff, "%s establece el modo  +t %s", nick, channel);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "-t")) {

                sprintf(buff, "%s establece el modo  -t %s", nick, channel);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "+b")) {

                sprintf(buff, "%s banea a %s", nick, usert);
                IRCInterface_WriteChannelThread(channel, "*", buff);

            } else if (strstr(mode, "+v")) {

                sprintf(buff, "%s da voz a %s", nick, usert);
                IRCInterface_WriteChannelThread(channel, "*", buff);
                IRCInterface_ChangeNickStateChannelThread(channel, usert, VOICE);

            } else if (strstr(mode, "-v")) {

                sprintf(buff, "%s quita voz a %s", nick, usert);
                IRCInterface_WriteChannelThread(channel, "*", buff);
                IRCInterface_ChangeNickStateChannelThread(channel, usert, NONE);

            } else if (strstr(mode, "+o")) {

                sprintf(buff, "%s da OP a %s", nick, usert);
                IRCInterface_WriteChannelThread(channel, "*", buff);
                IRCInterface_ChangeNickStateChannelThread(channel, usert, OPERATOR);

            } else if (strstr(mode, "-o")) {

                sprintf(buff, "%s quita OP a %s", nick, usert);
                IRCInterface_WriteChannelThread(channel, "*", buff);
                IRCInterface_ChangeNickStateChannelThread(channel, usert, VOICE);

            }
            break;        
        

        case RPL_WELCOME:
        {


            IRCInterface_PlaneRegisterInMessageThread(command);

            IRCParse_RplWelcome(command, &prefix, &nick, &msg);

            IRCInterface_WriteSystemThread(NULL, msg);

            gethost = strtok(msg, "@");
            gethost = strtok(NULL, "@");
            mfree(2,nick,msg);
            break;
        }
        case RPL_CREATED:
        {
            IRCInterface_PlaneRegisterInMessageThread(command);

            IRCParse_RplCreated(command, &prefix, &nick, &timedate, &msg);

            IRCInterface_WriteSystemThread(NULL, msg);
            mfree(3,nick,timedate,msg);
            break;
        }
        case RPL_YOURHOST:
        {
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_RplYourHost(command, &prefix, &nick, &msg, &servername, &versionname);
            IRCInterface_WriteSystemThread(NULL, msg);
            mfree(4,nick,msg,servername,versionname);
            break;
        }
            /*case RPL_ISUPPORT: 005
            {
                break;
            }*/
        case RPL_LUSERCLIENT:
        {
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_RplLuserClient(command, &prefix, &nick, &msg, &nusers, &ninvisibles, &nservers);
            IRCInterface_WriteSystemThread(NULL, msg);
            mfree(2,nick,msg);
            break;
        }
        case RPL_LUSERME:
        {
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_RplLuserMe(command, &prefix, &nick, &msg, &nclientes, &nservers);
            IRCInterface_WriteSystemThread(NULL, msg);
            mfree(2,nick,msg);
            break;
        }
        case RPL_MOTDSTART:
        {
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_RplMotdStart(command, &prefix, &nick, &msg, &server);
            IRCInterface_WriteSystemThread(NULL, msg);
            mfree(3,nick,msg,server);
            break;
        }
        case RPL_MOTD:
        {
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_RplMotd(command, &prefix, &nick, &msg);
            IRCInterface_WriteSystemThread(NULL, msg);
            mfree(2,nick,msg);
            break;
        }
        case RPL_ENDOFMOTD:
        {
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_RplEndOfMotd(command, &prefix, &nick, &msg);
            IRCInterface_WriteSystemThread(NULL, msg);
            mfree(2,nick,msg);
            break;
        }
        case ERR_NICKNAMEINUSE:
        {
            char cambio[60] = "Usted es ahora conocido como ";
            char* errnick;
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_ErrNickNameInUse(command, &prefix, &nick, &errnick, &msg);
            strcat(cambio, msg);
            IRCInterface_WriteSystemThread(NULL, cambio);
            mfree(3,errnick,nick,msg);
            break;
        }

        case RPL_WHOREPLY:
        {
            if (IRCParse_RplWhoReply(command, &prefix, &nick, &channel, &user, &host, &server, &nick2, &type, &msg, &hopcount, &realname) == IRC_OK) {
                IRCInterface_PlaneRegisterInMessageThread(command);
                IRCInterface_AddNickChannelThread(channel, nick2, user, realname, host, NONE);
                mfree(10, prefix, nick, channel, user, host, server, nick2, type, msg, realname);
            }

            break;
        }

        case RPL_CHANNELMODEIS:
        {
            IRCParse_RplChannelModeIs(command, &prefix, &nick, &channel, &mode);
            IRCInterface_PlaneRegisterInMessageThread(command);
            modelong = IRCInterface_ModeToIntModeThread(mode);
            IRCInterface_SetModeChannelThread(channel, modelong);
            if (modelong == 200L) {
                IRCInterface_SetProtectTopic();
            }
            mfree(2,nick,channel);
            break;

        }
        case ERR_CHANOPRIVSNEEDED:
        {
            char cambio[100]="";
            long valor;
            IRCInterface_PlaneRegisterInMessageThread(command);
            valor=IRCParse_ErrChanOPrivsNeeded(command,&prefix,&nick,&channel,&msg);
            //if(valor==IRC_OK){
                sprintf(cambio,"%s %s",nick,msg);
            //}
            IRCInterface_WriteChannelThread(channel,NULL,cambio);
            mfree(3,nick,channel,msg);
            break;
        }
        case ERR_NOSUCHCHANNEL:
        {
            char cambio[50];
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_ErrNoSuchChannel(command, &prefix, &nick, &channel, &msg);
            sprintf(cambio, "%s : %s", channel, msg);
            IRCInterface_WriteSystemThread(NULL, cambio);
            mfree(3,nick,channel,msg);
            break;
        }
        case ERR_UNKNOWNMODE:
        {
            IRCInterface_PlaneRegisterInMessageThread(command);
            if (IRCParse_ErrUnknownMode(command, &prefix, &nick, &mode, &channel, &msg) == IRC_OK) {
                IRCInterface_WriteSystemThread(NULL, "Modo desconocido");
            }
            mfree(3,nick,channel,msg);
            break;
        }
        case RPL_WHOISUSER:
        {
            char cambio[100] = "";
            IRCInterface_PlaneRegisterInMessageThread(command);
            if (IRCParse_RplWhoIsUser(command, &prefix, &nick, &nick2, &name, &host, &realname) == IRC_OK) {
                sprintf(cambio, "[%s] (%s@%s): %s", nick, name, host, realname);
                IRCInterface_WriteSystemThread(NULL, cambio);
            }
            mfree(5,nick,nick2,name,host,realname);
            break;
        }
        case RPL_WHOISSERVER:
        {
            char cambio[100] = "";
            IRCInterface_PlaneRegisterInMessageThread(command);
            if (IRCParse_RplWhoIsServer(command, &prefix, &nick, &nick2, &server, &msg) == IRC_OK) {
                sprintf(cambio, "[%s] %s %s", nick, server, msg);
                IRCInterface_WriteSystemThread(NULL, cambio);
            }
            mfree(4,nick,nick2,server,msg);
            break;
        }
        case RPL_WHOISIDLE:
        {
            char cambio[100] = "";
            char conectado[9] = "";
            int sec_idle, signon;
            IRCInterface_PlaneRegisterInMessageThread(command);
            if (IRCParse_RplWhoIsIdle(command, &prefix, &nick, &nick2, &sec_idle, &signon, &realname) == IRC_OK) {
                //strftime(conectado,9,"%H:%M:%S",sec_idle);
                //FORMATO DE HORA?
                sprintf(cambio, "[%s] inactivo %d segundos, entró: %d", nick, sec_idle, signon);
                IRCInterface_WriteSystemThread(NULL, cambio);
            }
            mfree(3,nick,nick2,realname);
            break;
        }
        case RPL_ENDOFWHOIS:
        {
            char cambio[100] = "";
            IRCInterface_PlaneRegisterInMessageThread(command);
            if (IRCParse_RplEndOfWhoIs(command, &prefix, &nick, &name, &msg) == IRC_OK) {
                sprintf(cambio, "[%s] Final de la lista WHOIS", nick);
                IRCInterface_WriteSystemThread(NULL, cambio);
            }
            mfree(3,nick,name,msg);
            break;
        }
        case RPL_ENDOFWHO:
        {
            //char cambio[100]="";
            IRCInterface_PlaneRegisterInMessageThread(command);
            /*if(IRCParse_RplEndOfWho(command,&prefix,&nick,&name,&msg)==IRC_OK){
                sprintf(cambio,"[%s] Final de la lista WHOIS",nick);
                IRCInterface_WriteSystemThread(NULL, cambio);
            }*/
            break;
        }
        case ERR_NOSUCHNICK:
        {
            char cambio[50];
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_ErrNoSuchNick(command, &prefix, &nick, &name, &msg);
            sprintf(cambio, "%s :%s", nick, msg);
            IRCInterface_WriteSystemThread(NULL, cambio);
            mfree(3,nick,name,msg);
            break;
        }
        case RPL_AWAY:
        {
            //char cambio[50];
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_RplAway(command, &prefix, &nick, &name, &msg);
            //sprintf(cambio, "%s :%s", nick, msg);
            //IRCInterface_WriteSystemThread(NULL, cambio);
            mfree(3,nick,name,msg);
            break;
        }
        case RPL_NOWAWAY:
        {
            char cambio[50];
            IRCInterface_PlaneRegisterInMessageThread(command);
            IRCParse_RplNowAway(command, &prefix, &nick, &msg);
            sprintf(cambio, "%s", msg);
            IRCInterface_WriteSystemThread(NULL, cambio);
            mfree(2,nick,msg);
            break;
        }

    }
}

