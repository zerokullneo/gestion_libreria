/* Prueba para la clase Cadena.
   Vea "cadena.h" para detalles.
*/
#include <iostream>
#include <stdexcept>
#include "cadena.h"
using namespace std;

int main()
{
    // Prueba de constructores
    Cadena s1(20, '[');
    Cadena s2(20, ']');
    Cadena c1(" !Bravo ");
    Cadena c2("a ");
    Cadena c3("todos ");
    Cadena s = s1 + c1;		// prueba de concatenación
    s += c2;			// pruebas de concatenación con asignación
    s += c3;
    s += "los ";
    s += "alumnos! ";
    s += s2;
    s += "\n";
    cout << "\n" << s;
    Cadena t("Por haber hecho el esfuerzo de teclear este programa. Repito:\n");
    Cadena u;			// constructor predeterminado
    u = t + s;			// pruebas de asignación, concatenación
    u = u;
    cout << u;
    Cadena v;
    v = "Hola";			// asignación con conversión desde const char*
    cout << v << endl;

    // Pruebas de índices
    size_t i = 87;
    cout << "El elemento " << i << " de la cadena es: " << u[i] << endl;
    u[94] = u[54] = 'p';
     cout << "Y tras modificar algunos caracteres, la cadena queda:\n" << u;
    try {
	cout << u.at(2000);		// Fuera de rango
    } catch(out_of_range& e) {
	cerr << "ERROR CAPTURADO. Mensaje: \"" << e.what() << "\"" << endl;
    }

    // Prueba de subcadena
    Cadena grande("Nihil novum sub solem"); // Nada nuevo bajo el Sol
    Cadena nuevo = grande.substr(6, 5);  // nuevo <- "novum"
    cout << "substr(6, 5) de " << grande << ": \"" << nuevo << "\"" << endl;
    if (nuevo != "novum")
      cerr << "*** ERROR *** Debería haber impreso \"novum\"" << endl;

    // Prueba de comparaciones
    cout << "Cadena a = \"novum\", b = \"Nihil novum sub solem\";\n a < b: "
	 << boolalpha << (nuevo < grande) << "\n a > b: " << (nuevo > grande)
	 << "\n a <= b: " << (nuevo <= grande) << "\n a >= b: "
	 << (nuevo >= grande) << "\n a == b: " << (nuevo == grande)
	 << "\n a != b: " << (nuevo != grande) << endl;
    // Prueba de longitud
    cout << "La cadena a tiene  5 caracteres: " << (nuevo.length() == 5)
	 << "\nLa cadena b tiene 21 caracteres: " << (grande.length() == 21)
	 << endl;

    // Pruebas de E/S
    Cadena c;
    cout << "Escribe un par de palabras, por favor: ";
    cin >> c;
    cout << "Palabra 1: " << c << endl;
    cout << "Carácter actual: '" << static_cast<char>(cin.peek())
	 << "'" << endl;
    cin >> c;
    cout << "Palabra 2: " << c << endl;

    // Pruebas de iteradores
    c = "No te digo trigo por no llamarte Rodrigo";
    for (auto i : c)
      cout << i;
    cout << endl;
    for (Cadena::const_reverse_iterator r = c.rbegin(); r != c.rend(); ++r)
      cout << *r;
    cout << endl;

} // ~
