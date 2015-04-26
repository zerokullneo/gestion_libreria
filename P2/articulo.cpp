// articulo.cpp
// lun abril 20 17:52:48 2015
// Copyright 2015 Jose M Barba Gonzalez
// <user@host>
// articulo.cpp
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

#include "articulo.h"

/*CLASE ARTICULO - const Autores& aut, autores_(aut),*/
Articulo::Articulo(const Cadena& refr, const Cadena& tit, const Fecha& fec, double pvp, unsigned int stk):
referencia_(refr), titulo_(tit), f_publi_(fec), precio_(pvp), stock_(stk)
{
	/*if(autores_.empty())
		throw Autores_vacios();*/
}
/*FIN CLASE ARTICULO*/

/*CLASE ARTICULOALMACENABLE - const Autores& a,*/
/*ArticuloAlmacenable::ArticuloAlmacenable(const Cadena& r,const Cadena& t,const Fecha& f,double p,unsigned int s):Articulo(r,t,f,p),stockAA_(s)
{
}*/
/*FIN CLASE ARTICULOALMACENABLE*/

/*CLASE INFORMEDIGITAL - const Autores& a,*/
/*InformeDigital::InformeDigital(const Cadena& r,const Cadena& t,const Fecha& f,double p,const Fecha& e):Articulo(r,t,f,p),expiracion_(e)
{
}

ostream& InformeDigital::imp_esp(ostream& out) const
{
	out << expiracion_.observadorPublico();
	return out;
}*/
/*FIN CLASE INFORMEDIGITAL*/

/*CLASE LIBRO - const Autores& a,*/
/*Libro::Libro(const Cadena& r,const Cadena& t,const Fecha& f,double p,unsigned int pg,unsigned int s):ArticuloAlmacenable(r,t,f,p,s),paginas_(pg)
{
}

ostream& Libro::imp_esp(ostream& out) const
{
	out << paginas_ << " págs., " << this->stock() << " unidades.";
	return out;
}*/
/*FIN CLASE LIBRO*/

/*CLASE CEDERRON - const Autores& a,*/
/*Cederron::Cederron(const Cadena& r,const Cadena& t,const Fecha& f,double p,unsigned tm,unsigned int s):ArticuloAlmacenable(r,t,f,p,s),tamano_(tm)
{
}

ostream& Cederron :: imp_esp(ostream& out) const
{
	out << tamano_ << " MB, " << this->stock() << " unidades.";
	return out;
}*/
/*FIN CLASE CEDERRON*/

ostream& operator <<(ostream& out, const Articulo& art)
{
	out << "[" << art.referencia() << "] \"" << art.titulo() << "\", " << art.f_publi().anno() << ". " << art.precio() << "€" << endl;
	return out;
}
