/***************************************************************************
* articulo.h
*
* lun abril 20 17:52:48 2015
* Copyright 2015 Jose M Barba Gonzalez
* <user@host>
****************************************************************************/
/*
* articulo.h
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

#ifndef ARTICULO_H
#define ARTICULO_H

#include <set>
#include <map>
#include <iomanip>
#include "cadena.h"
#include "fecha.h"

class Autor
{
	public:
		Autor(Cadena nom, Cadena apll, Cadena dir):nombre_(nom), apellidos_(apll), direccion_(dir){};

		//Métodos observadores de los atributos.
		Cadena nombre()const {return nombre_;}
		Cadena apellidos()const {return apellidos_;}
		Cadena direccion()const {return direccion_;}

	private:
		Cadena nombre_;
		Cadena apellidos_;
		Cadena direccion_;
};

class Articulo
{
    public:
        typedef set<Autor*> Autores;

        class Autores_vacios
        {
            public:
            Autores_vacios(){};
        };
   //protected:
        //Constructores -
        Articulo(const Autores& aut, const Cadena& refr, const Cadena& tit, const Fecha& fec, double pvp, unsigned int stk=1);

        //Métodos observadores
        Cadena referencia()const{return referencia_;}
        Cadena titulo()const{return titulo_;}
        Fecha f_publi()const{return f_publi_;}
        double precio()const{return precio_;}
        virtual unsigned int stock()const{return stock_;}
        const Autores& autores()const{return autores_;}

        //Métodos modificadores modifica el atributo precio_ con el nuevo valor indicado en pvp.
        void precio(double& pvp){precio_ = pvp;}
        double& precio(){return precio_;}
        //modifica el atributo stock_ con el nuevo valor indicado en stk.
        virtual void stock(unsigned int& stk){stock_ = stk;}
        virtual unsigned int& stock(){return stock_;}
        virtual ostream& imp_esp(ostream&) const = 0;
        virtual ~Articulo(){};

    private:
        Autores autores_;
        Cadena referencia_;
        Cadena titulo_;
        Fecha f_publi_;
        double precio_;
        unsigned int stock_;
};

class ArticuloAlmacenable: public Articulo
{
    public:
        //Método Observador del stock del artículo.
         unsigned int stock()const{return stockAA_;}
        //Método Modificador del stock del artículo.
         void stock(unsigned int& stk){stockAA_ = stk;}
         unsigned int& stock(){return stockAA_;}

    //protected:
        //Constructores -
        ArticuloAlmacenable(const Autores& a, const Cadena& r, const Cadena& t, const Fecha& f, double p, unsigned int s=1);
        virtual ~ArticuloAlmacenable() {}

    private:
        unsigned int stockAA_;
};


class LibroDigital: public Articulo
{
    public:
        LibroDigital(const Autores& a, const Cadena& r, const Cadena& t, const Fecha& f, double p, const Fecha& e);
        //Método observador de la fecha de expiración.
        const Fecha& f_expir()const{return expiracion_;}
        //Operador de flujo de salida con el formato de fecha día de mes de aaaa.
        ostream& imp_esp(ostream&)const;
    private:
        Fecha expiracion_;
};


class Libro: public ArticuloAlmacenable
{
    public:
        Libro(const Autores& a, const Cadena& r, const Cadena& t, const Fecha& f, double p, unsigned int pg, unsigned int s=1);
        //Método observador de Número de Páginas de un Libro.
        unsigned int n_pag()const{return paginas_;}
        //Operador de flujo de salida con el formato "paginas, unidades" de un Libro
        ostream& imp_esp(ostream&)const;
    private:
        unsigned int paginas_;
};


class Cederron:public ArticuloAlmacenable
{
    public:
        Cederron(const Autores& a, const Cadena& r, const Cadena& t, const Fecha& f, double p, unsigned tm, unsigned int s=1);
        //Método observador del tamaño en MB de un CD.
        unsigned tam()const{return tamano_;}
        //Operador de flujo de salida con el formato "MB, unidades" de un CD
        ostream& imp_esp(ostream&)const;

    private:
        unsigned tamano_;
};

//Operadores sobrecargados
ostream& operator <<(ostream& out, const Articulo& art);

#endif //ARTICULO_H
