/* $Id: test-caso1-auto.cpp 229 2015-04-21 20:21:04Z gerardo $
 * Test para las pruebas automáticas de los casos 1 y 2 del SGL.
 * ©2015 Los profesores de POO
 */
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include <type_traits>
#include <regex>
#include "../P1/fct.h"
#include "tarjeta.h"
#include "usuario.h"
#include "articulo.h"

using namespace std;

/**
   Comprueba que dos números de tarjeta son iguales. El enunciado solo
   pide <, y no ==, por lo que emulamos == mediante <. Se aplica la
   siguiente equivalencia:

   a == b iff < es orden total y !(a < b) y !(b < a)
*/
inline bool operator ==(const Numero& a, const Numero& b) {
  return !(a < b) && !(b < a);
}

FCT_BGN() {

  FCT_SUITE_BGN(Clave) {
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
  }
  FCT_SUITE_END();

  FCT_SUITE_BGN(Numero) {
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
      const Numero n("01234 56789 012 8");
      fct_chk(strcmp(n, "01234567890128") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(NUMERO - operador menor que: solo cambios en espacios) {
      const Numero n1("01234 56789 012 8");
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
  }
  FCT_SUITE_END();

  // Variables para el siguiente conjunto de casos de prueba. ¡No tocar!
  const Cadena referencia("1234XYZ");
  const Cadena titulo("Prueba");
  const Fecha    fecha(10, 10, 2000);
  const double   precio = 20.4;
  const unsigned stock = 30;
  Articulo *pA = 0;

  FCT_FIXTURE_SUITE_BGN(Articulo) {
    FCT_SETUP_BGN() {
      pA = new Articulo(referencia, titulo, fecha, precio, stock);
    }
    FCT_SETUP_END();

    FCT_TEARDOWN_BGN() {
      delete pA;
    }
    FCT_TEARDOWN_END();

    FCT_TEST_BGN(ARTICULO - observadores) {
      const Articulo& rA = *pA;
      fct_chk(rA.referencia() == referencia);
      fct_chk(rA.titulo() == titulo);
      fct_chk(rA.f_publi() == fecha);
      fct_chk_eq_int(rA.precio(), precio);
      fct_chk_eq_int(rA.stock(), stock);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(ARTICULO - modificacion) {
      const double nuevoPrecio = 10.5;
      const unsigned nuevoStock = pA->stock() + 1;
      pA->precio() = nuevoPrecio;
      pA->stock() = nuevoStock;
      fct_chk_eq_int(pA->precio(), nuevoPrecio);
      fct_chk_eq_int(pA->stock(), nuevoStock);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(ARTICULO - insercion en flujo) {
      const Articulo& rA = *pA;
      ostringstream os;
      os << rA;
      fct_chk(strcmp(os.str().c_str(),
                   "[1234XYZ] \"Prueba\", 2000. 20.40 €") == 0);
    }
    FCT_TEST_END();
  }
  FCT_FIXTURE_SUITE_END();

  // Variables para el siguiente conjunto de casos de prueba. ¡No tocar!
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
  Articulo articulo1("111", "The Standard Template Library",
                     fHoy, 42.10, 10);
  Articulo articulo2("110", "Fundamentos de C++",
                     fHoy, 35.95, 10);
  Usuario* pU;

  FCT_FIXTURE_SUITE_BGN(Usuario + Tarjeta + Articulo) {
    FCT_SETUP_BGN() {
      pU = new Usuario(sId, sNombre, sApellidos, sDireccion, clave);
    }
    FCT_SETUP_END();

    FCT_TEARDOWN_BGN() {
      if (pU) delete pU;
    }
    FCT_TEARDOWN_END();

    /*FCT_TEST_BGN(USUARIO - no es copiable) {
      fct_chk(!is_copy_constructible<Usuario>::value);
      fct_chk(!is_copy_assignable<Usuario>::value);
    }
    FCT_TEST_END();*/

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

    /*FCT_TEST_BGN(TARJETA - no es copiable) {
      fct_chk(!is_copy_constructible<Tarjeta>::value);
      fct_chk(!is_copy_assignable<Tarjeta>::value);
    }
    FCT_TEST_END();*/

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
      ostringstream os;
      os << tarjeta;
      const string sTarjeta = os.str();
      fct_chk(sTarjeta.find(nTarjeta) != string::npos);
      fct_chk(sTarjeta.find(tarjeta.titular_facial().c_str()) != string::npos);
      fct_chk(sTarjeta.find("Caduca:") != string::npos);

      // Equivalente en C++ a printf("%02d/%02d", mes, anno%100);
      os.str("");
      os << setfill('0')
         << setw(2)
         << tarjeta.caducidad().mes()
         << "/"
         << setw(2)
         << (tarjeta.caducidad().anno() % 100);
      string sFechaCad = os.str();
      fct_chk(sTarjeta.find(sFechaCad) != string::npos);
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
      ostringstream os;
      const Usuario& rU = *pU;
      os << rU;
      string s = os.str();
      fct_chk(s.find(sId.c_str())           != string::npos);
      fct_chk(s.find(clave.clave().c_str()) != string::npos);
      fct_chk(s.find(sNombre.c_str())       != string::npos);
      fct_chk(s.find(sApellidos.c_str())    != string::npos);
      fct_chk(s.find(sDireccion.c_str())    != string::npos);
      fct_chk(s.find("Tarjetas:")           != string::npos);
      fct_chk(s.find(nTarjeta)              != string::npos);
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

    FCT_TEST_BGN(USUARIO<->ARTICULO - comprar y devolver) {
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
      mostrar_carro(os, *pU);
      string sCarro = os.str();
      fct_chk(sCarro.find(sId.c_str())    != string::npos);
      fct_chk(sCarro.find("Artículos: 2") != string::npos);

      os.str("");
      os << articulo1;
      fct_chk(sCarro.find(os.str()) != string::npos);

      os.str("");
      os << articulo2;
      fct_chk(sCarro.find(os.str()) != string::npos);
    }
    FCT_TEST_END();
  }
  FCT_FIXTURE_SUITE_END();
}
FCT_END()
