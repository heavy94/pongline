// Vector2D.cpp: implementation of the Vector2D class.
//
//////////////////////////////////////////////////////////////////////

#include "Vector2D.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vector2D::Vector2D(float xv,float yv)
{
	x=xv;
	y=yv;
}

Vector2D::~Vector2D()
{

}
float Vector2D::modulo()
{
	return (float)sqrt(x*x+y*y);
}
float Vector2D::argumento()
{
	return (float)atan2(y,x);
}	

Vector2D Vector2D::operator - (const Vector2D &v)
{
	Vector2D res;
	res.x=x-v.x;
	res.y=y-v.y;
	return res;
}
Vector2D Vector2D::operator + (const Vector2D &v)
{
	Vector2D res;
	res.x=x+v.x;
	res.y=y+v.y;
	return res;
}
float Vector2D::operator *(const Vector2D &v)	
{
	return x*v.x+y*v.y;
}
Vector2D Vector2D::operator *(float f)
{
	Vector2D res;
	res.x=x*f;
	res.y=y*f;
	return res;
}
Vector2D Vector2D::Unitario()
{
	Vector2D retorno(x,y);
	float mod=modulo();
	if(mod>0.00001)
	{
		retorno.x/=mod;
		retorno.y/=mod;
	}
	return retorno;
}

std::ifstream& operator>>(std::ifstream& f,Vector2D& v)
{
	f>>v.x;
	f>>v.y;
	return f;
}

Vector2D Vector2D::Normal()
{
	Vector2D v;
	v.x=y;
	v.y=-x;
	return v.Unitario();
}
