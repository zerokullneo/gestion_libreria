// tarjeta.cpp
//
// lun abril 20 17:37:48 2015
// Copyright 2015 Jose M Barba Gonzalez
// <user@host>
//
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

bool luhn(const Cadena& numero, size_t n);

/*FIN VALIDACIÓN*/

/*CLASE TARJETA*/
Tarjeta::Tarjeta(const Numero& tjt, Usuario& usuario, const Fecha& f_cad):
tarjeta_(tjt), titular_(&usuario), f_caducidad_(f_cad), titular_facial_((usuario.nombre() + " " + usuario.apellidos()))
{
    const Fecha f_hoy;
    //cout << f_hoy << "-" << f_cad << endl;
    if((f_hoy > f_cad) == true)
        throw Caducada(f_cad);

    titular_->es_titular_de(*this);
}

void Tarjeta::anula_titular() noexcept
{
    titular_ = 0;
}

Tarjeta::~Tarjeta()
{
    Usuario *user = const_cast<Usuario*>(titular_);
    if(user)
    {
        this->anula_titular();
        user->no_es_titular_de(*this);
    }
}
/*FIN CLASE TARJETA*/

/*CLASE NUMERO*/
Numero::Numero(const Cadena& n)
{//enum Razon{LONGITUD, DIGITOS, NO_VALIDO};
    Cadena::iterator n_end = n.end();

    if(n.length() <=12)
        throw Incorrecto(LONGITUD);
    else
    {
        n_end = remove_if(n.begin(), n.end(), EsBlanco());//deja los espacios en blanco al final "  "
        *n_end = '\0';//se tiene que poner el final de cadena en el iterador.

        if(n_end != find_if(n.begin(), n.end(), EsDigito()))
            throw Incorrecto(DIGITOS);
    }

    if(strlen(n.c_str()) < 13 or strlen(n.c_str()) > 19)
        throw Incorrecto(LONGITUD);

    if(luhn(n, strlen(n.c_str())) == false)
        numero_ = n.c_str();
    else
        throw Incorrecto(NO_VALIDO);
}
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
