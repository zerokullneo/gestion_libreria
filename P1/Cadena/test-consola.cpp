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
    Cadena s1(20, '['), s2(20, ']'),
    c1(" !Bravo "), c2("a "), c3("todos "),
    s = s1 + c1;		// prueba de concatenación
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
    Cadena w("Casa de muchos, casa de sucios");
    Cadena x(w, 16, 4);
    cout << '"' << w << "\"(16, 4) = " << x << endl;
    Cadena y("Casa de muchos, casa de sucios", 4);
    cout << "Los 4 primeros caracteres de la Cadena \"" << w
	 << "\" son \"" << y << "\"\n";

    // Pruebas de índices
    size_t i = 87;
    cout << "El elemento " << i << " de la cadena es: " << u[i] << endl;
    u[94u] = u[54u] = 'p';
     cout << "Y tras modificar algunos caracteres, la cadena queda:\n" << u;
    try {
	cout << u.at(2000);		// Fuera de rango
    } catch(out_of_range& e) {
	cerr << "ERROR CAPTURADO. Mensaje: \"" << e.what() << "\"" << endl;
    }
    
    // Prueba de subcadena
    Cadena grande("Nihil novum sub solem");
    Cadena nuevo = grande.substr(6, 5);  // nuevo <- "novum"
    cout << "substr(6, 5) de " << grande << ": \"" << nuevo << "\"" << endl;

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
} // ~
