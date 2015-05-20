// cadena.cpp
//
// lun marzo 16 17:53:35 2015
// Copyright 2015 Jose M Barba Gonzalez
// <user@host>
//
// cadena.cpp
//
// Copyright (C) 2015 - Jose M Barba Gonzalez
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
#include <locale>

using namespace std;

/*CONSTRUCTORES*/
//Constructor de conversión
Cadena::Cadena(unsigned int longitud, char caracter):tamano_(longitud)
{
	npos = -1;
	texto_= new char[tamano_ + 1];

	if(texto_ == NULL)
		cerr << "Cadena: Predeterminado fallo de memoria." << endl;
	else
	{
		for(unsigned int i = 0; i < tamano_; i++)
			texto_[i]=caracter;

		texto_[tamano_] = '\0';
	}
}

//Constructor de espacios vacíos.
Cadena::Cadena(unsigned int tamano):tamano_(tamano)
{
	npos = -1;
	texto_ = new char[tamano_ + 1];

	if(texto_ == NULL)
		cerr << "Cadena: unsigned int fallo de memoria." << endl;
	else
	{
		for(unsigned int i = 0; i < tamano_; i++)
			texto_[i] = ' ';

		texto_[tamano_]='\0';
	}
}

//Constructor de copia de un objeto Cadena
Cadena::Cadena(const Cadena& frase):tamano_(frase.length())
{
	npos = -1;
	texto_ = new char[tamano_ + 1];
	if(texto_ == NULL)
		cerr << "Cadena: Cadena& fallo de memoria." << endl;
	else
		strncpy(texto_,frase.texto_, tamano_ + 1);
}

//Constructor de movimiento de un objeto Cadena
Cadena::Cadena(Cadena&& frase): texto_(frase.texto_), tamano_(frase.tamano_)
{
    frase.texto_ = new char[1];
    frase.texto_[0] = '\0';
    frase.tamano_ = 0;
}

//Constructor de copia de una cadena a bajo nivel.
Cadena::Cadena(const char* texto): tamano_(strlen(texto))
{
	npos = -1;
	texto_ = new char[tamano_ + 1];
	if(texto_ == NULL)
		cerr << "Cadena: const char* fallo de memoria." << endl;
	else
	    strncpy(texto_, texto, tamano_+1);
}

//Constructor de una sub-cadena de bajo nivel char*.
Cadena::Cadena(const char* texto, size_t n): tamano_(n)
{
    npos = -1;
    texto_= new char[tamano_ + 1];
    if(texto_ == NULL or tamano_ < 0)
        cerr << "Cadena: const char* fallo de memoria." << endl;
    else
    {
        for(unsigned int i = 0; i < tamano_; i++)
            texto_[i] = texto[i];
        texto_[tamano_] = '\0';
    }
}

//Constructor de una sub-cadena desde una posicion sobre un objeto Cadena.
Cadena::Cadena(const Cadena& frase, unsigned int pos, size_t n): tamano_(n)
{
    npos = pos + tamano_;
    texto_= new char[tamano_+1];
    if(texto_ == NULL or tamano_ < 0)
        cerr << "Cadena: const char* fallo de memoria." << endl;
    else
    {
        for(unsigned int i = pos; i < npos; i++)
            texto_[i-pos] = frase[i];
        texto_[tamano_] = '\0';
    }
}

//Constructor de uns sub-cadena de un objeto Cadena de un tamaño determinado.
Cadena::Cadena(const Cadena& frase, unsigned int pos): tamano_(frase.length() - pos)
{
    npos = frase.length() - pos;
    texto_= new char[tamano_ + 1];
    if(texto_ == NULL or tamano_ < 0)
        cerr << "Cadena: const char* fallo de memoria." << endl;
    else
    {
        for(unsigned int i = pos; i < frase.length(); i++)
            texto_[i-pos] = frase[i];
        texto_[tamano_] = '\0';
    }
}
/*FIN CONSTRUCTORES*/

/*OPERADORES*/

//se suma al 'texto_' existente la nueva 'frase'
Cadena& Cadena::operator +=(const Cadena& frase) noexcept
{
	char* texto_aux = new char[tamano_+1];
	strncpy(texto_aux, texto_, tamano_+1);
	tamano_ = this->tamano_ + frase.length();
	delete[] texto_;
	texto_ = new char[tamano_+1];
	strncpy(texto_, texto_aux, strlen(texto_aux)+1);
	strncat(texto_, frase.texto_,frase.length()+1);
	return *this;
}

Cadena& Cadena::operator =(const char* texto) noexcept
{
	tamano_ = strlen(texto)+1;
	delete[] texto_;
	texto_ = new char[tamano_];
	strncpy(texto_, texto, tamano_);
	return *this;
}

Cadena& Cadena::operator =(const Cadena& frase) noexcept
{
	tamano_ = frase.length();
	delete[] texto_;
	texto_ = new char[tamano_ + 1];
	strncpy(texto_, frase.texto_, tamano_+1);
	return *this;
}

//Asignacion de movimiento
Cadena& Cadena::operator =(Cadena&& frase) noexcept
{
    tamano_ = frase.tamano_;
    texto_ = frase.texto_;
    frase.texto_ = new char[1];
    frase.texto_[0] = '\0';
    frase.tamano_ = 0;
    return *this;
}

char& Cadena::operator [](unsigned int i) noexcept
{
	return *(texto_+i);
}

char Cadena::operator [](unsigned int i) const noexcept
{
	return *(texto_+i);
}

Cadena operator +(const Cadena& texto1,const Cadena& texto2)
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
Cadena Cadena::substr(unsigned int inicio, unsigned int num_caracteres)const throw(out_of_range)
{
    if(((inicio >= 0) and (inicio <= tamano_)) and (((num_caracteres > 0) and (num_caracteres < tamano_)) and ((inicio+num_caracteres) < tamano_)))
	{
	    Cadena subtxt(num_caracteres);
        for(unsigned int i = inicio, j = 0; j < num_caracteres; i++, j++)
            subtxt.texto_[j]= texto_[i];

        return subtxt;
    }

    else
        throw std::out_of_range("Error substr: caracteres fuera de rango.");
}

//at Leer caracter
char Cadena::at(unsigned int i)const throw(out_of_range)
{
    if((i >= 0 and i <= tamano_) and (tamano_ > 0))
        return texto_[i];

    else
        throw std::out_of_range("Error read at: posicion fuera de rango.");
}

//at Escribir caracter
char& Cadena::at(unsigned int i) throw(out_of_range)
{
    if((i >= 0 and i <= tamano_) and (tamano_ >= 0))
        return texto_[i];

    else
        throw std::out_of_range("Error write at: posicion fuera de rango.");
}
/*FIN SUBCADENA*/

/*OPERACIONES SOBRE ITERADORES*/
Cadena::iterator Cadena::begin() const noexcept
{
    return &texto_[0]; //iterator(begin());
}

Cadena::iterator Cadena::end() const noexcept
{
    return &texto_[tamano_]; //iterator(end());
}

Cadena::const_iterator Cadena::cbegin() noexcept
{
    return &texto_[0]; //const_iterator(begin());
}

Cadena::const_iterator Cadena::cend() noexcept
{
    return &texto_[tamano_]; //const_iterator(end());
}

Cadena::reverse_iterator Cadena::rbegin() const noexcept
{
    return reverse_iterator(end());
}

Cadena::reverse_iterator Cadena::rend() const noexcept
{
    return reverse_iterator(begin());
}

Cadena::const_reverse_iterator Cadena::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

Cadena::const_reverse_iterator Cadena::crend() const noexcept
{
    return const_reverse_iterator(begin());
}
/*FIN OPERACIONES SOBRE ITERADORES*/

/*OPERADORES DE FLUJO*/
ostream& operator <<(ostream& out,const Cadena& texto)
{
	out << texto.c_str();
	return out;
}

//Extraccion
istream& operator >>(istream& in, Cadena& texto)
{
	//calcular la longitud del stream "in"
	in.seekg(0, in.end);
    int length = in.tellg();
    in.seekg(0, in.beg);

    // alojar memoria de "in":
    char *buffer = new char [length+1];
	buffer[length]='\0';
    // leer datos como un bloque:
    while(in.get() == ' ') in.peek();//Se salta los espacios iniciales.
    in.seekg(-1, in.cur);//Coloca el puntero de "in" en el primer caracter a leer despues de saltar los espacios.
    in.getline(buffer,length+1,' ');//lee la entrada hasta el siguiente espacio
    in.putback(' ');//deja el puntero de "in" en el espacio


	if(strspn(buffer, " \t\r\n\v") > 0)// or buffer == '\0')
    {
        const Cadena z;
        texto = z.c_str();
        return in;
    }
	else
    {
        texto=buffer;
        return in;
    }
}
/*FIN OPERADORES DE FLUJO*/
