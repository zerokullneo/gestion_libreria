/***************************************************************************
* tarjeta.h
*
* lun abril 20 17:37:48 2015
* Copyright 2015 Jose M Barba Gonzalez
* <user@host>
****************************************************************************/
/*
* tarjeta.h
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

#ifndef TARJETA_H
#define TARJETA_H
#include <iomanip>
#include "usuario.h"
#include "../P1/Cadena/cadena.h"
#include "../P1/Fecha/fecha.h"

class Usuario;

class Numero
{
    public:
        //Atributo que indica la no validez del número.
        enum Razon{LONGITUD, DIGITOS, NO_VALIDO};
        //Clase de verificación de número incorrecto.
        class Incorrecto
        {
            public:
            //constructor de la clase Incorrecto.
            Incorrecto(Razon r);
            //Método observador del atributo de razon_.
            Razon razon()const {return razon_;}
            private:
                Razon razon_;
        };

        //constructor del numero de tarjeta
        Numero(const Cadena& n)throw (Incorrecto);
        //operador de conversión a cadena de bajo nivel
        operator const char*()const{return numero_.c_str();}
        //Validación del número de tarjeta, según el algoritmo de Lhun.static
        int isValidNumber(char* number)throw();
        friend bool operator <(const Numero&, const Numero&);

    private:
        Cadena numero_;
};

class Tarjeta
{
    public:
        //Clase de excepcion Caducada
        class Caducada
        {
            public:
                Caducada(const Fecha& f):caducada_(f){};
                Fecha cuando()const{return caducada_;};
            private:
                Fecha caducada_;
        };

        Tarjeta(const Numero& tjt,Usuario& usuario,const Fecha& cad)throw(Caducada);

        //Métodos observadores de los atributos de Tarjeta.
        Numero tarjeta()const{return tarjeta_;}
        Numero numero()const{return tarjeta_;}
        Fecha caducidad()const{return f_caducidad_;}
        Cadena titular_facial()const{return titular_facial_;}
        const Usuario* titular()const{return titular_;}
        void anula_titular();
        friend void caducar(Tarjeta& t, const Fecha& f);

        ~Tarjeta();

    private:
        //Evitar la copia de una Tarjeta
        Tarjeta(const Tarjeta&);
        //Evitar la asignacion de una Tarjeta
        Tarjeta& operator=(const Tarjeta&);
        Numero tarjeta_;
        const Usuario* titular_;
        Fecha f_caducidad_;
        Cadena titular_facial_;
};

bool operator <(const Tarjeta& t1, const Tarjeta& t2);
bool operator ==(const Tarjeta& t1, const Tarjeta& t2);
bool operator <(const Numero& n1, const Numero& n2);
ostream& operator << (ostream& out, const Tarjeta& tjt);
#endif //TARJETA_H
