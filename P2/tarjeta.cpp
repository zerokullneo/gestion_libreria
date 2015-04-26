// tarjeta.cpp
// lun abril 20 17:37:48 2015
// Copyright 2015 Jose M Barba Gonzalez
// <user@host>
// tarjeta.cpp
//
// Copyright (C) 2015 - Jose M Barba Gonzalez
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "tarjeta.h"
#include "usuario.h"

/*VALIDACIÓN DEL NÚMERO DE TARJETA*/
int Numero::luhn(const Cadena& numero, size_t n) noexcept
{
    size_t suma = 0;
    bool alt = false;

    for(int i = n - 1; i > -1; --i)
    {
        n = numero[size_t(i)] - '0';
        if (alt)
        {
            n *= 2;
            if (n > 9)
                n = (n % 10) + 1;
        }
        alt = !alt;
        suma += n;
    }
    return suma % 10;
}
/*FIN VALIDACIÓN*/

/*CLASE TARJETA*/
Tarjeta::Tarjeta(const Numero& tjt,Usuario& usuario,const Fecha& cad)throw(Caducada):tarjeta_(tjt), titular_(&usuario)
{}

void Tarjeta::anula_titular()
{
    titular_ = 0;
}

Tarjeta::~Tarjeta()
{
    Usuario *user = const_cast<Usuario*>(titular_);
    if(user)
        user->no_es_titular_de(*this);
}
/*FIN CLASE TARJETA*/

/*CLASE NUMERO*/
Numero::Numero(const Cadena& n)throw(Incorrecto)
{
    Cadena digitos(n);
    unsigned int i = 0, j = 0;

    if(n.length() == 0)
        throw Incorrecto(LONGITUD);

    while(i <= n.length())
    {
        if(n.at(i) == ' ')
            ++i;
        else
        {
            if(!isdigit(n.at(i)) && (n.at(i) != '\0'))
                throw Incorrecto(DIGITOS);

            digitos[j] = n[i];
            ++j;
            ++i;
        }
    }
cout << "-" << digitos << "-" << endl;
    if(digitos.length() < 13 || digitos.length() > 19)
        throw Incorrecto(LONGITUD);

    if(luhn(digitos, digitos.length()) != 0)
        numero_ = digitos.c_str();
    else
        throw Incorrecto(NO_VALIDO);
}

Numero::Incorrecto::Incorrecto(Razon r):razon_(r)
{}
/*FIN CLASE NUMERO*/

/*OPERADORES*/
bool operator <(const Tarjeta& t1, const Tarjeta& t2)
{
    if(strcmp(t1.numero(), t2.numero()) < 0)
        return true;
    else
        return false;
}

bool operator ==(const Tarjeta& t1, const Tarjeta& t2)
{
    if(strcmp(t1.numero(), t2.numero()) == 0)
        return true;
    else
        return false;
}

bool operator <(const Numero& n1, const Numero& n2)
{
    if(strcmp( n1.numero_.c_str(), n2.numero_.c_str()) < 0)
        return true;
    else
        return false;
}

ostream& operator <<(ostream& out, const Tarjeta& tjt)
{
    out << tjt.tarjeta() << endl;
    out << tjt.titular_facial()<<endl;
    out << "Caduca: " << setw(2) << setfill('0') << tjt.caducidad().mes() << "/" << ((tjt.caducidad().anno())%100);
    return out;
}
