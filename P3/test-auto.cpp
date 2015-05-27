/* $Id: test-auto.cpp 251 2015-05-22 19:38:11Z gerardo $
 * Pruebas automáticas para el S. G. L.
 * Prácticas de POO
 * ©2014-15 Antonio G.ª Domínguez y el resto de profesores de POO
 */
#include <iomanip>
#include <locale>
#include "fct.h"
using namespace std;

#include "test-auto.h"

/*
  Un Fichero para gobernarlos a todos, un Fichero para encontrarlos,
  un Fichero para atraerlos a todos y ejecutarlos de una vez, que
  resulta más cómodo.

  Incluye también varias funciones de utilidad para las pruebas.
*/

Articulo::Autores crea_autores(Autor& autor) {
  Articulo::Autores autores;
  autores.insert(&autor);
  return autores;
}

/**
   Convierte un número de coma flotante de doble precisión a una
   cantidad de euros, con dos dígitos decimales.
*/
string toEuros(double cantidad) {
  ostringstream os;
  os.imbue(locale(locale("es_ES.UTF-8"), new sin_separador()));
  os << fixed << setprecision(2) << cantidad << " €";
  return os.str();
}

void caducar(Tarjeta& t, const Fecha& f)
{
  struct SusioTruco {
    // Los atributos deben ir exactamente en el mismo orden que en Tarjeta.
    // Solo importa la fecha de caducidad.
    Numero n;
    const Usuario* u;
    Fecha la_caducidad;
    Cadena t;
  } *pst = reinterpret_cast<SusioTruco*>(&t); // Aquí está lo bueno.
  pst->la_caducidad = f;          // Y aquí se comete el pecado. }-)
}

regoff_t find_regex(const char* regex, const char* text) throw (BadRegex) {
  regex_t reg;
  if (regcomp(&reg, regex, REG_EXTENDED) != 0) {
    throw BadRegex(regex);
  }

  regmatch_t matches[1] = {{-1, -1}};
  regexec(&reg, text, 1, matches, 0);
  regfree(&reg);

  return matches[0].rm_so;
}

/***** CUERPO PRINCIPAL *****/

FCT_BGN() {

  // Establecemos la localización por omisión a la española de España
  // con UTF-8, para asegurarnos de que las pruebas funcionen como
  // deben. Si no, se mezclarán los separadores decimales ingleses
  // ('.') y españoles (',').
  locale::global(locale("es_ES.UTF-8"));

  // No sirve para nada: simplemente callamos a GCC C++ para que no
  // incordie al compilar con avisos de funciones no usadas con FCTX.
  // Anulado porque ya no hace falta con las últimas versiones de GCC.
#if 0
  FCT_SUITE_BGN(callar avisos de GCC) {
    FCT_TEST_BGN(callar avisos de GCC) {
      fct_chk(true);
    } FCT_TEST_END();
  }
  FCT_SUITE_END();
#endif
  FCTMF_SUITE_CALL(test_cadena);
  FCTMF_SUITE_CALL(test_fecha);
  FCTMF_SUITE_CALL(test_p2);
  FCTMF_SUITE_CALL(test_p3_clases);
  FCTMF_SUITE_CALL(test_p3_informes);
  FCTMF_SUITE_CALL(test_p4);
  cerr << endl;
}
FCT_END()
