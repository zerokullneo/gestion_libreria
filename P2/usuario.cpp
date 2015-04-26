// usuario.cpp
// lun abril 20 17:47:48 2015
// Copyright 2015 Jose M Barba Gonzalez
// <user@host>
// usuario.cpp
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#define _XOPEN_SOURCE
#include <unistd.h>
#include "usuario.h"
#include "articulo.h"
#include "tarjeta.h"

/*Clase clave*/
Clave::Clave(const char* clav)throw(Clave::Incorrecta)
{
    if(strlen(clav) < 5)throw Incorrecta(CORTA);

    const char* c = crypt(clav,"@#");
    clave_ = c;

    if(!clave_.length())throw Incorrecta(ERROR_CRYPT);
}

Clave::Incorrecta::Incorrecta(Razon r):r_(r)
{}

bool Clave::verifica(const char* pass) const
{
    pass = crypt(pass,"@#");

    if(0 == strcmp(pass, clave_.c_str()))
        return true;
    else
        return false;
}
/*Fin Clase clave*/

/*Clase Usuario*/
//Columna de identificadores de usuarios.
static Usuario::Usuarios id_;
Usuario::Usuario(Cadena id, Cadena nom, Cadena apll, Cadena dir, Clave pass)throw(Usuario::Id_duplicado,Clave::Incorrecta):
identificador_(id), nombre_(nom), apellidos_(apll), direccion_(dir), contrasenia_(pass)
{
    //comprobamos si ese identificador de usuario ya existe.
    if (id_.insert(id).second == false) throw Id_duplicado(id);
}

Usuario::Id_duplicado::Id_duplicado(const Cadena& id_d):idd_(id_d)
{}

void Usuario::es_titular_de(Tarjeta& T)
{
    tarjetas_.insert(pair<Numero,Tarjeta*>(T.tarjeta(),&T));
}

void Usuario::no_es_titular_de(Tarjeta& T)
{
    tarjetas_.erase(T.tarjeta());
}

void Usuario::compra(Articulo& A, unsigned i)
{
    if(i == 0)
        articulos_.erase(&A);
    else
    {
        if(!articulos_.insert(pair<Articulo*,unsigned>(&A,i)).second)
        articulos_[&A] = i;
    }
}

ostream& operator <<(ostream& out, const Usuario& u)
{
    out << u.id() << " [" << u.clave().clave() << "] " << u.nombre() << " " << u.apellidos() << std::endl;
    out << u.direccion() << endl;
    out << "Tarjetas:\n";

    for(Usuario::Tarjetas::const_iterator it = u.tarjetas().begin(); it != u.tarjetas().end(); it++)
        out << *((*it).second) << endl;

    return out;
}

ostream& mostrar_carro(ostream& out, const Usuario& u)
{
    out << "Carrito de compras de " << u.id() << " [Artículos: " << u.n_articulos() << "]\n";

    if(u.n_articulos() != 0)
    {
        out << " Cant. Artículo\n";
        out << "=======================================================================\n";
        for(Usuario::Articulos::const_iterator it = u.compra().begin(); it != u.compra().end(); it++)
        {
            out << (*it).second << " " << "[" << it->first->referencia() << "] \"" << it->first->titulo() << "\", " << it->first->f_publi().anno();
            out << ". " << setprecision(2) << fixed << it->first->precio() << "€" << endl;
        }
    }

    return out;
}
