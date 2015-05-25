/***************************************************************************
* usuario-pedido.cpp
*
* mie mayo 13 18:30:35 2015
* Copyright 2015 Jose M Barba Gonzalez
* <user@host>
****************************************************************************/
/*
* usuario-pedido.cpp
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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef USUARIO_PEDIDO_H
#define USUARIO_PEDIDO_H

//#include "../cabeceras.h"
#include "pedido.h"
#include "usuario.h"

class Pedido;
class Usuario;

class Usuario_Pedido
{
	public:
		typedef set<Pedido*> Pedidos;
		void asocia(Usuario& u, Pedido& p);
		void asocia(Pedido& p,Usuario& u);
		Pedidos pedidos(Usuario& u);
		Usuario* cliente(Pedido& p);

	private:
		map <Usuario*, Pedidos> Usuario_Pedidos_;
		map <Pedido*, Usuario*> Pedido_Usuario_;
};

inline void Usuario_Pedido::asocia(Usuario& u, Pedido& p)
{
	Usuario_Pedidos_[&u].insert(&p);
	Pedido_Usuario_.insert(make_pair(&p,&u));
}

inline void Usuario_Pedido::asocia(Pedido& p, Usuario& u)
{
	this->asocia(u,p);
}

inline Usuario_Pedido::Pedidos Usuario_Pedido::pedidos(Usuario& u)
{
	return Usuario_Pedidos_[&u];
}

inline Usuario* Usuario_Pedido::cliente(Pedido& p)
{
	return Pedido_Usuario_[&p];
}
#endif /*USUARIO_PEDIDO_H*/
