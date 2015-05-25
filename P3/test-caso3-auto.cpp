/* $Id: test-caso3-auto.cpp 232 2015-04-26 11:41:11Z gerardo $
 * Pruebas unitarias automáticas del caso 3 de la SGL.
 * ©2015 Los profesores de POO
 */
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <memory>
#include <locale>
#include <type_traits>
#include <regex.h>		// Lamentablemente, aún <regex> no funciona bien
#include "../P1/fct.h"
#include "tarjeta.h"
#include "usuario.h"
#include "articulo.h"
#include "pedido.h"
#include "usuario-pedido.h"
#include "pedido-articulo.h"

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

#define chk_incl_str(haystack, needle)                          \
  fct_xchk(strstr(haystack.c_str(), needle.c_str()) != NULL,    \
           "failed chk_incl_str:\n'%s' not in '%s'",            \
           needle.c_str(), haystack.c_str())

#define chk_incl_cstr(haystack, needle)                         \
  fct_xchk(strstr(haystack.c_str(), needle) != NULL,            \
           "failed chk_incl_cstring:\n'%s' not in '%s'",        \
           (const char*)needle, haystack.c_str())

// Esto es para eliminar el separador de decimales, porque da problemas con
// algunas localizaciones españolas, que incorrectamente ponen el ".". También
// de paso fijamos el separador de decimales a la coma.
struct sin_separador : numpunct<char> {
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
string toString(const T& o) {
  ostringstream os;
  os.imbue(locale(locale("es_ES.UTF-8"), new sin_separador()));
  os << o;
  return os.str();
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

/**
  Función que cambia la fecha de caducidad de una tarjeta.

  Esto es un "hack" protagonizado por el "hacker" Gerardo :-P
  para evitar el tener que declarar la función
  caducar() como amiga de la clase Tarjeta, ya que ni eso tiene
  sentido ni se dice en el enunciado que se toque dicha clase para hacer
  ese cambio. Solo se utiliza para probar un pago con una tarjeta
  caducada, que por cierto, ya hay que ser cierrabares y pudrecolchones para
  intentar pagar con una tarjeta caducada.
*/
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
    pst->la_caducidad = f;	    // Y aquí se comete el pecado. }-)
}

/**
   Clase de excepción para expresiones regulares no válidas.
 */
class BadRegex : public exception {
public:
  BadRegex(const char* regex) : regex_(regex) {}
  const char* regex() const { return regex_; }
private:
  const char* regex_;
};

/**
   Lamentablemente, aún <regex>, de la biblioteca estándar de C++11,
   no funciona bien: muy mal en GCC C++ 4.8 y algo menos, pero no bien,
   en GCC C++ 4.9. Y Clang++ usa la misma biblioteca.

   Función que busca una expresión regular dentro de una cadena y
   devuelve la posición del comienzo de la primera
   coincidencia. Devuelve -1 cuando no encuentra ninguna. Lanza la
   excepción BadRegex cuando la expresion regular no es válida.
 */

regoff_t find_regex(const char* regex, const char* text) throw (BadRegex) {
  regex_t reg;
  if (regcomp(&reg, regex, REG_EXTENDED) != 0)
    throw BadRegex(regex);

  regmatch_t matches[1] = {{-1, -1}};
  regexec(&reg, text, 1, matches, 0);
  regfree(&reg);

  return matches[0].rm_so;
}

FCT_BGN() {

  // Establecemos la localización por omisión a la española de España
  // con UTF-8, para asegurarnos de que las pruebas funcionen como
  // deben. Si no, se mezclarán los separadores decimales ingleses
  // ('.') y españoles (',').
  locale::global(locale("es_ES.UTF-8"));

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
  }
  FCT_SUITE_END();

  // Variables para el siguiente conjunto de casos de prueba. ¡No tocar!
  const Cadena referencia("1234XYZ");
  const Cadena titulo("Prueba");
  const Fecha    fecha(10, 10, 2000);
  const double   precio = 20.4;
  const unsigned stock = 30;
  Articulo *pA = nullptr;

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
      fct_chk_eq_str(toString(*pA).c_str(),
                         "[1234XYZ] \"Prueba\", 2000. 20,40 €");
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
                     fHoy, 42.10, 50);
  Articulo articulo2("110", "Fundamentos de C++",
                     fHoy, 35.95, 50);
  Usuario* pU = nullptr;

  FCT_FIXTURE_SUITE_BGN(Usuario + Tarjeta + Articulo) {
    FCT_SETUP_BGN() {
      pU = new Usuario(sId, sNombre, sApellidos, sDireccion, clave);
    }
    FCT_SETUP_END();

    FCT_TEARDOWN_BGN() {
      if (pU) delete pU;
    }
    FCT_TEARDOWN_END();

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
      pU = nullptr;
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
        fct_chk(pU->compra().begin()->first->referencia() ==
		articulo1.referencia());
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
      chk_incl_str(sCarro, sId);
      chk_incl_cstr(sCarro, "Artículos: 2");

      os.str("");
      os << articulo1;
      chk_incl_str(sCarro, os.str());

      os.str("");
      os << articulo2;
      chk_incl_str(sCarro, os.str());
    }
    FCT_TEST_END();
  }
  FCT_FIXTURE_SUITE_END();

// P3

  FCT_SUITE_BGN(LineaPedido) {
    FCT_TEST_BGN(LINEA_PEDIDO - valor por defecto en 2do parametro constructor){
      const double pVenta = 10.5;
      const LineaPedido lp(pVenta);
      fct_chk_eq_dbl(lp.precio_venta(), pVenta);
      fct_chk_eq_int(lp.cantidad(), 1);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(LINEA_PEDIDO - constructor 2 parametros) {
      const double pVenta = 7.3;
      const unsigned cantidad = 5;
      const LineaPedido lp(pVenta, cantidad);
      fct_chk_eq_dbl(lp.precio_venta(), pVenta);
      fct_chk_eq_int(lp.cantidad(), 5);
//      fct_chk_eq_dbl(lp.cantidad(), double(cantidad));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(LINEA_PEDIDO - insercion flujo) {
      const LineaPedido lp(9.2, 42);
      fct_chk_eq_str(toString(lp).c_str(), "9,20 €\t42");
    }
    FCT_TEST_END();
  }
  FCT_SUITE_END();

  // Este conjunto de casos de prueba reutiliza alguna de las
  // variables de los anteriores. No debe cambiarse de orden respecto
  // al resto de conjunto de casos de prueba.
  Usuario_Pedido  *pAsocUsuarioPedido;
  Pedido_Articulo *pAsocPedidoArticulo;
  Usuario         *pU2;
  Tarjeta         *pTarjetaU;
  const Tarjeta   *pTarjetaU2;
  const Fecha     fAyer = fHoy - 1;

  // El éxito de las pruebas depende de su orden. ¡No reordenar!
  FCT_FIXTURE_SUITE_BGN(Creación de pedidos) {
    FCT_SETUP_BGN() {
        pAsocUsuarioPedido  = new Usuario_Pedido();
        pAsocPedidoArticulo = new Pedido_Articulo();
        pU  = new Usuario(sId, sNombre, sApellidos, sDireccion, clave);
        pU2 = new Usuario("u2", "Mario", "Segali", "Mundo 1-1", "jumpman");
        pTarjetaU  = new Tarjeta(nTarjeta, *pU, fUnaSemana);
//        pTarjetaU2 = new Tarjeta(nTarjeta3, *const_cast<Usuario*>(pU2), fUnaSemana);
        pTarjetaU2 = new Tarjeta(nTarjeta3, *pU2, fUnaSemana);
    }
    FCT_SETUP_END();

    FCT_TEARDOWN_BGN() {
      delete pTarjetaU2;
      delete pTarjetaU;
      delete pU2;
      delete pU;
      delete pAsocPedidoArticulo;
      delete pAsocUsuarioPedido;
    }
    FCT_TEARDOWN_END();

    FCT_TEST_BGN(PEDIDO - carrito vacio) {
      try {
        Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
	       *pU, *pTarjetaU, fHoy);
        fct_chk(!"Se esperaba una excepción");
      }
      catch(const Pedido::Vacio& ex) {
        fct_chk(&ex.usuario() == pU);
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(PEDIDO - impostor) {
      pU2->compra(articulo1, 3);
      try {
        Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
               */*const_cast<Usuario*>*/(pU2), *pTarjetaU, fHoy);
        fct_chk(!"Se esperaba una excepción");
      }
      catch(const Pedido::Impostor& ex) {
        fct_chk(&ex.usuario() == pU2);
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(PEDIDO - sin stock) {
      pU->compra(articulo1, 9001);
      try {
        Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
	       *pU, *pTarjetaU, fHoy);
        fct_chk(!"Se esperaba una excepción");
      }
      catch (const Pedido::SinStock& ex) {
        fct_chk(&ex.articulo() == &articulo1);
        fct_chk(pU->compra().empty());
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(PEDIDO - tarjeta caducada) {
      pU->compra(articulo1, 3);//4649
      caducar(*pTarjetaU, fAyer);
      try {
        Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
	       *pU, *pTarjetaU, fHoy);
        fct_chk(!"Se esperaba una excepción");
      }
      catch (const Tarjeta::Caducada& ex) {}
    }
    FCT_TEST_END();

    FCT_TEST_BGN(PEDIDO - actualizacion relacionados y asociaciones) {
      const unsigned cantidad = 1;
      pU->compra(articulo1, cantidad);
      pU->compra(articulo2, cantidad);
      const auto_ptr<const Pedido> pPed(
          new Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
		     *pU, *pTarjetaU, fHoy));

      // Actualización de carrito y stock
      fct_chk(pU->compra().empty());
      fct_chk_eq_int(articulo1.stock(), 49);
      fct_chk_eq_int(articulo2.stock(), 49);

      // Asociación Usuario-Pedido
      fct_chk(pAsocUsuarioPedido->cliente(*const_cast<Pedido*>(pPed.get()))
	      == pU);
      if (pAsocUsuarioPedido->pedidos(*pU).size() == 1) {
        fct_chk(*pAsocUsuarioPedido->pedidos(*pU).begin() == pPed.get());
      }
      else {
        fct_chk(!"Debería asociarse al usuario con el pedido");
      }

      // Asociación Artículo-Pedido
      const Pedido_Articulo::ItemsPedido itPed
        = pAsocPedidoArticulo->detalle(* const_cast<Pedido*>(pPed.get()));
      if (itPed.size() == 2) {
        // Los artículos deben ir ordenados por código de referencia
        Pedido_Articulo::ItemsPedido::const_iterator it = itPed.begin();
        fct_chk_eq_int(it->second.cantidad(), cantidad);
        fct_chk_eq_dbl(it->second.precio_venta(), articulo2.precio());

        ++it;
        fct_chk_eq_int(it->second.cantidad(), cantidad);
        fct_chk_eq_dbl(it->second.precio_venta(), articulo1.precio());
      }
      else {
        fct_chk(!"El pedido debería tener dos artículos");
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(PEDIDO - observadores) {
      pU->compra(articulo1, 1);
      pU->compra(articulo2, 1);
      const double totalEsperado = articulo1.precio() + articulo2.precio();
      const unique_ptr<const Pedido> pPed(
          new Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
		     *pU, *pTarjetaU, fHoy));

      fct_chk_eq_int(pPed->numero(), 2);
      fct_chk(pPed->tarjeta() == pTarjetaU);
      fct_chk(pPed->fecha() == fHoy);
      fct_chk_eq_dbl(pPed->total(), totalEsperado);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(PEDIDO - insercion en flujo) {
      pU->compra(articulo1, 1);
      pU->compra(articulo2, 1);
      const double totalEsperado = articulo1.precio() + articulo2.precio();
      const unique_ptr<const Pedido> pPed(
          new Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
		     *pU, *pTarjetaU, fHoy));

      const string sPed = toString(*pPed);
      chk_incl_cstr(sPed, "Núm. pedido:");
      chk_incl_cstr(sPed, "Fecha:");
      chk_incl_cstr(sPed, "Pagado con:");
      chk_incl_cstr(sPed, "Importe:");
      chk_incl_str(sPed, toString(pPed->numero()));
      chk_incl_cstr(sPed, pPed->fecha().cadena());
      chk_incl_cstr(sPed, pTarjetaU->numero());
      chk_incl_str(sPed, toEuros(totalEsperado));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(ARTICULO<->PEDIDO - detalle de un pedido) {
      pU->compra(articulo1, 1);
      const unique_ptr<const Pedido> pPed(
          new Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
		     *pU, *pTarjetaU, fHoy));
      const Pedido_Articulo::ItemsPedido detalle
        = pAsocPedidoArticulo->detalle(*const_cast<Pedido*>(pPed.get()));
      if (detalle.size() == 1) {
        const auto& it = detalle.cbegin();
        const LineaPedido& lp = detalle.cbegin()->second;
        fct_chk(it->first == &articulo1);
        fct_chk_eq_dbl(lp.precio_venta(), articulo1.precio());
        fct_chk_eq_int(lp.cantidad(), 1);
      }
      else {
        fct_chk(!"El pedido debería tener un solo elemento");
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(ARTICULO<->PEDIDO - insercion en flujo de ItemsPedido) {
      const unsigned cantidad = 1;
      pU->compra(articulo1, cantidad);
      const unique_ptr<const Pedido> pPed(
          new Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
		     *pU, *pTarjetaU, fHoy));
      const Pedido_Articulo::ItemsPedido detalle
        = pAsocPedidoArticulo->detalle(*const_cast<Pedido*>(pPed.get()));

      const string sDetalle = toString(detalle);
      chk_incl_str(sDetalle, toString(pPed->total()));
      chk_incl_str(sDetalle, toString(cantidad));
    }
    FCT_TEST_END();
  }
  FCT_FIXTURE_SUITE_END();

  /// Batería de pruebas para los informes. Simula dos ventas de dos usuarios.
  const Pedido *pPed1, *pPed2;
  const unsigned cantidad_A1_P1 = 1;
  const unsigned cantidad_A1_P2 = 3;
  const unsigned cantidad_A2_P2 = 5;

  FCT_FIXTURE_SUITE_BGN(Dos compras de dos usuarios) {

    FCT_SETUP_BGN() {
      pAsocUsuarioPedido  = new Usuario_Pedido();
      pAsocPedidoArticulo = new Pedido_Articulo();
      pU  = new Usuario(sId, sNombre, sApellidos, sDireccion, clave);
      pU2 = new Usuario("u2", "Mario", "Segali", "Mundo 1-1", "jumpman");
      pTarjetaU  = new Tarjeta(nTarjeta, *pU, fUnaSemana);
      pTarjetaU2 = new Tarjeta(nTarjeta3, *const_cast<Usuario*>(pU2),
			       fUnaSemana);
      // Primera venta
      pU->compra(articulo1, cantidad_A1_P1);
      pPed1 = new Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
			 *pU, *pTarjetaU, fHoy);

      // Segunda venta, de otro usuario
      pU2->compra(articulo1, cantidad_A1_P2);
      pU2->compra(articulo2, cantidad_A2_P2);
      pPed2 = new Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
                         */*const_cast<Usuario*>*/(pU2), *pTarjetaU2, fHoy);
    }
    FCT_SETUP_END();

    FCT_TEARDOWN_BGN() {
      delete pPed2;
      delete pPed1;
      delete pTarjetaU2;
      delete pTarjetaU;
      delete pU2;
      delete pU;
      delete pAsocPedidoArticulo;
      delete pAsocUsuarioPedido;
    }
    FCT_TEARDOWN_END();

    FCT_TEST_BGN(ARTICULO<->PEDIDO - ventas de un articulo) {
      const auto& pedArticulo1 = pAsocPedidoArticulo->ventas(articulo1);
      if (pedArticulo1.size() == 2) {
        auto it = pedArticulo1.find(const_cast<Pedido*>(pPed1));
        fct_chk_eq_dbl(it->second.precio_venta(), articulo1.precio());
        fct_chk_eq_int(it->second.cantidad(), cantidad_A1_P1);

        ++it;
        fct_chk_eq_dbl(it->second.precio_venta(), articulo1.precio());
        fct_chk_eq_int(it->second.cantidad(), cantidad_A1_P2);
      }
      else {
        fct_chk(!"El artículo 1 debería tener dos ventas");
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(ARTICULO<->PEDIDO - insercion en flujo de Pedidos) {
      const auto& pedArticulo1 = pAsocPedidoArticulo->ventas(articulo1);
      const string sPedidos = toString(pedArticulo1);

      chk_incl_str(sPedidos, toString(articulo1.precio()));
      chk_incl_str(sPedidos, toString(cantidad_A1_P1));
      chk_incl_cstr(sPedidos, pPed1->fecha().cadena());
      chk_incl_str(sPedidos, toString(cantidad_A1_P2));
      chk_incl_cstr(sPedidos, pPed2->fecha().cadena());

      const double totalEsperado = (cantidad_A1_P1 + cantidad_A1_P2)
	* articulo1.precio();
      chk_incl_str(sPedidos, toEuros(totalEsperado));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(ARTICULO<->PEDIDO - mostrar detalle pedidos) {
      ostringstream os;
      pAsocPedidoArticulo->mostrarDetallePedidos(os);
      const string sDetalle = os.str();

      const double totalEsperado
        = (cantidad_A1_P1 + cantidad_A1_P2) * articulo1.precio()
        + cantidad_A2_P2 * articulo2.precio();

      chk_incl_str(sDetalle, toEuros(totalEsperado));
      chk_incl_str(sDetalle, articulo1.titulo());
      chk_incl_str(sDetalle, articulo2.titulo());
      chk_incl_str(sDetalle, toString(cantidad_A1_P1));
      chk_incl_str(sDetalle, toString(cantidad_A1_P2));
      chk_incl_str(sDetalle, toString(cantidad_A2_P2));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(ARTICULO<->PEDIDO - mostrar ventas agrupadas) {
      ostringstream os;
      pAsocPedidoArticulo->mostrarVentasArticulos(os);
      const string sDetalle = os.str();

      // Construimos expresiones regulares que buscan las cantidades
      // como palabras separadas del resto por espacios. Es decir, si
      // la cantidad es 1, ' 1' a final de línea, '1 ' a principio de
      // línea, o ' 1 ' valen, pero no '123', por ejemplo.
      const string sRegexPrefijo = "€[[:space:]]+";
      const string sRegexSufijo  = "([[:space:]]|$)";
      const string sRegexA1P1
        = sRegexPrefijo + toString(cantidad_A1_P1) + sRegexSufijo;
      const string sRegexA1P2
        = sRegexPrefijo + toString(cantidad_A1_P2) + sRegexSufijo;
      const string sRegexA2P2
        = sRegexPrefijo + toString(cantidad_A2_P2) + sRegexSufijo;

      const regoff_t posCantidad_A1_P1
        = find_regex(sRegexA1P1.c_str(), sDetalle.c_str());
      const regoff_t posCantidad_A1_P2
        = find_regex(sRegexA1P2.c_str(), sDetalle.c_str());
      const regoff_t posCantidad_A2_P2
        = find_regex(sRegexA2P2.c_str(), sDetalle.c_str());

      // Las tres compras deben estar en la salida
      fct_chk(posCantidad_A1_P1 != -1);
      fct_chk(posCantidad_A1_P2 != -1);
      fct_chk(posCantidad_A2_P2 != -1);

      /*
        Las compras deben estar agrupadas por artículo, y debidamente
        ordenadas.
      */
      fct_chk(posCantidad_A2_P2 < posCantidad_A1_P1);
      fct_chk(posCantidad_A1_P1 < posCantidad_A1_P2);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(PEDIDO<->USUARIO - pedidos de un usuario) {
      const auto& pedidosU2 = pAsocUsuarioPedido->pedidos(*pU2);
      if (pedidosU2.size() == 1) {
        fct_chk(*pedidosU2.begin() == pPed2);
      } else {
        fct_chk(!"Debería haber 1 pedido registrado del usuario 2");
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(PEDIDO<->USUARIO - usuario de los pedidos) {
      fct_chk(pAsocUsuarioPedido->cliente(*const_cast<Pedido*>(pPed1)) == pU);
      fct_chk(pAsocUsuarioPedido->cliente(*const_cast<Pedido*>(pPed2)) == pU2);
    }
    FCT_TEST_END();
  }
  FCT_FIXTURE_SUITE_END();

}
FCT_END()
