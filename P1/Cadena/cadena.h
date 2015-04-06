/***************************************************************************
 *            cadena.h
 *
 *  vie diciembre 20 01:53:35 2013
 *  Copyright  2013  Jose M Barba Gonzalez
 *  <user@host>
 ****************************************************************************/
/*
 * cadena.h
 *
 * Copyright (C) 2013 - Jose M Barba Gonzalez
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

//#include "../../cabeceras.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>

using namespace std;

#ifndef CADENA_H_
#define CADENA_H_
class Cadena
{
	public:
	    /*Constructores*/
		//Constructor predeterminado texto_[0]=' ';
		explicit Cadena (){tamano_=0;texto_=new char[tamano_];}
		//Constructor de conversión
		Cadena (unsigned int longitud, char caracter);
		//Constructor de espacios vacíos.
		explicit Cadena(unsigned int tamano);
		//Constructor de copia de un objeto Cadena
		Cadena (const Cadena& frase);
		//Constructor de copia de una cadena a bajo nivel.
		Cadena (const char* texto);

		//operadores sobrecargados
		Cadena& operator +=(const Cadena& frase);
		Cadena& operator =(const char* texto);
		Cadena& operator =(const Cadena& frase);
		char& operator[](unsigned int i);
		char operator[](unsigned int i) const;

		//Funcion que extrae una subcadena dentro de otra
		//La variable 'inicio' indica el indice donde comenzara a contar, contando
		//el numero de caracteres especificado en la variable 'num_caracteres'.
		Cadena subcadena(unsigned int inicio, unsigned int num_caracteres)const throw out_of_range;
		const char* c_str()const{return texto_;};
		char at(unsigned int i)const throw out_of_range;
		char& at(unsigned int i) throw out_of_range;

		//funciones observadoras
		char* Cad()const{return texto_;}
		//recibe un objeto Cadena para verificar su longitud
		unsigned int longitud(Cadena& c)const{return c.tamano_;}
		//devuelve el atributo tamano_ del objeto cadena actual
		unsigned int longitud()const{return tamano_;}
		//const char* imprimirP()const;
		void imprimirP()const;

		//Insercción y extracción de flujo
		friend ostream& operator <<(ostream& out,const Cadena& texto);
		friend istream& operator >>(istream& in,Cadena& texto);

		//Destructor de Cadena
		~Cadena(){delete [] texto_;};

	private:
		char* texto_;
		unsigned int tamano_;
};

//operadores sobrecargados externos de mas de un elemento
Cadena operator+(const Cadena& texto1,const Cadena& texto2);
bool operator ==(const Cadena& texto1,const Cadena& texto2);
bool operator !=(const Cadena& texto1,const Cadena& texto2);
bool operator >=(const Cadena& texto1,const Cadena& texto2);
bool operator >(const Cadena& texto1,const Cadena& texto2);
bool operator <=(const Cadena& texto1,const Cadena& texto2);
bool operator <(const Cadena& texto1,const Cadena& texto2);

//Insercción y extracción de flujo
//ostream& operator <<(ostream& out,const Cadena& texto);
//istream& operator >>(istream& in,Cadena& texto);
#endif	/* CADENA_H_ */
