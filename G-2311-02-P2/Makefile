#============================================================
# Proyecto: REDES II
# Fichero: Makefile
# Fecha: 02/04/2016
# Descripcion: Makefile
# Integrantes: Francisco Andreu Sanz, Javier Martínez Hernández
#============================================================

#============================================================
# Entorno de desarrollo y opciones
#============================================================

# Compilador
CC	= gcc
# Opciones de compilacion
## -wall: indica que se muestren todas las advertencias al compilar
## -g: genera informacion de depuracion
CFLAGS	= -Wall -g

## Sirve para la generacion del archivo de distribucion
DIR	= ~/G-2311-02-P2
TAR = G-2311-02-P2

#============================================================
# Archivos que componen el proyecto
#============================================================

EJECUTABLES	= cliente
xchat2		= xchat2.o
CONNECTSERVER   = connectClient.o
LIBS      	= -lircredes -lirctad -lpthread -lircinterface -lsoundredes -rdynamic
EXEC_NAME 	= cliente
SRC			= src/
SRCLIB			= srclib/
INCL		= includes/
OBJ			= obj/
LIB		= lib/
#============================================================
#Construccion de los ejecutables
#============================================================

all: dist $(EJECUTABLES) autores

cliente:    $(OBJ)$(xchat2) $(LIB)libClientFuncs.a
	$(CC) -o $(EXEC_NAME) $(OBJ)$(xchat2) `pkg-config --cflags gtk+-3.0` $(LIB)libClientFuncs.a $(LIBS) `pkg-config --libs gtk+-3.0`


#============================================================
# Generacion de los .o
#============================================================
## Para cada archivo .c se genera un .o
## Un archivo .o depende del archivo .c y todos los .h propios que se incluyan en el .c

#main.o: $(INCL)connectServer.h $(SRC)main.c
#	@echo Compilando main.c...
	#$(CC) $(CFLAGS) -c -lircredes $(SRC)main.c
$(OBJ)xchat2.o: $(SRC)xchat2.c
	$(CC) -c $< -o $@ `pkg-config --cflags gtk+-3.0` -I./includes/

$(OBJ)clientfuncs.o: $(SRCLIB)clientfuncs.c
	$(CC) -c $< -o $@ `pkg-config --cflags gtk+-3.0` -I./includes/

$(LIB)libClientFuncs.a: $(OBJ)clientfuncs.o
	ar r $(LIB)libClientFuncs.a $(OBJ)clientfuncs.o

#============================================================
# Crea un archivo .tgz para distribuir el codigo
#============================================================
##Crea un archivo .tgz elmininando previamente el codigo objeto

dist: clean
	#@ cp $(DIR)/Makefile $(DIR)/G-2311-02-P1-makefile
	tar -cvf $(TAR).tar.gz ../$(TAR)


#============================================================
# Limpia los ficheros temporales
#============================================================

clean:
	#rm -f $(EXEC_NAME) $(MAIN) $(CONNECTSERVER)
	rm -f $(EXEC_NAME) all $(OBJ)*.o $(LIB)*.a $(DIR)/*~ core $(DIR)/Makefile

#============================================================
# Mensaje que aparece al ejecutar el MAKE, y que ayuda a identificar
# a los autores del trabajo
#============================================================

autores:
	@echo "-----------------------------------------------------------------------"
	@echo "Practica Redes2 grupo 2311 desarrollado por el equipo 02 compuesto por:"
	@echo "Andreu Sanz, Francisco y Martínez Hernández, Javier"
	@echo "-----------------------------------------------------------------------"

#============================================================
# Ayuda
#============================================================

ayuda:
	@echo Uso: make [objetivo]
	@echo "make \t\t Make all"
	@echo "make dist \t Genera el archivo de distribucion .tgz"
	@echo "make limpia \t Limpia los ficheros temporales"
