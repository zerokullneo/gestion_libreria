# $Id: Makefile 227 2015-04-21 12:34:41Z gerardo $
# Makefile para la práctica 2, caso de uso 1 de la librería electrónica
# ©2009-2015 Los profesores de POO
#
#--- Parte configurable
# NOMBREALUMNO es el nombre del directorio donde el alumno guarda las prácticas
# de POO (C++). Dentro de ese directorio debe haber uno para cada práctica
# llamado NOMBREPRACTICA, que será de la forma P<n>, siendo <n> el número
# de práctica: P1, P2, etc.
# NOMBREALUMNO debería ser de la forma: Ap1_Ap2_NP
# (Véanse las instrucciones de entrega).
NOMBREALUMNO   = JOSE_MANUEL_BARBA_GONZALEZ
NOMBREPRACTICA = P3

# CADENADIR y FECHADIR son los directorios donde están respectivamente
# los Makefile, módulos objeto y cabeceras de Cadena y Fecha:
# Makefile, cadena.{oh} y fecha.{oh}.
# TESTHDR es el directorio donde está la cabecera del test automático, fct.h
DIRFECHACADENA=P1
CADENADIR= ../${DIRFECHACADENA}/Cadena
FECHADIR = ../${DIRFECHACADENA}/Fecha
TESTHDR = ../${DIRFECHACADENA}

# ¿Compilador C++ de LLVM (clang++) o de GNU (g++)?
#CXX      = clang++
CXX	 = g++

# ¿Qué estándar seguir?
# ansi o c++98 o c++03: estándar de 1998, corregido en 2003
# c++0x: borrador del estándar de 2011
# c++11: estándar de 2011
# c++1y: sgute. revisión (2014)
STD=c++11

ifeq (${CXX},clang++)
  CXXFLAGS = -g -Wall -std=${STD} ${INCLUDES}
else
  CXXFLAGS = -g -pedantic -Wall -std=${STD} ${INCLUDES}
endif
# --- Fin parte configurable

INCLUDES  = -I${CADENADIR} -I${FECHADIR} -I${TESTHDR}
LOCALLIBS = cadena.o fecha.o
EXTLIBS   = -lcrypt
LIBS      = ${LOCALLIBS} ${EXTLIBS}
VPATH     = .:${CADENADIR}:${FECHADIR}

COMM_HDRS = usuario.h tarjeta.h articulo.h pedido.h pedido-articulo.h usuario-pedido.h test-auto.h
COMM_SRCS = usuario.cpp tarjeta.cpp articulo.cpp pedido.cpp pedido-articulo.cpp luhn.cpp Makefile
COMM_OBJS = usuario.o tarjeta.o articulo.o pedido.o pedido-articulo.o luhn.o
MAIN_SRCS = $(COMM_SRCS) test-caso4-consola.cpp
MAIN_OBJS = $(COMM_OBJS) test-caso4-consola.o
MAIN_EXE  = test-caso4-consola
MAIN_SRCS = $(COMM_SRCS) test-auto.cpp test-caso3-auto.cpp test-caso4-auto.cpp
TEST_OBJS = $(COMM_OBJS) test-auto.o test-caso3-auto.o test-caso4-auto.o
TEST_EXE  = test-caso3-auto test-caso4-auto
SRCS      = $(MAIN_SRCS)
HDRS      = $(COMM_HDRS)

NOMBRETAR = ../../${NOMBREALUMNO}.tar.gz
NOMBREDIR = ${NOMBREALUMNO}/${NOMBREPRACTICA}

all: ${MAIN_EXE} ${TEST_EXE}

Debug: ${TEST_EXE}
	./$<

test-consola: ${MAIN_EXE}
	./$<

test-auto: ${TEST_EXE}
	./$<

check: compra_check
	./$< usuario.cpp tarjeta.cpp articulo.cpp -- -std=c++11

${MAIN_EXE}: ${MAIN_OBJS} ${LOCALLIBS}
	${CXX} -o $@ ${LDFLAGS} $^ ${EXTLIBS}

${TEST_EXE}: ${TEST_OBJS} ${LOCALLIBS}
	${CXX} -o $@ ${LDFLAGS} $^ ${EXTLIBS}

${MAIN_OBJS}: ${COMM_HDRS}

${TEST_OBJS}: ${COMM_HDRS}

clean:
	${RM} ${RMFLAGS} ${MAIN_EXE} ${MAIN_OBJS} ${TEST_EXE} ${TEST_OBJS} \
			 ${LOCALLIBS} *.{aux,log,dvi,toc} *~ \#*
	make -C ${CADENADIR} clean
	make -C ${FECHADIR}  clean

# Esto lo deberían usar los alumnos a la hora de la entrega:
# crea el archivo .tar.gz en el directorio padre tras borrar lo sobrante.
dist: clean ${NOMBRETAR}

${NOMBRETAR}: ${SRCS} ${HDRS}
ifeq ($(strip $(NOMBREALUMNO)),)
	@echo ""
	@echo "ATENCIÓN: debe definir en el Makefile la variable NOMBREALUMNO"
	@echo "con el nombre del directorio superior a este."
	@echo " Vea las instrucciones de entrega de las prácticas."
else
	tar -cvz -C../.. -f ${NOMBRETAR} ${NOMBREDIR} \
				${NOMBREALUMNO}/${DIRFECHACADENA}
endif

distclean: clean
	${RM} ${RMFLAGS} ${NOMBRETAR}

.PHONY: all clean dist distclean
