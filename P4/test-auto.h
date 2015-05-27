/* -*-c++-*-
 * $Id: test-auto.h 246 2015-05-19 20:47:08Z gerardo $ 
 *
 * ©2014-15 Antonio G.ª Dguez. & el resto de profesores de POO
 */
#ifndef TEST_AUTO_H_
#define TEST_AUTO_H_

#include <string>
#include <sstream>
#include <stdexcept>
#include <regex.h>
#include <locale>

#include "articulo.h"
#include "fecha.h"
#include "tarjeta.h"

/********************* COMPROBACIONES *********************/

#define chk_incl_str(haystack, needle)                          \
  fct_xchk(strstr(haystack.c_str(), needle.c_str()) != NULL,    \
           "failed chk_incl_str:\n'%s' not in '%s'",            \
           needle.c_str(), haystack.c_str())

#define chk_incl_cstr(haystack, needle)                         \
  fct_xchk(strstr(haystack.c_str(), needle) != NULL,            \
           "failed chk_incl_cstring:\n'%s' not in '%s'",        \
           (const char*)needle, haystack.c_str())

#define chk_eq_str(haystack, needle)                         \
  fct_xchk(strcmp(haystack.c_str(), needle.c_str()) == 0,    \
           "failed chk_incl_str:\n'%s' != '%s'",             \
           needle.c_str(), haystack.c_str())

#define chk_eq_cstr(haystack, needle)                        \
  fct_xchk(strcmp(haystack.c_str(), needle) == 0,            \
           "failed chk_incl_str:\n'%s' != '%s'",             \
           needle.c_str(), haystack)

/********************* CLASES *********************/

/**
   Clase de excepción para expresiones regulares no válidas.
*/
class BadRegex : public std::exception {
public:
  BadRegex(const char* regex) : regex_(regex) {}
  const char* regex() const { return regex_; }
private:
  const char* regex_;
};

/********************* UTILIDADES *********************/

/**
   Comprueba que dos números de tarjeta son iguales. El enunciado solo
   pide <, y no ==, por lo que emulamos == mediante <. Se aplica la
   siguiente equivalencia:

   a == b iff < es orden total y !(a < b) y !(b < a)
*/
inline bool operator ==(const Numero& a, const Numero& b) {
  return !(a < b) && !(b < a);
}

// Esto es para eliminar el separador de decimales, porque da problemas con
// algunas localizaciones españolas, que incorrectamente ponen el ".". También
// de paso fijamos el separador de decimales a la coma.
struct sin_separador : std::numpunct<char> {
protected:
  virtual string_type do_grouping() const 
  { return "\000"; } // grupos de 0 (deshabilitar)
  virtual char_type do_decimal_point() const { return ','; }
};


/**
   Función de utilidad para convertir algo a cadena, aprovechando su
   operador de inserción en flujo.
*/
template<typename T>
std::string toString(const T& o) {
  std::ostringstream os;
  os.imbue(std::locale(std::locale("es_ES.UTF-8"), new sin_separador()));
  os << o;
  return os.str();
}


/**
   Convierte una cantidad a euros, con su '€' y todo.
 */
std::string toEuros(double cantidad);

/**
   Crea un conjunto de autores con un solo autor. Útil cuando sólo
   podemos contar con un bloque namespace donde inicializarlo todo.
 */
Articulo::Autores crea_autores(Autor& autor);

/*
  Función que cambia la fecha de caducidad de una tarjeta.

  Esto es un "hack" protagonizado por el "hacker" Gerardo
  para evitar el tener que declarar la función
  caducar() como amiga de la clase Tarjeta, ya que ni eso tiene
  sentido ni se dice en el enunciado que se toque dicha clase para hacer
  ese cambio. Sólo se utiliza para probar un pago con una tarjeta
  caducada, que por cierto, ya hay que ser cierrabares y pudrecolchones para
  intentar pagar con una tarjeta caducada.
*/
void caducar(Tarjeta& t, const Fecha& f);

/**
   Función que busca una expresión regular dentro de una cadena y
   devuelve la posición del comienzo de la primera
   coincidencia. Devuelve -1 cuando no encuentra ninguna. Lanza la
   excepción BadRegex cuando la expresion regular no es válida.
*/
regoff_t find_regex(const char* regex, const char* text) throw (BadRegex);

#endif
