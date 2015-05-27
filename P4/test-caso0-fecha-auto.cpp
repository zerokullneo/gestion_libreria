/* $Id: test-caso0-fecha-auto.cpp 247 2015-05-20 08:38:11Z gerardo $
 * ©2014-15 Antonio G.ª Dguez. & el resto de profesores de POO
 */

#include <ctime>
#include <iostream>
#include <sstream>
#include "../P2/fct.h"

#include "fecha.h"

using namespace std;

#define fecha_is_eq(f, d, m, y)\
  (f.dia() == d && f.mes() == m && f.anno() == y)

#define chk_eq_fecha(f, d, m, y)\
  fct_xchk(\
    fecha_is_eq(f,d,m,y),\
    "failed chk_eq_fecha:\n<%02d/%02d/%04d>!=<%02d/%02d/%04d>",\
    f.dia(), f.mes(), f.anno(), d, m, y)

namespace {
  int annoSistema, mesSistema, diaSistema;
  bool bPrimera = true;
}

FCTMF_FIXTURE_SUITE_BGN(test_fecha) {
  if (bPrimera) {
    bPrimera = false;
    cerr << "\n---------- PRÁCTICA 1: FECHA ----------\n" << endl;
  }

  FCT_SETUP_BGN() {
    // 1.º establecemos la zona horaria a UTC para evitar cosas raras
    setenv("TZ", "", 1);
    tzset();
    const time_t ahora = time(nullptr);
    const tm* fecha = localtime(&ahora);
    annoSistema = 1900 + fecha->tm_year;
    mesSistema = 1 + fecha->tm_mon;
    diaSistema = fecha->tm_mday;
  }
  FCT_SETUP_END();

  FCT_TEARDOWN_BGN() {
  }
  FCT_TEARDOWN_END();

  FCT_TEST_BGN(Ctor: dia - mes - anyo) {
    const Fecha f(9, 10, 1999);
    chk_eq_fecha(f, 9, 10, 1999);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: dia - mes sin ceros) {
    const Fecha f(3, 7);
    chk_eq_fecha(f, 3, 7, annoSistema);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: dia - mes con ceros) {
    const Fecha f(3, 7, 0);
    chk_eq_fecha(f, 3, 7, annoSistema);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: dia sin ceros) {
    const Fecha f(2);
    chk_eq_fecha(f, 2, mesSistema, annoSistema);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: dia con un cero) {
    const Fecha f(2, 0);
    chk_eq_fecha(f, 2, mesSistema, annoSistema);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: dia con dos ceros) {
    const Fecha f(2, 0, 0);
    chk_eq_fecha(f, 2, mesSistema, annoSistema);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: sin parametros) {
    const Fecha f;
    chk_eq_fecha(f, diaSistema, mesSistema, annoSistema);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: todo ceros) {
    const Fecha f(0, 0, 0);
    chk_eq_fecha(f, diaSistema, mesSistema, annoSistema);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: copia) {
    const Fecha f(2, 3, 2000);
    const Fecha g(f);
    chk_eq_fecha(f, g.dia(), g.mes(), g.anno());
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: cadena 2/2/4) {
    const Fecha f("10/12/2011");
    chk_eq_fecha(f, 10, 12, 2011);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: cadena 1/1/4) {
    const Fecha f = "1/5/2010";
    chk_eq_fecha(f, 1, 5, 2010);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: cadena 1/1/0) {
    const Fecha f = "1/5/0";
    chk_eq_fecha(f, 1, 5, annoSistema);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: cadena 1/0/4) {
    const Fecha f = "1/0/2010";
    chk_eq_fecha(f, 1, mesSistema, 2010);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: cadena 0/2/4) {
    const Fecha f = "0/11/2010";
    chk_eq_fecha(f, diaSistema, 11, 2010);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: cadena 2/0/0) {
    const Fecha f = "1/0/0";
    chk_eq_fecha(f, 1, mesSistema, annoSistema);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: cadena 0/0/0) {
    const Fecha f = "0/0/0";
    chk_eq_fecha(f, diaSistema, mesSistema, annoSistema);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: dia no valido) {
    try {
      const Fecha f(31, 6, 2009);
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: dia valido para bisiesto) {
    try {
      const Fecha f(29, 2, 2004);
      fct_chk(true);
    } catch(Fecha::Invalida) {
      fct_chk(!"No se esperaba una excepción");
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: dia no valido ni para bisiesto) {
    try {
      const Fecha f(30, 2, 2004);
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: mes no valido) {
    try {
      const Fecha f(1, -1, 2020);
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: anno no valido) {
    try {
      const Fecha f(1, 1, -1);
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor cadena: dia no valido) {
    try {
      const Fecha f("31/6/2009");
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor cadena: mes no valido) {
    try {
      const Fecha f("1/13/2020");
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor cadena: anno no valido) {
    try {
      const Fecha f("1/1/-1");
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Ctor: cadena incorrecta) {
    try {
      const Fecha f("31-1-2000");
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Preincremento) {
    Fecha f(31, 3, 2010);
    Fecha g(++f);
    chk_eq_fecha(f, 1, 4, 2010);
    chk_eq_fecha(g, 1, 4, 2010);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Preincremento - asociatividad) {
    Fecha f(31, 3, 2010);
    Fecha g(++++f);
    chk_eq_fecha(f, 2, 4, 2010);
    chk_eq_fecha(g, 2, 4, 2010);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Postincremento) {
    Fecha f(31, 3, 2010);
    Fecha g(f++);
    chk_eq_fecha(f, 1, 4, 2010);
    chk_eq_fecha(g, 31, 3, 2010);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Predecremento) {
    Fecha f(1, 1, 2010);
    Fecha g(--f);
    chk_eq_fecha(f, 31, 12, 2009);
    chk_eq_fecha(g, 31, 12, 2009);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Predecremento - asociatividad) {
    Fecha f(1, 1, 2010);
    Fecha g(----f);
    chk_eq_fecha(f, 30, 12, 2009);
    chk_eq_fecha(g, 30, 12, 2009);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Postdecremento) {
    Fecha f(1, 1, 2010);
    Fecha g(f--);
    chk_eq_fecha(f, 31, 12, 2009);
    chk_eq_fecha(g, 1, 1, 2010);
  }
  FCT_TEST_END();

//PRUEBA ANULADA: día + Fecha.
#if 0
  FCT_TEST_BGN(Suma de dias - dias + fecha) {
      const Fecha f(30, 4, 2003);
      const Fecha g(10 + f);
      chk_eq_fecha(g, 10, 5, 2003);
  }
  FCT_TEST_END();
#endif

  FCT_TEST_BGN(Resta de dias - fecha - dias) {
    const Fecha f(1, 4, 2008);
    const Fecha g(f - 10);
    chk_eq_fecha(g, 22, 3, 2008);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Suma con asignacion) {
    Fecha f(5, 5, 2005);
    f += 3;
    chk_eq_fecha(f, 8, 5, 2005);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Suma con asignacion - asociatividad) {
    Fecha f(5, 5, 2005);
    (f += 3) += 2;
    chk_eq_fecha(f, 10, 5, 2005);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Suma con asignacion: fecha fuera de rango) {
    try {
      Fecha f(31, 12, Fecha::AnnoMaximo);
      f += 2;
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Resta con asignacion) {
    Fecha f(2, 7, 2010);
    f -= 2;
    chk_eq_fecha(f, 30, 6, 2010);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Resta con asignacion - asociatividad) {
    Fecha f(2, 7, 2010);
    (f -= 2) -= 5;
    chk_eq_fecha(f, 25, 6, 2010);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Resta con asignacion: fecha fuera de rango) {
    try {
      Fecha f(1, 1, Fecha::AnnoMinimo);
      f -= 2;
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Asignacion entre fechas) {
    const Fecha f(1, 1, 2001);
    Fecha g(3, 4, 2005);
    g = f;
    chk_eq_fecha(g, 1, 1, 2001);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Asignacion entre fechas - asociatividad) {
    const Fecha f(1, 1, 2001);
    Fecha g(3, 4, 2005);
    Fecha h(g);
    h = g = f;
    chk_eq_fecha(h, 1, 1, 2001);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Conversion a cadena) {
    const Fecha f(12, 9, 2001);
    const char* s = f.cadena();
    fct_chk(strstr(s, "miércoles") != 0);
    fct_chk(strstr(s, "septiembre") != 0);
    fct_chk(strstr(s, "2001") != 0);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Comparacion: igualdad) {
    const Fecha f(1, 2, 2000);
    const Fecha g(f);
    const Fecha h(2, 3, 2001);
    fct_chk(f == g);
    fct_chk(!(f == h));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Comparacion: desigualdad) {
    const Fecha f(1, 2, 2000);
    const Fecha g(f);
    const Fecha h(2, 3, 2001);
    fct_chk(!(f != g));
    fct_chk(f != h);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Comparacion: mayor) {
    const Fecha f(10, 2, 2000);
    const Fecha g(15, 2, 2000);
    const Fecha h(20, 1, 2000);
    const Fecha i(10, 5, 2000);
    const Fecha j(30, 5, 1999);
    const Fecha k(1, 1, 2004);
    fct_chk(!(f > f));
    fct_chk(g > f);
    fct_chk(f > h);
    fct_chk(i > f);
    fct_chk(f > j);
    fct_chk(k > f);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Comparacion: mayor o igual) {
    const Fecha f(10, 2, 2000);
    const Fecha g(15, 2, 2000);
    const Fecha h(20, 1, 2000);
    const Fecha i(10, 5, 2000);
    const Fecha j(30, 5, 1999);
    const Fecha k(1, 1, 2004);
    fct_chk(f >= f);
    fct_chk(g >= f);
    fct_chk(f >= h);
    fct_chk(i >= f);
    fct_chk(f >= j);
    fct_chk(k >= f);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Comparacion: menor) {
    const Fecha f(10, 2, 2000);
    const Fecha g(15, 2, 2000);
    const Fecha h(20, 1, 2000);
    const Fecha i(10, 5, 2000);
    const Fecha j(30, 5, 1999);
    const Fecha k(1, 1, 2004);
    fct_chk(!(f < f));
    fct_chk(f < g);
    fct_chk(h < f);
    fct_chk(f < i);
    fct_chk(j < f);
    fct_chk(f < k);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Comparacion: menor o igual) {
    const Fecha f(10, 2, 2000);
    const Fecha g(15, 2, 2000);
    const Fecha h(20, 1, 2000);
    const Fecha i(10, 5, 2000);
    const Fecha j(30, 5, 1999);
    const Fecha k(1, 1, 2004);
    fct_chk(f <= f);
    fct_chk(f <= g);
    fct_chk(h <= f);
    fct_chk(f <= i);
    fct_chk(j <= f);
    fct_chk(f <= k);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Diferencia en dias entre 2 fechas distintas) {
    const Fecha f1(6, 2, 2015);
    const Fecha f2(29, 12, 1958);
    long int d1 = f1 - f2;
    long int d2 = f2 - f1;
    fct_chk(d1 == 20493l && d2 == -20493l);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Diferencia en dias entre 2 fechas muy distintas) {
    const Fecha f1(31, 12, Fecha::AnnoMaximo);
    const Fecha f2( 1,  1, Fecha::AnnoMinimo);
    long int d1 = f1 - f2;
    long int d2 = f2 - f1;
    // Dejamos un margen de ±1
    fct_chk((d1 >= 49672l && d1 <= 49674l) && (d2 <= -49672l && d2 >= -49674l));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Diferencia en dias entre 2 fechas iguales) {
    const Fecha f1(16, 3, 2013);
    const Fecha f2(f1);
    long int d = f1 - f2;
    fct_chk(d == 0l);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Extraccion: fecha valida) {
    istringstream is("01/01/2001");
    Fecha f;
    is >> f;
    chk_eq_fecha(f, 1, 1, 2001);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Extraccion: fecha no valida) {
    try {
      istringstream is("90/20/4000");
      Fecha f;
      is >> f;
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Extraccion: entrada no valida) {
    try {
      istringstream is("gomi");
      Fecha f;
      is >> f;
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Extraccion: desbordamiento) {
    try {
      istringstream is("007/007/1997");
      Fecha f;
      is >> f;
      fct_chk(!"Se esperaba una excepción");
    } catch (Fecha::Invalida) {
      fct_chk(true);
    }
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Insercion) {
    const Fecha f("23/09/2015");
    ostringstream os;
    os << f;
    string s = os.str();
    fct_chk("miércoles 23 de septiembre de 2015" == s);
  }
  FCT_TEST_END();
}
FCTMF_FIXTURE_SUITE_END()
