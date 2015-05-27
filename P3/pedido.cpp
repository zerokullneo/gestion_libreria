// pedido.cpp
//
// mie mayo 13 18:30:35 2015
// Copyright 2015 Jose M Barba Gonzalez
// <user@host>
//
// pedido.cpp

// Copyright (C) 2015 - Jose M Barba Gonzalez

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

#include "pedido.h"
#include "usuario-pedido.h"
#include "pedido-articulo.h"
#include "articulo.h"

int Pedido::N_pedidos = 0;

Pedido::Pedido(Usuario_Pedido& U_P, Pedido_Articulo& P_A, Usuario& U, const Tarjeta& T,const Fecha& F):num_(N_pedidos+1),total_(0.0),tarjeta_(&T),fecha_pedido_(F)
{
    if(U.compra().empty())
        throw(Vacio(U));

    if(tarjeta_->titular() != &U)
        throw Impostor(U);

    for(auto stk = U.compra().begin(); stk != U.compra().end(); stk++)
        if(stk->second > stk->first->stock())
        {
            U.compra(*stk->first,0);
            throw SinStock((*stk->first));
        }

    if(tarjeta_->caducidad() < fecha_pedido_)
        throw Tarjeta::Caducada(tarjeta_->caducidad());

    Usuario::Tarjetas t(U.tarjetas());

    Usuario::Articulos A(U.compra());

    bool NoHayStock = false;

    Usuario::Articulos::iterator i;

    for(i = A.begin(); i != A.end() && !NoHayStock; i++)
    {
        //Comprobar si se trata de un puntero a un objeto a InformeDigital
        //if(dynamic_cast<InformeDigital*>(i->first) != 0)
        //{
            //if(static_cast<InformeDigital*>(i->first)->f_expir()>fecha_)
            //{
                //añadir el precio del articulo al total_
                total_ += (*(i->first)).precio() * (*i).second;
                //Asociar Pedido con Articulo
                P_A.pedir(*(i->first),*this,(i->first)->precio(),(*i).second);
            //}
            //else
            //U.compra(*(i->first),0);
        //}
        //else
        //{
            if((*i).second > (*i->first).stock())//static_cast<ArticuloAlmacenable*>(i->first)->stock())
                NoHayStock=true;
            //else
            //{
                //Actualizacion de el stock
                //(*i->first).stock() = (*i->first).stock() - (*i).second;
                //static_cast<ArticuloAlmacenable*>(i->first)->stock() -= (*i).second;
                //añadir el precio del articulo al total_
                //total_ += (*(i->first)).precio() * (*i).second;
                //Asociar Pedido con Articulo
                //P_A.pedir(*(i->first),*this,(i->first)->precio(),(*i).second);
            //}
        //}
    }
    if(!U.n_articulos())
        throw(Vacio(U));

    //Vaciar el Carrito
    for(Usuario::Articulos::iterator j = A.begin(); j != A.end(); j++)
        U.compra(*(j->first),0);

    //Puntero Art_ptr apunta a A.end() o bien al primer articulo sin stock
    Articulo* Art_ptr = (--i)->first;

    //Lanzar La excepcion SinStock
    if(NoHayStock)
        throw(SinStock(*Art_ptr));

    //num_ = N_pedidos++;

    //Asociar usuario con Pedido
    U_P.asocia(U,*this);

    ++N_pedidos;
}

ostream& operator <<(ostream& out,const Pedido& P)
{
    out << "Núm. pedido:\t" << P.numero() << endl;
    out << "Fecha:\t"<< P.fecha().cadena() << endl;
    out << "Pagado con:\t" << P.tarjeta()->tarjeta() << endl;
    out << "Importe:\t" << P.total() << " €";
    return out;
}
