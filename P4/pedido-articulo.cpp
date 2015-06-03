// pedido_articulo.cpp
//
// mie mayo 13 18:30:35 2015
// Copyright 2015  Jose M Barba Gonzalez
// <user@host>
//
// pedido_articulo.cpp
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


#include "pedido-articulo.h"

/*##########Clase LineaPedido##########*/
LineaPedido::LineaPedido(double p, unsigned c):precio_venta_(p),cantidad_(c)
{}

ostream& operator <<(ostream& out,const LineaPedido& L)
{
	out << setprecision(2) << fixed << L.precio_venta() << " €\t" << L.cantidad();
	return out;
}

/*##########Clase OrdenaPedidos##########*/

bool OrdenaPedidos::operator()(Pedido* P1,Pedido* P2)const
{
	return (P1->numero() < P2->numero());
}

/*##########Clase OrdenaArticulos##########*/

bool OrdenaArticulos::operator()(Articulo* A1, Articulo* A2)const
{
	return (A1->referencia() < A2->referencia());
}

/*##########Clase Pedido_Articulo##########*/

void Pedido_Articulo::pedir(Pedido& pedido, Articulo& articulo, double precio, unsigned cantidad)
{
    //insercción en ambos sentidos de los artículos de un pedido
	Pedido_Articulo_[&pedido].insert(std::make_pair(&articulo,LineaPedido(precio,cantidad)));
	Articulo_Pedido_[&articulo].insert(std::make_pair(&pedido,LineaPedido(precio,cantidad)));
}

void Pedido_Articulo::pedir(Articulo& articulo, Pedido& pedido, double precio, unsigned cantidad)
{
	pedir(pedido,articulo,precio,cantidad);
}

Pedido_Articulo::ItemsPedido& Pedido_Articulo::detalle(Pedido& P)
{
	return Pedido_Articulo_[&P];
}

Pedido_Articulo::Pedidos& Pedido_Articulo::ventas(Articulo& A)
{
	return Articulo_Pedido_[&A];
}

ostream& operator <<(ostream& out, const Pedido_Articulo::ItemsPedido& I)
{
	double precio_total = 0.0;
	if(!I.empty())
	{
		out << "=============================================================================\n";
		out << "PVP\tCant.\tArtículo";
		out << "\n=============================================================================\n";

		for (Pedido_Articulo::ItemsPedido::const_iterator i = I.begin(); i!=I.end(); ++i)
		{
			out << i->first->precio() << " €\t" << i->second.cantidad();
			out << "\t[" << i->first->referencia() << "] \"" << i->first->titulo() << "\"\n";
			precio_total+=i->second.precio_venta() * i->second.cantidad();
		}

		out << "=============================================================================\n";
		out << "Total:\t" << precio_total << " €\n";
	}

	return out;
}

ostream& operator <<(ostream& out, const Pedido_Articulo::Pedidos& P)
{
	out << "[Pedidos: " << P.size() << "]";
	if(!P.empty())
	{
		out << "\n=============================================================================\n";
		out << "PVP\tCant.\tFecha venta";
		out << "\n=============================================================================\n";
		double precio_total = 0.0;
		int cantidad_total = 0;

		for(Pedido_Articulo::Pedidos::const_iterator i = P.begin(); i!=P.end(); ++i)
		{
			out << i->second.precio_venta() << " €" << "\t" << i->second.cantidad() << "\t" << i->first->fecha() << endl;
			precio_total += (i->second).precio_venta() * i->second.cantidad();
			cantidad_total += i->second.cantidad();
		}
		out << "=============================================================================\n";
		out << "Precio Total: " << setprecision(2) << fixed << precio_total << " €\tCantidad Total: " << cantidad_total;
	}
	return out;
}

void Pedido_Articulo::mostrarDetallePedidos(ostream& out)const
{
	double total_ventas = 0.0;

	for(map<Pedido*,ItemsPedido,OrdenaPedidos>::const_iterator i = Pedido_Articulo_.begin(); i != Pedido_Articulo_.end(); ++i)
	{
		out << "Pedido núm.\t" << (i->first)->numero() << " Cliente:\t" << (i->first)->tarjeta()->titular_facial() << "\tFecha:\t" << (i->first)->fecha().cadena() << endl;
		out << i->second;
		total_ventas += (i->first)->total();
	}
	if(!Pedido_Articulo_.empty())
		out << "TOTAL VENTA:\t" << total_ventas << " €";
}

void Pedido_Articulo::mostrarVentasArticulos(ostream& out)const
{
	for(map<Articulo*,Pedidos>::const_iterator i = Articulo_Pedido_.begin(); i != Articulo_Pedido_.end(); ++i)
	{
		out << "Ventas de " << "[" << i->first->referencia() << "] \"" << i->first->titulo() << "\"\t" << i->first->precio() << " €";
		out	<< i->second << endl << endl;
	}
}
