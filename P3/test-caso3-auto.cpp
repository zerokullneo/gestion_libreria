/* $Id: test-caso3-auto.cpp 255 2015-05-25 11:36:52Z gerardo $
 * ©2014 Antonio G.ª Dguez. y el resto de profesores de POO
 *
 */
#include <ctime>
#include <string>
#include <cmath>
#include <iostream>
#include <memory>
using namespace std;

#include "fct.h"
#include "test-auto.h"

#include "tarjeta.h"
#include "usuario.h"
#include "articulo.h"
#include "pedido.h"
#include "usuario-pedido.h"
#include "pedido-articulo.h"

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
    
    Usuario_Pedido  *pAsocUsuarioPedido;
    Pedido_Articulo *pAsocPedidoArticulo;
    Usuario         *pU2;
    Tarjeta         *pTarjetaU;
    const Tarjeta   *pTarjetaU2;
    const Fecha     fAyer = fHoy - 1;
    
    const Pedido *pPed1, *pPed2;
    const unsigned cantidad_A1_P1 = 1;
    const unsigned cantidad_A1_P2 = 3;
    const unsigned cantidad_A2_P2 = 5;

    bool bPrimera = true;
}

FCTMF_FIXTURE_SUITE_BGN(test_p3_clases) {

  if (bPrimera) {
    bPrimera = false;
    cerr << "\n---------- PRÁCTICA 3 ----------\n" << endl;
  }

  FCT_TEST_BGN(LINEA_PEDIDO - valor predeterminado en 2do. parametro constructor) {
    const double pVenta = 10.5;
    const LineaPedido lp(pVenta /*, 1 */);
    fct_chk_eq_dbl(lp.precio_venta(), pVenta);
    fct_chk_eq_int(lp.cantidad(), 1);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(LINEA_PEDIDO - constructor 2 parametros) {
    const double pVenta = 7.3;
    const unsigned cantidad = 5;
    const LineaPedido lp(pVenta, cantidad);
    fct_chk_eq_dbl(lp.precio_venta(), pVenta);
    fct_chk_eq_int(lp.cantidad(), cantidad);
    fct_chk_eq_dbl(lp.cantidad(), double(cantidad));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(LINEA_PEDIDO - insercion flujo) {
    const LineaPedido lp(9.2, 42);
    fct_chk_eq_str(toString(lp).c_str(), "9,20 €\t42");
  }
  FCT_TEST_END();

  // El éxito de las pruebas depende de su orden. ¡No reordenar!
  FCT_SETUP_BGN() {
    pAsocUsuarioPedido  = new Usuario_Pedido();
    pAsocPedidoArticulo = new Pedido_Articulo();
    pU  = new Usuario(sId, sNombre, sApellidos, sDireccion, clave);
    pU2 = new Usuario("u2", "Mario", "Segali", "Mundo 1-1", "jumpman");
    pTarjetaU  = new Tarjeta(nTarjeta, *pU, fUnaSemana);
    pTarjetaU2 = new Tarjeta(nTarjeta3, */*const_cast<Usuario*>*/(pU2), fUnaSemana);
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
      Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo, *pU, *pTarjetaU, fHoy);
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
      Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo, *pU, *pTarjetaU, fHoy);
      fct_chk(!"Se esperaba una excepción");
    }
    catch (const Pedido::SinStock& ex) {
      fct_chk(&ex.articulo() == &articulo1);
      fct_chk(pU->compra().empty());
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(PEDIDO - tarjeta caducada) {
    pU->compra(articulo1, 4649);
    caducar(*pTarjetaU, fAyer);
    try {
      Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo, *pU, *pTarjetaU, fHoy);
      fct_chk(!"Se esperaba una excepción");
    }
    catch (const Tarjeta::Caducada&) {
      fct_chk(true);
    }
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
    const unique_ptr<const Pedido> pPed(new Pedido(*pAsocUsuarioPedido, 
						   *pAsocPedidoArticulo, 
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
    const unique_ptr<const Pedido> pPed(new Pedido(*pAsocUsuarioPedido, 
						   *pAsocPedidoArticulo, 
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
    const unique_ptr<const Pedido> pPed(new Pedido(*pAsocUsuarioPedido, 
						   *pAsocPedidoArticulo, 
						   *pU, *pTarjetaU, fHoy));
    const Pedido_Articulo::ItemsPedido detalle
      = pAsocPedidoArticulo->detalle(*const_cast<Pedido*>(pPed.get()));
    if (detalle.size() == 1) {
      const Pedido_Articulo::ItemsPedido::const_iterator& it = detalle.begin();
      const LineaPedido& lp = detalle.begin()->second;
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
    const unique_ptr<const Pedido> pPed(new Pedido(*pAsocUsuarioPedido, 
						   *pAsocPedidoArticulo, 
						   *pU, *pTarjetaU, fHoy));
    const Pedido_Articulo::ItemsPedido detalle
      = pAsocPedidoArticulo->detalle(*const_cast<Pedido*>(pPed.get()));

    const string sDetalle = toString(detalle);
    chk_incl_str(sDetalle, toString(pPed->total()));
    chk_incl_str(sDetalle, toString(cantidad));
  }
  FCT_TEST_END();

  /// Batería de pruebas para los informes. Simula dos ventas de dos usuarios.
}
FCTMF_FIXTURE_SUITE_END()

FCTMF_FIXTURE_SUITE_BGN(test_p3_informes) {

  FCT_SETUP_BGN() {
    pAsocUsuarioPedido  = new Usuario_Pedido();
    pAsocPedidoArticulo = new Pedido_Articulo();
    pU  = new Usuario(sId, sNombre, sApellidos, sDireccion, clave);
    pU2 = new Usuario("u2", "Mario", "Segali", "Mundo 1-1", "jumpman");
    pTarjetaU  = new Tarjeta(nTarjeta, *pU, fUnaSemana);
    pTarjetaU2 = new Tarjeta(nTarjeta3, */*const_cast<Usuario*>*/(pU2), fUnaSemana);

    // Primera venta
    pU->compra(articulo1, cantidad_A1_P1);
    pPed1 = new Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo, 
		       *pU, *pTarjetaU, fHoy);

    // Segunda venta, de otro usuario
    pU2->compra(articulo1, cantidad_A1_P2);
    pU2->compra(articulo2, cantidad_A2_P2);
    pPed2 = new Pedido(*pAsocUsuarioPedido, *pAsocPedidoArticulo,
                       *const_cast<Usuario*>(pU2), *pTarjetaU2, fHoy);
  };
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
    const Pedido_Articulo::Pedidos& pedArticulo1 = 
	pAsocPedidoArticulo->ventas(articulo1);
    if (pedArticulo1.size() == 2) {
      Pedido_Articulo::Pedidos::const_iterator it = 
	  pedArticulo1.find(const_cast<Pedido*>(pPed1));
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
    const Pedido_Articulo::Pedidos& pedArticulo1 = 
	pAsocPedidoArticulo->ventas(articulo1);
    const string sPedidos = toString(pedArticulo1);

    chk_incl_str(sPedidos, toString(articulo1.precio()));
    chk_incl_str(sPedidos, toString(cantidad_A1_P1));
    chk_incl_cstr(sPedidos, pPed1->fecha().cadena());
    chk_incl_str(sPedidos, toString(cantidad_A1_P2));
    chk_incl_cstr(sPedidos, pPed2->fecha().cadena());

    const double totalEsperado = (cantidad_A1_P1 + cantidad_A1_P2) * 
	articulo1.precio();
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
    const string sRegexPrefijo = "(€[[:space:]]|^)";
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
    const Usuario_Pedido::Pedidos& pedidosU2 = 
      pAsocUsuarioPedido->pedidos(*pU2);
    if (pedidosU2.size() == 1)
      fct_chk(*pedidosU2.begin() == pPed2);
    else
      fct_chk(!"Debería haber 1 pedido registrado del usuario 2");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(PEDIDO<->USUARIO - usuario de los pedidos) {
    fct_chk(pAsocUsuarioPedido->cliente(*const_cast<Pedido*>(pPed1)) == pU);
    fct_chk(pAsocUsuarioPedido->cliente(*const_cast<Pedido*>(pPed2)) == pU2);
  }
  FCT_TEST_END();
}
FCTMF_FIXTURE_SUITE_END()
