/***************************************************************************
* usuario.h
*
* lun abril 20 17:47:48 2015
* Copyright 2015 Jose M Barba Gonzalez
* <user@host>
****************************************************************************/
/*
* usuario.h
*
* Copyright (C) 2015 - Jose M Barba Gonzalez
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef USUARIO_H
#define USUARIO_H
#include <set>
#include <map>
#include "../P1/Cadena/cadena.h"
#include "../P1/Fecha/fecha.h"
#include "articulo.h"
#include "tarjeta.h"

class Tarjeta;
class Numero;

class Clave
{
    public:
        //Atributo que indica la no validez de la clave.
        enum Razon{CORTA, ERROR_CRYPT};
        //Clase de verificación de clave incorrecta.
        class Incorrecta
        {
            public:
                Incorrecta(Razon r);
                Razon razon()const{return r_;}

            private:
                Razon r_;
        };

        Clave(const char* clav)throw (Incorrecta);
        Cadena clave()const{return clave_;}
        bool verifica(const char* pass) const;

    private:
        Cadena clave_;
};

class Usuario
{
    public:
        typedef set<Cadena> Usuarios;
        typedef map<Numero, Tarjeta*> Tarjetas;
        typedef map<Articulo*, unsigned> Articulos;

        class Id_duplicado
        {
            public:
                //Constructor predeterminado
                Id_duplicado(const Cadena& id_d):idd_(id_d){};
                //Método observador
                Cadena idd()const {return idd_;}
            private:
                Cadena idd_;
        };

        //Constructor
        Usuario(Cadena id, Cadena nom, Cadena apll, Cadena dir, Clave pass) throw(Usuario::Id_duplicado);
        //Métodos observadores de los atributos.
        Cadena id()const {return identificador_;}
        Cadena nombre()const {return nombre_;}
        Cadena apellidos()const {return apellidos_;}
        Cadena direccion()const {return direccion_;}
        Clave clave()const {return contrasenia_;}
        size_t n_articulos()const{return articulos_.size();}
        const Tarjetas& tarjetas()const{return tarjetas_;}
        const Articulos& compra()const{return articulos_;}

        //Métodos modificadores
        void es_titular_de(Tarjeta& T);
        void no_es_titular_de(Tarjeta& T);
        void compra(Articulo& A, unsigned i=1);

    private:
        //Evitar la copia de un objeto Usuario
        Usuario(const Usuario&);
        Usuario& operator =(const Usuario&);
        Cadena identificador_;
        Cadena nombre_;
        Cadena apellidos_;
        Cadena direccion_;
        Clave contrasenia_;
        Articulos articulos_;
        Tarjetas tarjetas_;
};

std::ostream& operator << (std::ostream& out , const Usuario& u);
std::ostream& mostrar_carro (std::ostream& out , const Usuario& u);
#endif //USUARIO_H
