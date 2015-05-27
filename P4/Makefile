# $Id: Makefile 255 2015-05-25 11:36:52Z gerardo $
# Makefile para el caso 4 de la LIBRERÍA
# ©2010-15 Inma, Gerardo, Fidel, Juanma, Antonio
#
# --- Parte configurable ---

# NOMBREALUMNO es el nombre del directorio donde el alumno guarda las
# prácticas de POO (C++). Dentro de ese directorio debe haber uno para
# cada práctica llamado NOMBREPRACTICA, que será de la forma P<n>,
# siendo <n> el número de práctica: P0, P1, etc.

NOMBREALUMNO   = JOSE_MANUEL_BARBA_GONZALEZ
NOMBREPRACTICA = P4

# CADENADIR y FECHADIR son los directorios donde están respectivamente
# los módulos objeto y cabeceras de Cadena y Fecha:
# Makefile, cadena.{oh} y fecha.{oh}.
# TESTHDR es el directorio donde está la cabecera para el test automático, fct.h
# LUHNDIR es el directorio donde está luhn.{cpp,o} ¡Si se cambia, hay que
# cambiar P2 en la regla dist y poner donde esté!
DIRFECHACADENA=P1
CADENADIR= ../${DIRFECHACADENA}/Cadena
FECHADIR = ../${DIRFECHACADENA}/Fecha
TESTHDR  = ../P2
LUHNDIR  = ../P2

# ¿Compilador C++ de LLVM (clang++) o de GNU (g++, c++, g++-VERSIÓN)?
#CXX      = clang++
CXX = g++

# ¿Qué estándar seguir?
# ansi o c++98 o c++03: estándar de 1998, corregido en 2003
# c++0x: borrador del estándar de 2011
# c++11: estándar de 2011
# c++1y: estándar actual de 2014 (clang++ admite c++14 en vez de c++1y)
# c++1z: estándar hipotético de 2017
# RECOMENDADO EN EL CURSO 2014-15: c++11
STD=c++11

CXXFLAGS = -g -pedantic -Wall -std=${STD} ${INCLUDES}

# --- Fin parte configurable

INCLUDES  = -I${CADENADIR} -I${FECHADIR} -I${TESTHDR}
LOCALLIBS = ${CADENADIR}/cadena.o ${FECHADIR}/fecha.o ${LUHNDIR}/luhn.o
EXTLIBS   = -lcrypt
LIBS      = ${LOCALLIBS} ${EXTLIBS}
VPATH     = .:${CADENADIR}:${FECHADIR}

COMM_SRCS = usuario.cpp tarjeta.cpp articulo.cpp pedido.cpp \
	pedido-articulo.cpp ${LUHNDIR}/luhn.cpp
COMM_HDRS = articulo.h pedido-articulo.h pedido.h tarjeta.h usuario.h \
	usuario-pedido.h
MAIN_SRCS = test-caso4-consola.cpp
MAIN_OBJS = $(COMM_SRCS:.cpp=.o) $(MAIN_SRCS:.cpp=.o)
MAIN_EXE  = test-caso4-consola
TEST_SRCS = test-caso0-cadena-auto.cpp test-caso0-fecha-auto.cpp \
  test-caso1-auto.cpp test-caso3-auto.cpp test-caso4-auto.cpp test-auto.cpp
TEST_HDRS = test-auto.h ${TESTHDR}/fct.h
TEST_OBJS = $(COMM_SRCS:.cpp=.o) $(TEST_SRCS:.cpp=.o)
TEST_EXE  = test-p1_4-auto

SRCS = ${COMM_SRCS} ${TEST_SRCS} ${MAIN_SRCS}
HDRS = ${COMM_HDRS} ${TEST_HDRS}
NOMBRETAR = ../../${NOMBREALUMNO}.tar.gz
NOMBREDIR = ${NOMBREALUMNO}/${NOMBREPRACTICA}

## OBJETIVOS PRINCIPALES #######################################################

.PHONY: all clean dist distclean test-consola test-auto

all: ${MAIN_EXE} ${TEST_EXE}

Debug: ${MAIN_EXE} ${TEST_EXE}

test-consola: ${MAIN_EXE}
	./$<

test-auto: ${TEST_EXE}
	./$<

check: catalogo_check
	ln usuario-pedido.h usuario-pedido.hpp
	./$< ${COMM_SRCS} usuario-pedido.hpp -- -std=c++11 ${INCLUDES}
	${RM} usuario-pedido.hpp

clean:
	${RM} ${MAIN_EXE} ${MAIN_OBJS} ${TEST_EXE} ${TEST_OBJS} \
		${LUHNDIR}/luhn.o *.d *~ \#*
	${MAKE} -C ${FECHADIR}  clean
	${MAKE} -C ${CADENADIR} clean

## COMPILACIÓN #################################################################

${MAIN_EXE}: ${MAIN_OBJS} ${LOCALLIBS}
	${CXX} -o $@ ${LDFLAGS} $^ ${EXTLIBS}

${TEST_EXE}: ${TEST_OBJS} ${LOCALLIBS}
	${CXX} -o $@ ${LDFLAGS} $^ ${EXTLIBS}

# Detección automática de dependencias en código fuente C++ con GCC #

%.d: %.cpp
	@$(CXX) -MM $(CXXFLAGS) $< > $@
	@sed -ri 's,($*)\.o[ :]*,\1.o $@ : ,g' $@

-include $(COMM_SRCS:.cpp=.d) $(MAIN_SRCS:.cpp=.d) $(TEST_SRCS:.cpp=.d)

## COBERTURA / PERFILADO #######################################################

# Descomentando estas dos líneas de CXXFLAGS y LDFLAGS se activa el soporte
# para las pruebas de cobertura y el perfilado de los programas.
# Una vez tenemos esto y recompilamos por completo (generando una serie de
# ficheros .gcno para gcov), podemos ejecutar el programa de la forma usual,
# y se generarán gmon.out (para gprof) y un .gcda (para gcov) para cada .cpp
# y .h del programa ejecutado.
#
# Desde el directorio donde está gmon.out, podremos hacer:
#
# gprof ejecutable
#
# para ver (en la salida estándar) qué funciones han consumido más tiempo.
# Y podremos usar:
#
# gcov fichero.cpp
#
# para ver qué líneas se han ejecutado y cuáles no en fichero.cpp.gcov.

#CXXFLAGS = -g -pg -fprofile-arcs -ftest-coverage -std=${STD} -pedantic -Wall ${INCLUDES}
#LDFLAGS = -pg -fprofile-arcs

# Posibles herramientas de pruebas a considerar:
# - valgrind (para fugas de memoria, http://valgrind.org/)
# - dehydra  (para análisis estático del código, https://developer.mozilla.org/en/Dehydra)

## EMPAQUETADO #################################################################

${NOMBRETAR}: ${SRCS} ${HDRS} ${LUHNDIR}/luhn.cpp Makefile
ifeq ($(strip $(NOMBREALUMNO)),)
	@echo ""
	@echo "ATENCIÓN: debe definir en el Makefile la variable NOMBREALUMNO"
	@echo "con el nombre del directorio superior a este."
	@echo " Vea las instrucciones de entrega de las prácticas."
else
	tar -cvz -C../.. -f ${NOMBRETAR} ${NOMBREALUMNO}/${NOMBREPRACTICA} \
				${NOMBREALUMNO}/${DIRFECHACADENA} \
				${NOMBREALUMNO}/P2/luhn.cpp \
				${NOMBREALUMNO}/fct.h
endif

# Esto lo deberían usar los alumnos a la hora de la entrega:
# crea el archivo .tar.gz en el directorio padre tras borrar lo sobrante.
dist: clean ${NOMBRETAR}

distclean: clean
	${RM} ${NOMBRETAR}
