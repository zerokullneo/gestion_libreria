//           fecha.cpp
//  vie diciembre 20 01:57:48 2013
//  Copyright  2013  Jose M Barba Gonzalez
//  <user@host>
// fecha.cpp
//
// Copyright (C) 2013 - Jose M Barba Gonzalez
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

#include "fecha.h"

using namespace std;

//CONSTRUCTORES

//inline Fecha::Fecha(int dia, int mes, int year){ Fecha.d_ = dia; Fecha.m_ = mes; Fecha.a_ = year;}

Fecha::Fecha(int dia, int mes, int year):d_(dia),m_(mes),a_(year)
{
	defecto_();
	if(!dia || dia == 0) d_ = getd_;
	else d_ = dia;

	if(!mes || mes == 0) m_ = getm_;
	else m_ = mes;

	if(!year || year == 0) a_ = geta_;
	else a_ = year;

	comprueba_fecha(d_, m_, a_);
}

//Constructor de conversión de Cadena a Fecha.
Fecha::Fecha(const char* string_fecha)
{
	char fech[11];

	if(sscanf(string_fecha,"%i/%i/%i", &d_, &m_, &a_)==0)
	{
        throw Invalida("Entrada Incorrecta.");
	}
	else
	{
		strncpy(fech, string_fecha,strlen(string_fecha));

		char *fecha=fech;
		fecha = strtok(fecha,"/-");
		d_ = atoi(fecha);
		fecha = strtok(NULL, "/-");
		m_ = atoi(fecha);
		fecha = strtok(NULL, "/-");
		a_ = atoi(fecha);

		comprueba_fecha(d_, m_, a_);
	}
}

Fecha::Invalida::Invalida(const char* t)
{
	tp_ = t;
	cerr << por_que(tp_);
}
//FIN CONSTRUCTORES

//OPERADORES

Fecha& Fecha::operator +=(int incremento)
{
	this->sumadias(incremento);
	return *this;
}

Fecha& Fecha::operator -=(int decremento)
{
	this->restadias(decremento);
	return *this;
}

Fecha& Fecha::operator ++()//postincremento
{
	this->sumadias(1);
	return *this;
}

Fecha Fecha::operator ++(int)//preincremento
{
	Fecha f(*this);
	this->sumadias(1);
	return f;
}

Fecha& Fecha::operator --()//postdecremento
{
	this->restadias(1);
	return *this;
}

Fecha Fecha::operator --(int)//predecremento
{
	Fecha f(*this);
	this->restadias(1);
	return f;
}

Fecha& Fecha::operator =(const Fecha& fec)
{
	if(this != &fec)
	{
            d_ = fec.d_;
            m_ = fec.m_;
            a_ = fec.a_;
	}
	return *this;
}

bool Fecha::operator +(int incremento)
{
	if(incremento > 31)
		return false;

	this->sumadias(incremento);
	return true;
}

bool Fecha::operator -(int decremento)
{
	if(decremento > 31)
		return false;

	this->restadias(decremento);
	return true;
}

/*------------------FIN OPERADORES---------------------*/

/*--------------------MODIFICADORAS--------------------*/
bool bisiesto(int a)
{
	return !(a%4) && ((a%100) || !(a%400));
}

Fecha& Fecha::sumadias(int incmt_d)
{
	int dm[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	this->d_ += incmt_d;
	if(bisiesto(this->a_)) dm[1] = 29; else dm[1] = 28;

	while(this->d_ > dm[this->m_-1])
	{
		this->d_ -= dm[this->m_-1];
		this->m_+=1;
		if(this->m_ > 12)
		{
			this->m_ = 1;
			this->a_+=1;
			if(bisiesto(this->a_)) dm[1] = 29; else dm[1] = 28;
		}
	}

	if(a_ >	YEAR_MAXIMO)
		throw Invalida("Año Incorrecto.");//year

	return *this;
}

Fecha& Fecha::restadias(int decmt_d)
{
	int dm[] = {0,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},sum,i;

	this->d_ -= decmt_d;
	if(bisiesto(this->a_)) dm[1] = 29; else dm[1] = 28;

	sum = this->d_;

	for(((dm[this->m_-1] < 1) ? i = dm[12] :(i = dm[this->m_-1])); (sum < i || sum < dm[this->m_-1])&& (sum <= (dm[this->m_]));)
	{
		if(sum == 0)
		{
			this->m_= this->m_ - 1;
			if(this->m_ < 1)
			{
				this->m_ = 12;
				this->a_-=1;
				if(bisiesto(this->a_)) dm[1] = 29; else dm[1] = 28;
			}
			this->d_ = this->d_ + dm[this->m_];
			sum = dm[this->m_];
		}
		else
		{
			this->m_ = this->m_ - 1;

			if(this->m_ < 1)
			{
				this->m_ = 12;
				this->a_-=1;
				if(bisiesto(this->a_)) dm[1] = 29; else dm[1] = 28;
			}
			if((sum = this->d_ + dm[this->m_]) < 32)
			{
				this->d_ = this->d_ + dm[this->m_];
				sum = this->d_ + dm[this->m_];
			}
			else
				this->m_ = this->m_ + 1;
		}
	}

	if(a_ <	YEAR_MINIMO)
		throw Invalida("Año Incorrecto.");//year
	return *this;
}

Fecha& Fecha::sumames(int incmt_m)
{
    if (incmt_m == 12)
    {
        this->m_ = 1;
    }
    else
        this->m_ = incmt_m;

    return *(this);
}

Fecha& Fecha::restames(int decmt_m)
{
    if (decmt_m == 1)
    {
        this->m_ = 12;
    }
    else
        this->m_ = decmt_m;

    return *(this);
}

Fecha& Fecha::sumayear(int incmt_a)
{
    this->a_ = a_ + incmt_a;
    return *this;
}

Fecha& Fecha::restayear(int decmt_a)
{
    this->a_ = a_ - decmt_a;
    return *this;
}
/*-----------------FIN MODIFICADORAS--------------------*/

/*------------------OBSERVADORAS------------------------*/

ostream& Fecha::observadorPublico() const
{
	tm timeinfo = { 0, 0, 0, d_, m_ - 1, a_ - 1900, 0, 0, -1 };
	mktime(&timeinfo);
	const char* const weekday[7] = {"Lunes","Martes","Miercoles","Jueves","Viernes","Sábado","Domingo"};
	const char* const month[12] = {"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"};
	return cout << "Día " << weekday[timeinfo.tm_wday] << " " << d_ << " de " << month[timeinfo.tm_mon] << " del " << a_ << "." << endl;
}

const char* Fecha::cadena()const
{
	static char f_explicita[40];
	tm timeinfo = { 0, 0, 0, d_, m_ - 1, a_ - 1900, 0, 0, -1 };
	mktime(&timeinfo);
	const char* const weekday[7] = {"domingo","lunes","martes","miércoles","jueves","viernes","sábado"};
	const char* const month[12] = {"enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre"};
	sprintf(f_explicita,"%s %d de %s de %4d", weekday[timeinfo.tm_wday], dia(), month[timeinfo.tm_mon], anno());
	return f_explicita;
}

void Fecha::visualizar() const
{
   if(d_ > 0 && d_ < 10) cout << "0"; cout << d_ << "/";if(m_ > 0 && m_ < 10) cout << "0"; cout << m_ << "/" << a_ << endl;
}
/*------------------FIN OBSERVADORAS---------------------*/

bool Fecha::comprueba_fecha(int& dia, int& mes, int& year)
{
	if ((year < YEAR_MINIMO) || (year > YEAR_MAXIMO))
	{
		throw Invalida("Año Incorrecto.");//year
	}

	if(mes > 0 && mes < 13)
	{
		if(dia > 31)
			throw Invalida("Dia fuera de rango");
		else
		{
			switch(mes)
			{
				case 1:
					case 3:
						case 5:
							case 7:
								case 8:
									case 10:
										case 12:
										{
											if(dia < 1 || dia > 31)
												throw Invalida("dia31");//dia
											break;
										}
				case 4:
					case 6:
						case 9:
							case 11:
							{
								if(dia < 1 || dia > 30)
									throw Invalida("dia30");//dia
								break;
							}
				case 2:
				{
					if((year % 4) == 0)
					{
						if (dia < 1 || dia > 29)
							throw Invalida("dia29");//dia
					}
					else
					{
						if(dia < 0 || dia > 28)
							throw Invalida("dia28");//dia
					}
					break;
				}
				default:
					throw Invalida("defmes");//mes
			}
		}
	}
	else
		throw Invalida("mes");//mes

	return true;
}


/*---OPERADORES EXTERNOS---*/

bool operator ==(const Fecha& fec1, const Fecha& fec2)
{
	if ((fec1.visualizar_dia() == fec2.visualizar_dia()) && (fec1.visualizar_mes() == fec2.visualizar_mes()) && (fec1.visualizar_anyo() == fec2.visualizar_anyo()))
		return true;
	else
		return false;
}

bool operator <(const Fecha& fec1, const Fecha& fec2)
{
	if((fec1.dia() < fec2.dia()) || ((fec1.dia() >= fec2.dia() && (fec1.mes() < fec2.mes())) || ((fec1.mes() >= fec2.mes()) && fec1.anno() < fec2.anno())))
	    return true;
	else
	    return false;
}

bool operator >(const Fecha& fec1, const Fecha& fec2)
{
	if((fec1.dia() > fec2.dia()) || ((fec1.dia() <= fec2.dia() && (fec1.mes() > fec2.mes())) || ((fec1.mes() <= fec2.mes()) && fec1.anno() > fec2.anno())))
	    return true;
	else
	    return false;
}

bool operator <=(const Fecha& fec1, const Fecha& fec2)
{
	if((fec1.dia() <= fec2.dia()) || ((fec1.dia() > fec2.dia() && (fec1.mes() <= fec2.mes())) || ((fec1.mes() > fec2.mes()) && fec1.anno() <= fec2.anno())))
	    return true;
	else
	    return false;
}

bool operator >=(const Fecha& fec1, const Fecha& fec2)
{
	if((fec1.dia() >= fec2.dia()) || ((fec1.dia() < fec2.dia() && (fec1.mes() >= fec2.mes())) || ((fec1.mes() < fec2.mes()) && fec1.anno() >= fec2.anno())))
	    return true;
	else
	    return false;
}

bool operator !=(const Fecha& fec1, const Fecha& fec2)
{
	if ((fec1.visualizar_dia() == fec2.visualizar_dia()) && (fec1.visualizar_mes() == fec2.visualizar_mes()) && (fec1.visualizar_anyo() == fec2.visualizar_anyo()))
	    return false;
	else
	    return true;
}

ostream& operator <<(ostream& os, const Fecha& fec)
{
    os << fec.cadena();
    return os;
}

istream& operator >>(istream& is, Fecha& fec)
{
	static char fecha[11];
	is >> fecha;
	fec=Fecha(fecha);
	return is;
}

Fecha operator + (int incremento, const Fecha& fec)
{
	Fecha tmp(fec);
	tmp += incremento;
	return tmp;
}

Fecha operator + (const Fecha& fec, int incremento)
{
	Fecha tmp(fec);
	tmp += incremento;
	return tmp;
}

Fecha operator - (int decremento, const Fecha& fec)
{
	Fecha tmp(fec);
	tmp -= decremento;
	return tmp;
}

Fecha operator - (const Fecha& fec, int decremento)
{
	Fecha tmp(fec);
	tmp -= decremento;
	return tmp;
}
