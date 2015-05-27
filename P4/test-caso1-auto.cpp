/* $Id: test-caso1-auto.cpp 255 2015-05-25 11:36:52Z gerardo $
 * ©2014 Antonio G.ª Dguez. & el resto de profesores de POO
 */

#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <memory>
#include <locale>
#include <regex.h>		// <regex> aún no funciona bien

#include "../P2/fct.h"
#include "test-auto.h"

#include "tarjeta.h"
#include "usuario.h"
#include "articulo.h"

using namespace std;

// Variables para el siguiente conjunto de casos de prueba. ¡No tocar!
namespace {
  const Cadena referencia("1234XYZ");
  const Cadena titulo("Prueba");
  const Fecha  fecha(10, 10, 2000);
  const Cadena sId("pperez");
  const Cadena sNombre("Perico");
  const Cadena sApellidos("Perez Palotes");
  const Cadena sDireccion("13 Rue del Percebe");
  const Clave  clave("pedrofueacomprarpan");
  const Numero nTarjeta("01234 56789 012 8");
  const Numero nTarjeta2("01234567890128");
  const Numero nTarjeta3("11234567890126");
  const Fecha  fHoy;
  const Fecha  fUnaSemana = fHoy + 7;
  const Fecha  fSiguienteAnno(1, 1, fHoy.anno() + 1);

  Autor autor("Harry", "Potter", "Hogwarts");
  Articulo::Autores autores = crea_autores(autor);

  Libro articulo1(autores, "111", "The Standard Template Library",
                  fHoy, 42.10, 200, 50);
  Cederron articulo2(autores, "110", "Fundamentos de C++",
                     fHoy, 35.95, 100, 50);
  Usuario* pU;

  bool bPrimera = true;
}

FCTMF_FIXTURE_SUITE_BGN(test_p2) {

  if (bPrimera) {
    bPrimera = false;
    cerr << endl
         << "---------- PRÁCTICA 2 ----------"
         << endl << endl;
  }

  FCT_SETUP_BGN() {
    pU = new Usuario(sId, sNombre, sApellidos, sDireccion, clave);
  }
  FCT_SETUP_END();

  FCT_TEARDOWN_BGN() {
    if (pU) delete pU;
  }
  FCT_TEARDOWN_END();

  FCT_TEST_BGN(CLAVE - validacion: demasiado corta) {
    try {
      Clave c("hola");
      fct_chk(!"Se esperaba excepción");
    } catch (const Clave::Incorrecta& ex) {
      fct_chk_eq_int(ex.razon(), Clave::CORTA);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(CLAVE - verificacion: coincide) {
    const Clave c("buenas");
    fct_chk(c.verifica("buenas"));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(CLAVE - verificacion: no coincide) {
    const Clave c("buenas");
    fct_chk(!c.verifica("adioses"));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(NUMERO - validacion: muy corto) {
    try {
      Numero n("");
      fct_chk(!"Se esperaba una excepción");
    } catch (const Numero::Incorrecto& ex) {
      fct_chk_eq_int(ex.razon(), Numero::LONGITUD);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(NUMERO - validacion: muy largo) {
    try {
      Numero n("01234567890123456789");
      fct_chk(!"Se esperaba una excepción");
    } catch (const Numero::Incorrecto& ex) {
      fct_chk_eq_int(ex.razon(), Numero::LONGITUD);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(NUMERO - validacion: con caracteres no numericos) {
    try {
      Numero n("0123456789abcd");
      fct_chk(!"Se esperaba una excepción");
    } catch (const Numero::Incorrecto& ex) {
      fct_chk_eq_int(ex.razon(), Numero::DIGITOS);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(NUMERO - validacion: cifra de comprobacion no valida) {
    try {
      // La cifra de comprobación correcta es 8, no 2
      Numero n("01234 56789 012 2");
      fct_chk(!"Se esperaba una excepción");
    } catch (const Numero::Incorrecto& ex) {
      fct_chk_eq_int(ex.razon(), Numero::NO_VALIDO);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(NUMERO - conversion implicita a cadena) {
    const Numero n("01234 56789 012                                  8");
    fct_chk(strcmp(n, "01234567890128") == 0);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(NUMERO - operador menor que: solo cambios en espacios) {
    const Numero n1("01234\v5\r6 789\t012\f8");
    const Numero n2("01234567890128");
    fct_chk(!(n1 < n2));
    fct_chk(!(n2 < n1));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(NUMERO - operador menor que: numeros distintos) {
    const Numero n1("01234 56789 012 8");
    const Numero n2("11234567890126");
    fct_chk(n1 < n2);
    fct_chk(!(n2 < n1));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO - no es copiable) {
    fct_chk(!is_copy_constructible<Usuario>::value);
    fct_chk(!is_copy_assignable<Usuario>::value);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO - observadores) {
    const Usuario& rU = *pU;
    fct_chk(rU.id() == sId);
    fct_chk(rU.nombre() == sNombre);
    fct_chk(rU.apellidos() == sApellidos);
    fct_chk(rU.direccion() == sDireccion);
    fct_chk(rU.tarjetas() == Usuario::Tarjetas());
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO - identificador repetido) {
    try {
      Usuario u(sId,
                "otro nombre", "otros apellidos", "otra dirección",
                Clave("otraclave"));
      fct_chk(!"Se esperaba una excepción");
    } catch (const Usuario::Id_duplicado& ex) {
      fct_chk(ex.idd() == sId);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(TARJETA - no es copiable) {
    fct_chk(!is_copy_constructible<Tarjeta>::value);
    fct_chk(!is_copy_assignable<Tarjeta>::value);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(TARJETA - caducada) {
    const Fecha f(1, 1, 2000);
    try {
      Tarjeta tarjeta(nTarjeta, *pU, f);
      fct_chk(!"Se esperaba una excepción");
    } catch (const Tarjeta::Caducada& ex) {
      fct_chk(ex.cuando() == f);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(TARJETA - observadores) {
    const Tarjeta tarjeta(nTarjeta, *pU, fUnaSemana);
    fct_chk(tarjeta.numero() == nTarjeta);
    fct_chk(tarjeta.caducidad() == fUnaSemana);
    fct_chk(tarjeta.titular() == pU);
    fct_chk_eq_istr(tarjeta.titular_facial().c_str(),
                    "PERICO PEREZ PALOTES");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(TARJETA - comparacion) {
    const Tarjeta tarjeta1(nTarjeta, *pU, fUnaSemana),
      tarjeta2(nTarjeta2, *pU, fUnaSemana),
      tarjeta3(nTarjeta3, *pU, fUnaSemana);
    fct_chk(!(tarjeta1 < tarjeta2));
    fct_chk(!(tarjeta2 < tarjeta1));
    fct_chk(  tarjeta1 < tarjeta3);
    fct_chk(!(tarjeta3 < tarjeta1));
    fct_chk(  tarjeta2 < tarjeta3);
    fct_chk(!(tarjeta3 < tarjeta2));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(TARJETA - insercion en flujo) {
    const Tarjeta tarjeta(nTarjeta, *pU, fSiguienteAnno);
    const string sTarjeta = toString(tarjeta);
    chk_incl_cstr(sTarjeta, nTarjeta);
    chk_incl_str(sTarjeta, tarjeta.titular_facial());
    chk_incl_cstr(sTarjeta, "Caduca:");

    // Equivalente en C++ a printf("%02d/%02d", mes, anno%100);
    ostringstream os;
    os << setfill('0')
       << setw(2)
       << tarjeta.caducidad().mes()
       << "/"
       << setw(2)
       << (tarjeta.caducidad().anno() % 100);
    const string sFecha = os.str();
    chk_incl_str(sTarjeta, sFecha);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO<->TARJETA - destruccion de Tarjeta) {
    const Usuario::Tarjetas& tarjetas = pU->tarjetas();
    {
      Tarjeta tarjeta(nTarjeta, *pU, fUnaSemana);
      if(tarjetas.size() == 1) {
        fct_chk(tarjetas.begin()->second->numero() == tarjeta.numero());
      } else {
        fct_chk(!"Debería haber una tarjeta");
      }
    }
    fct_chk(tarjetas.empty());
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO<->TARJETA - destruccion de Usuario) {
    const Tarjeta tarjeta(nTarjeta, *pU, fUnaSemana);
    delete pU;
    pU = 0;
    fct_chk(tarjeta.titular() == 0);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO<->TARJETA - insercion en flujo) {
    const Tarjeta tarjeta(nTarjeta, *pU, fUnaSemana);
    const string s = toString(*pU);
    chk_incl_str(s, sId);
    chk_incl_str(s, clave.clave());
    chk_incl_str(s, sNombre);
    chk_incl_str(s, sApellidos);
    chk_incl_str(s, sDireccion);
    chk_incl_cstr(s, "Tarjetas:");
    chk_incl_cstr(s, nTarjeta);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO<->ARTICULO - meter en el carrito) {
    fct_chk_eq_int(pU->n_articulos(), 0);
    fct_chk(pU->compra().empty());
    pU->compra(articulo1, 3);
    if (pU->n_articulos() == 1) {
      fct_chk(pU->compra().begin()->first->referencia() == articulo1.referencia());
      fct_chk_eq_int(pU->compra().begin()->second, 3);
    } else {
      fct_chk(!"Debería haber un artículo en el carrito");
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO<->ARTICULO - meter varias veces) {
    pU->compra(articulo1, 1);
    pU->compra(articulo1, 3);
    if (pU->n_articulos() == 1)  {
      fct_chk_eq_int(pU->compra().find(&articulo1)->second, 3);
    } else {
      fct_chk(!"Debería haber un artículo en el carrito");
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO<->ARTICULO - meter varios articulos) {
    pU->compra(articulo1, 1);
    pU->compra(articulo2, 10);
    if (pU->n_articulos() == 2) {
      fct_chk_eq_int(pU->compra().find(&articulo1)->second, 1);
      fct_chk_eq_int(pU->compra().find(&articulo2)->second, 10);
    } else {
      fct_chk(!"Debería haber dos artículos en el carrito");
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO<->ARTICULO - meter y sacar) {
    pU->compra(articulo1, 5);
    pU->compra(articulo2, 7);
    pU->compra(articulo1, 0);
    if (pU->n_articulos() == 1) {
      fct_chk(pU->compra().begin()->first->referencia()
              == articulo2.referencia());
      fct_chk_eq_int(pU->compra().find(&articulo2)->second, 7);
    } else {
      fct_chk(!"Debería haber un artículo en el carrito");
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(USUARIO<->ARTICULO - mostrar_carro) {
    pU->compra(articulo1, 1);
    pU->compra(articulo2, 3);

    ostringstream os;
    os.imbue(std::locale(std::locale("es_ES.UTF-8"), new sin_separador()));
    mostrar_carro(os, *pU);
    string sCarro = os.str();
    chk_incl_str(sCarro, sId);
    chk_incl_cstr(sCarro, "Artículos: 2");

    os.str("");
    os << "[111] \"The Standard Template Library\", "
       << articulo1.f_publi().anno()
       << ". 42,10 €";
    chk_incl_str(sCarro, os.str());

    os.str("");
    os << "[110] \"Fundamentos de C++\", "
       << articulo2.f_publi().anno()
       << ". 35,95 €";
    chk_incl_str(sCarro, os.str());
  }
  FCT_TEST_END();
}
FCTMF_FIXTURE_SUITE_END()
