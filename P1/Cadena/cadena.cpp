//           cadena.cpp
//  vie diciembre 20 01:53:35 2013
//  Copyright  2013  Jose M Barba Gonzalez
//  <user@host>
// cadena.cpp
//
// Copyright (C) 2013 - Jose M Barba Gonzalez
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "cadena.h"

using namespace std;

/*CONSTRUCTORES*/
//Constructor de conversión
Cadena::Cadena(unsigned int longitud, char caracter)
{
	tamano_ = longitud;
	texto_= new char[longitud + 1];

	if(texto_ == NULL)
		cerr << "Cadena: Predeterminado fallo de memoria." << endl;
	else
	{
		for(unsigned int i = 0; i < longitud; i++)
			texto_[i]=caracter;

		texto_[longitud] = '\0';
	}
}

//Constructor de espacios vacíos.
Cadena::Cadena(unsigned int tamano)
{
	tamano_ = tamano;
	texto_= new char[tamano + 1];

	if(texto_ == NULL)
		cerr << "Cadena: unsigned int fallo de memoria." << endl;
	else
	{
		for(unsigned int i = 0; i < tamano; i++)
			texto_[i] = ' ';

		texto_[tamano]='\0';
	}
}

//Constructor de copia de un objeto Cadena
Cadena::Cadena(const Cadena& frase)
{
	tamano_ = frase.longitud();
	texto_ = new char[tamano_ + 1];
	if(texto_ == NULL)
		cerr << "Cadena: Cadena& fallo de memoria." << endl;
	else
	{
		strcpy(texto_,frase.texto_);
		texto_[tamano_ + 1] = '\0';
	}
}

//Constructor de copia de una cadena a bajo nivel.
Cadena::Cadena(const char* texto)
{
	tamano_= strlen(texto);
	texto_ = new char[tamano_ + 1];
	if(texto_ == NULL)
		cerr << "Cadena: const char* fallo de memoria." << endl;
	else
	{
		//strncpy(texto_,texto,tamano_);
		for(unsigned int i = 0; i <= tamano_; i++)
			texto_[i] = texto[i];
		texto_[tamano_ + 1] = '\0';
	}
}
/*FIN CONSTRUCTORES*/

/*OPERADORES*/

//se suma al 'texto_' existente la nueva 'frase'
Cadena& Cadena::operator += (const Cadena& frase)
{
	unsigned int i, j=tamano_;
	int tam = tamano_ + frase.tamano_ + 1;

	char* texto_aux = new char[tam];

	for(i = 0; i <= tamano_; i++)
		texto_aux[i] = texto_[i];
	for(i = 0; i <= frase.tamano_; j++, i++)
		texto_aux[j] = frase.texto_[i];

	texto_aux[tam] = '\0';
	texto_=new char[tam];
	this->tamano_ = tam;
	strncpy(texto_, texto_aux, tam);
	delete texto_aux;
	return *this;
}

Cadena& Cadena::operator =(const char* texto)
{
	tamano_ = strlen(texto) + 1;
	texto_ = (char*) realloc(texto_, tamano_);
	strncpy(texto_, texto, tamano_);
	return *this;
}

Cadena& Cadena::operator =(const Cadena& frase)
{
	tamano_ = strlen(frase.c_str()) + 1;
	texto_ = (char*) realloc(texto_, tamano_);
	strncpy(texto_, frase.texto_, tamano_);
	return *this;
}

char Cadena::operator[](unsigned int i) const
{
	return *(texto_+i);
}

char& Cadena::operator[](unsigned int i)
{
	return *(texto_+i);
}

Cadena operator+(const Cadena& texto1,const Cadena& texto2)
{
	Cadena frase(texto1);
	frase += texto2;
	return frase;
}

bool operator ==(const Cadena& texto1,const Cadena& texto2)
{
	if(strcmp(texto1.c_str(),texto2.c_str()) == 0)
		return true;
	else
		return false;
}

bool operator !=(const Cadena& texto1,const Cadena& texto2)
{
	if(strcmp(texto1.c_str(),texto2.c_str()) != 0)
		return true;
	else
		return false;
}

bool operator >=(const Cadena& texto1,const Cadena& texto2)
{
	if(strcmp(texto1.c_str(),texto2.c_str()) >= 0)
		return true;
	else
		return false;
}

bool operator >(const Cadena& texto1,const Cadena& texto2)
{
	if(strcmp(texto1.c_str(),texto2.c_str()) > 0)
		return true;
	else
		return false;
}

bool operator <=(const Cadena& texto1,const Cadena& texto2)
{
	if(strcmp(texto1.c_str(),texto2.c_str()) <= 0)
		return true;
	else
		return false;
}

bool operator <(const Cadena& texto1,const Cadena& texto2)
{
	if(strcmp(texto1.c_str(),texto2.c_str()) < 0)
		return true;
	else
		return false;
}
/*FIN OPERADORES*/

/*SUBCADENA*/
Cadena Cadena::subcadena(unsigned int inicio, unsigned int num_caracteres)const throw()
{
    try
    {
        //if((inicio < 0) || (num_caracteres < 0) || ((inicio+num_caracteres) > tamano_) || (inicio > tamano_) || (num_caracteres > tamano_))
		Cadena subtxt(num_caracteres);
		for(unsigned int i = inicio, j = 0; i < inicio + num_caracteres; i++, j++)
			subtxt.texto_[j]= texto_[i];

		return subtxt;
    }
	catch(const char* out_of_range) //else
	{
        cerr << "El indice de comienzo o el numero de caracteres indicado está fuera de rango. " << endl;
	}
}

char Cadena::at(unsigned int i)const throw()
{
	/*if((i >= tamano_) || (i < 0))
		throw out_of_range("El índice indicado está fuera de rango.\n");
	else
		return texto_[i];*/
    try
    {
        return texto_[i];
    }
    catch(const char* out_of_range)
    {
        cerr << "El índice indicado está fuera de rango. " << endl;
    }
}

char& Cadena::at(unsigned int i) throw()
{
	/*if((i >= tamano_) || (i<0))
		throw out_of_range("El índice indicado está fuera de rango.\n");
	else
		return texto_[i];*/
    try
    {
        return texto_[i];
    }
    catch(const char* out_of_range)
    {
        cerr << "El índice indicado está fuera de rango. " << endl;
    }
}
/*FIN SUBCADENA*/

/*OPERADORES DE FLUJO*/
ostream& operator <<(ostream& out,const Cadena& texto)
{
	out << texto.texto_;
	return out;
}

istream& operator >>(istream& in, Cadena& texto)
{
	//calcular la longitud del stream "in"
	in.seekg (0, in.end);
    int length = in.tellg();
    in.seekg (0, in.beg);

    // alojar memoria de "in":
    char *buffer = new char [length+1];
	buffer[length]='\0';
    // leer datos como un bloque:
    in.read (buffer,length);

	if(buffer == '\0')
		texto=Cadena();
	else
		texto=buffer;
	return in;
}

/*FIN OPERADORES DE FLUJO*/

/*OBSERVADORAS*/
void Cadena::imprimirP()const
{
	cout << texto_;
}
/*FIN OBSERVADORAS*/
