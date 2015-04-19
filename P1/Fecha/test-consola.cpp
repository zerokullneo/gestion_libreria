/* $Id$
  Programa para probar la clase Fecha de P1.
  Vea "fecha.h" para más información.
  ©2000-2012 `El Equipo C'
  ©2013 Los profesores de POO
  ©2015 Gerardo - diferencia de fechas
*/

#include "fecha.h"
#include <iostream>
using namespace std;

// No podemos sobrecargarlas porque tienen la misma signatura.
Fecha obtener_fecha_v1(), obtener_fecha_v2();

int main()
{
  // Pruebas de constructores
  Fecha hoy, copia_hoy(hoy);	// Predeterminado, de copia
  Fecha mi_nacimiento(29, 12, 1958);
// #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
  Fecha mannana = ++hoy;	// pre-incremento
  Fecha ayer = --copia_hoy;	// pre-decremento

  cout << "Hoy es " << --hoy	// Impresión, operador de inserción
       << "\nAyer fue " << ayer << ", y mañana será " << mannana << '.' << endl;

  hoy += 7;			// Prueba de suma con asignación
  cout << "Dentro de una semana será: " << hoy << endl;
  hoy += 365 * 3;
  cout << "Dentro de 3 años y 1 semana será: " << hoy << endl;
  hoy -= 365 * 3 + 7;
  cout << "Volvemos a hoy: " << hoy << endl;

  // Pruebas de excepciones
  try {
    Fecha("20/1/2000");
    Fecha("31/2/2000");
  } catch(const Fecha::Invalida &e)
    {
      cerr << "EXCEPCIÓN: " << e.por_que() << endl;
    }

  cout << "Yo nací: " << mi_nacimiento
       << "\nDentro de una semana será: " << hoy + 7
       << "\nHoy: " << hoy
       << "\nHace 2 semanas fue: " << hoy - 14

       << "\nEl día después del 28 de febrero de este año es: ";
  Fecha f(28, 2);		// prueba de constructor de 2 parámetros
  cout << ++f
       << "\nY el de después del 28 de febrero de 2000 fue: ";
  f = Fecha(28, 2, 2000);	// prueba de asignación
  cout << ++f
       << "\nTras Año Viejo viene: ";
  Fecha av(31,12);
  cout << ++av << "\nY Año Viejo es: ";
  cout << --av

  // Prueba de constructor "cadena de caracteres"
       << "\n20/1/2000 está mejor dicho como " << Fecha("20/1/2000") << endl;

  /* Pruebas de excepciones  */

  Fecha f1 = obtener_fecha_v1(), f2 = obtener_fecha_v2();
  cout << "La 1ª fecha fue: \"" << f1
       << "\", y la 2ª: \"" << f2 << "\"." << endl;

// Diferencia de fechas
  cout << "6/2/2015 - 29/12/1958 = 20493 ..." ;
  long d = Fecha(6, 2, 2015) - Fecha(29, 12, 1958);
  if (d == 20493)
    cout << "OK!";
  else
    cout << " =/= " << d;
  cout << endl;
} // FIN

Fecha obtener_fecha_v1()
{
    while(true)
    {
        try
        {
            cout << "Introduzca una fecha en el formato DD/MM/AAAA, por favor: ";
            Fecha f(12, 12, 2012);
            cin >> f;			// operador de extracción
            return f;      // Fecha correcta: salimos.
        }
        catch(const Fecha::Invalida& e)
        {
            cerr << e.por_que() << "\aInténtelo de nuevo.\n" << endl;
            return Fecha();
        }
    }// Fin while
}

Fecha obtener_fecha_v2() // Otra alternativa. Tomamos la fecha de hoy
    try {
	cout << "Introduzca una fecha en el formato DD/MM/AAAA, por favor: ";
	Fecha f(12, 12, 2012);
	cin >> f;
	return f;
    } catch(const Fecha::Invalida& e) {
	cerr << e.por_que() << "Tom\aamos la fecha de «hoy»." << endl;
	return Fecha();
    }
