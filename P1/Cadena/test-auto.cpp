// 	$Id: test-auto.cpp 206 2015-03-13 21:19:58Z gerardo $
/* Pruebas automáticas de la clase Cadena para P1 y ss.
 * ©Antonio G.ª Dguez.
 * ©Los profesores de POO
 */
#include "cadena.h"
#include "fct.h"
#include <cstdlib>
#include <sstream>
#include <stdexcept>
using namespace std;

FCT_BGN() {
  FCT_SUITE_BGN(Cadena) {
    FCT_TEST_BGN(Ctor: repetir caracter ciertas veces) {
      const Cadena a(5, '*');
      fct_chk(strcmp(a.c_str(), "*****") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Ctor: entero - repite espacios) {
      const Cadena a(10);
      fct_chk(strcmp(a.c_str(), "          ") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Ctor: sin parametros - vacia) {
      const Cadena a;
      fct_chk(strcmp(a.c_str(), "") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Ctor: por copia de otra cadena) {
      const Cadena a(3);
      const Cadena b(a);
      fct_chk(strcmp(b.c_str(), "   ") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Ctor: por copia de otra sub-Cadena) {
      const Cadena a("Casa de muchos, casa de sucios");
      const Cadena b(a, 16, 4);
      const Cadena c(a, 16);
      const Cadena d(a, 0 /*, Cadena::npos */);
      const Cadena e(a, 0, 0);
      fct_chk(strcmp(b.c_str(), "casa") == 0);
      fct_chk(strcmp(c.c_str(), "casa de sucios") == 0);
      fct_chk(strcmp(d.c_str(), a.c_str()) == 0);
      fct_chk(strcmp(e.c_str(), "") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Ctor: de movimiento) {
      Cadena a(3);
      const char* c = a.c_str();
      const Cadena b(static_cast<Cadena&&>(a));
      cout << endl << "--" << b.c_str() << "--|" << "--" << c << "--|" << "--" << a.c_str() << "--" << endl;
      fct_chk(b.c_str() == c);
      fct_chk(strcmp(b.c_str(), "   ") == 0);
      fct_chk(strcmp(a.c_str(), "   ") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Ctor: desde cadena de bajo nivel) {
      const Cadena a("hola");
      fct_chk(strcmp(a.c_str(), "hola") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Ctor: N primeros caracteres de cadena de C) {
      const Cadena a("No te digo trigo por no llamarte Rodrigo", 16);
      const Cadena b("Apetekawn", 0);
      const Cadena c("Mary had a little lamb", 30);
      fct_chk(strcmp(a.c_str(), "No te digo trigo") == 0);
      fct_chk(strcmp(b.c_str(), "") == 0);
      fct_chk(strcmp(c.c_str(), "Mary had a little lamb") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Asignar una cadena a otra) {
      Cadena a("adios");
      const Cadena b;
      a = b;
      fct_chk(strcmp(a.c_str(), b.c_str()) == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Asignar una cadena a otra: movimiento) {
      Cadena a("hola");
      Cadena b("adios");
      const char* c = b.c_str();
      a = static_cast<Cadena&&>(b);
      fct_chk(a.c_str() == c);
      fct_chk(strcmp(a.c_str(), "adios") == 0);
      fct_chk(strcmp(b.c_str(), "") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Asignar una cadena de bajo nivel) {
      Cadena a;
      a = "lembas";
      fct_chk(strcmp(a.c_str(), "lembas") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Observadora de longitud: cadena vacia) {
      const Cadena a;
      fct_chk(a.length() == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Observadora de longitud: cadena no vacia) {
      const char *s = "saludos cordiales";
      const Cadena a(s);
      fct_chk(a.length() == strlen(s));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Concatenacion: con +=) {
      Cadena a("cram");
      a += Cadena(" y lembas");
      fct_chk(strcmp(a.c_str(), "cram y lembas") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Concatenacion: con +) {
      const Cadena a("Cada cual,"), b(" en su corral");
      Cadena c = a + b;
      fct_chk(strcmp((a + b).c_str(), "Cada cual, en su corral") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Comparacion: igualdad) {
      fct_chk(Cadena("hola") == Cadena("hola"));
      fct_chk(!(Cadena("hola") == Cadena("adios")));
      fct_chk(!(Cadena("hola") == Cadena("holas")));
      fct_chk(!(Cadena("holas") == Cadena("hola")));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Comparacion: desigualdad) {
      fct_chk(!(Cadena("hola") != Cadena("hola")));
      fct_chk(Cadena("hola")  != Cadena("adios"));
      fct_chk(Cadena("hola")  != Cadena("holas"));
      fct_chk(Cadena("holas") != Cadena("hola"));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Comparacion: menor que) {
      fct_chk(Cadena("") < Cadena("x"));
      fct_chk(Cadena("a") < Cadena("b"));
      fct_chk(!(Cadena("ab") < Cadena("ab")));
      fct_chk(!(Cadena("ca") < Cadena("aa")));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Comparacion: menor o igual que) {
      fct_chk(Cadena("") <= Cadena("x"));
      fct_chk(Cadena("a") <= Cadena("b"));
      fct_chk(Cadena("ab") <= Cadena("ab"));
      fct_chk(!(Cadena("ca") <= Cadena("aa")));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Comparacion: mayor que) {
      fct_chk(!(Cadena("") > Cadena("x")));
      fct_chk(!(Cadena("a") > Cadena("b")));
      fct_chk(!(Cadena("ab") > Cadena("ab")));
      fct_chk(Cadena("ca") > Cadena("aa"));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Comparacion: mayor o igual que) {
      fct_chk(!(Cadena("") >= Cadena("x")));
      fct_chk(!(Cadena("a") >= Cadena("b")));
      fct_chk(Cadena("ab") >= Cadena("ab"));
      fct_chk(Cadena("ca") >= Cadena("aa"));
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Leer caracter: [] y posicion valida) {
      const Cadena a("abcd");
      fct_chk(a.operator[](0) == 'a');
      fct_chk(a.operator[](3) == 'd');
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Leer caracter: [] y posicion no valida) {
      const Cadena a;
      a.operator[](0);
      fct_chk(true);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Leer caracter: at y posicion valida) {
      const Cadena a("abcd");
      fct_chk(a.at(0) == 'a');
      fct_chk(a.at(3) == 'd');
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Leer caracter: at y posicion no valida) {
      const Cadena a;
      try {
        a.at(0);
        fct_chk(!"Se esperaba excepción");
      } catch (out_of_range) {
        fct_chk(true);
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Escribir caracter: []) {
      Cadena a("abcd");
      a.operator[](2) = 'x';
      fct_chk(strcmp(a.c_str(), "abxd") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Escribir caracter: at()) {
      Cadena a("abcd");
      a.at(2) = 'x';
      fct_chk(strcmp(a.c_str(), "abxd") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Subcadena: caso normal) {
      const Cadena a("eenie meenie moe");
      fct_chk(strcmp(a.substr(6,6).c_str(), "meenie") == 0);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Subcadena: posicion inicial mayor que longitud) {
      const Cadena a("0123456789");
      try {
        a.substr(10, 10);
        fct_chk(!"Se esperaba excepción");
      } catch (out_of_range) {
        fct_chk(true);
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Subcadena: posicion inicial negativa) {
      const Cadena a("0123456789");
      try {
        a.substr(-1, 10);
        fct_chk(!"Se esperaba excepción");
      } catch (out_of_range) {
        fct_chk(true);
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Subcadena: tama~no mayor que longitud restante) {
      const Cadena a("0123456789");
      try {
        a.substr(9,2);
        fct_chk(!"Se esperaba excepción");
      } catch (out_of_range) {
        fct_chk(true);
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Subcadena: tama~no negativo) {
      const Cadena a("0123456789");
      try {
        a.substr(9,-1);
        fct_chk(!"Se esperaba excepción");
      } catch (out_of_range) {
        fct_chk(true);
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Subcadena: tama~no excesivo) {
      const Cadena a("0123456789");
      try {
        a.substr(9,1000);
        fct_chk(!"Se esperaba excepción");
      } catch (out_of_range) {
        fct_chk(true);
      }
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Extraccion: entrada vacia) {
      istringstream is("");
      Cadena c("algo");
      is >> c;
      fct_chk(c == "");
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Extraccion: solo espacios) {
      istringstream is(" \t\r\n\v");
      Cadena c("algo");
      is >> c;
      fct_chk(c == "");
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Extraccion: hasta fin de entrada) {
      istringstream is("prueba");
      Cadena c;
      is >> c;
      fct_chk(c == "prueba");
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Extraccion: una palabra) {
      istringstream is("bueno bonito barato");
      Cadena c;
      is >> c;
      fct_chk(c == "bueno");
      fct_chk(is.peek() == ' ');
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Extraccion: una palabra con espaciado inicial) {
      istringstream is("   hola adiós");
      Cadena c;
      is >> c;
      fct_chk(c == "hola");
      fct_chk(is.peek() == ' ');
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Insercion) {
      ostringstream os;
      const char *s = "probando";
      const Cadena c(s);
      os << c;
      fct_chk(os.str() == s);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Iterador: begin()) {
	Cadena a("hola");
	*a.begin() = ' ';
	fct_chk(a.begin() == a.c_str());
	fct_chk(a[0] == ' ');
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Iterador: end()) {
	Cadena a("hola");
	fct_chk(a.end() == a.c_str() + 4);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Iterador: begin() const) {
	const Cadena a("hola");
	fct_chk(a.begin() == a.c_str());
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Iterador: end() const) {
	const Cadena a("hola");
	fct_chk(a.end() == a.c_str() + 4);
    }
    FCT_TEST_END();

    FCT_TEST_BGN(Iteradores: recorridos directos) {
	Cadena a("hola");
	for (Cadena::iterator i = a.begin(); i != a.end(); ++i)
	    (*i)++;
	fct_chk(a == "ipmb");
	const Cadena b(a);
	ostringstream os;
	for (Cadena::const_iterator i = b.begin(); i != b.end(); ++i)
	  os << *i;
	fct_chk(os.str() == "ipmb");
	for (auto i = a.cbegin(); i != a.cend(); ++i) os << *i;
	fct_chk(os.str() == "ipmbipmb");
    }
    FCT_TEST_END();
    FCT_TEST_BGN(Iteradores: recorridos inversos) {
	const Cadena a("hola");
	ostringstream os;
	for (Cadena::const_reverse_iterator r = a.rbegin(); r != a.rend(); ++r)
	    os << *r;
	fct_chk(os.str() == "aloh");
	for (auto r = a.crbegin(); r != a.crend(); ++r) os << *r;
	fct_chk(os.str() == "alohaloh");
    }
    FCT_TEST_END();
  };
  FCT_SUITE_END();
}
FCT_END()
