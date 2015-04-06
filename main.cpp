//#include "cabeceras.h"
#include <fstream>
#include <sstream>
#include "P1/Cadena/cadena.h"
#include "P1/Fecha/fecha.h"
//#include "P4/articulo.h"
//#include "P4/tarjeta.h"

//Articulo::Autores crea_autores(Autor& autor);

int main()
{
	const Cadena NombreM("Jose Manuel Barba Gonzalez");
	Fecha fecM(21,2,1982);
	Fecha fpubli(1,1,1970);
	Fecha fexp(3,7,2015);
	const Fecha fString("1/1/1970");
	Fecha f;
	const char *user = "Jose M. Barba";
	Cadena art_id("110");
	Cadena art_nom("Programación C");
	Cadena cadM(user);
	//Numero Num_tjtM("1234 9840 9482 3847");
//	Numero Num_tjtM("378282246310005");

	Cadena userid("001");
	Cadena userNom("Jose M");
	Cadena userApll("Barba Gonzalez");
	Cadena userDir("su casa");
//	const Clave userPass("holas");
	//istringstream isM("2/4/2006");
	//ostringstream os("");
	cout << "---" << endl;
	//isM >> f;

/*	Usuario userM(userid,userNom,userApll,userDir,userPass);
	Tarjeta TjtM(Num_tjtM,userM,fexp);
	Autor autM(userNom,userApll,userDir);
	Articulo::Autores autores = crea_autores(autM);
	Libro artM(autores,art_id,art_nom,fpubli,50.55,200,100);
	InformeDigital InfDigM(autores,art_id,art_nom,fpubli,50.55,fexp);
*/
	cout << "Main\n" << NombreM << endl << cadM << "." << endl;
	cout << "---Cadena---" << endl;

	cout << "Extraccion: una palabra --- ";
	istringstream is("bueno bonito barato");
	Cadena c("algo");
	is >> c;
	if(c == "bueno" && is.peek() == ' ') cout << "OK." << endl;
	cout << c << endl;

	cout << endl << "---Fecha---" << endl;

	cout << "Fecha de Nacimiento: "; fecM.visualizar();
	cout << "Fecha como cadena: "; fString.visualizar();
Fecha g(--fecM);g.visualizar();g.restadias(3);g.visualizar();
++g;g.visualizar();g.sumadias(2);g.visualizar();
	cout << "---" << endl;
	//f.visualizar();
	//cout << f;

/*	cout << "\n---Articulo Libro---\n" << artM;
	cout << "\n---Articulo InformeDigital---\n" << "A la venta hasta el "; InfDigM.imp_esp(os);
	cout << "\n---Tarjeta---\n" << TjtM.tarjeta() << endl;
	cout << "\n---Usuario---\n" << userM.id() << "|" << userM.nombre() << " " << userM.apellidos() << "|" << userM.direccion() << "|" << userPass.clave() << endl;

	if(userPass.verifica("holas"))
		cout << "Verificada" << endl;
	else
		cout << "No Verificada" << endl;
*/
	return 0;
}

/*Articulo::Autores crea_autores(Autor& autor) {
  Articulo::Autores autores;
  autores.insert(&autor);
  return autores;
}
*/
