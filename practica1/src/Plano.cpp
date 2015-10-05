// Plano.cpp: implementation of the Plano class.
//
//////////////////////////////////////////////////////////////////////

#include "Plano.h"
#include "glut.h"
#include "Vector2D.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Plano::Plano()
{
	r=g=b=1.0f;
}

Plano::~Plano()
{

}

void Plano::Dibuja()
{
	glColor3f(r,g,b);
	glDisable(GL_LIGHTING);
	
	glBegin(GL_POLYGON);
		glVertex3d(x1,y1,-1);
		glVertex3d(x1,y1,1);
		glVertex3d(x2,y2,1);	
		glVertex3d(x2,y2,-1);	
	glEnd();
}

//Calculo de distancia de un Pared a un punto, adicionalmente
//se modifica el valor de un vector direccion que contendrá
//el vector unitario saliente que indica la direccion de la 
//recta mas corta entre el punto y el Pared.
float Plano::Distancia(Vector2D punto, Vector2D *direccion)
{
	Vector2D limite1(x1,y1);
	Vector2D limite2(x2,y2);
	Vector2D u=(punto-limite1);
	//esto seria mejor almacenarlo en variables miembro
	//para no tener que calcularlo cada vez
	Vector2D v=(limite2-limite1).Unitario();
	float longitud=(limite1-limite2).modulo();

	Vector2D dir;
	float valor=u*v;
	float distancia=0;

	if(valor<0)
		dir=u;
	else if(valor>longitud)
		dir=(punto-limite2);
	else
		dir=u-v*valor;

	distancia=dir.modulo();
	if(direccion!=0)
		*direccion=dir.Unitario();
	
	return distancia;
}
bool Plano::Rebota(Esfera &e)
{
	Vector2D dir;
	float dif=Distancia(e.centro,&dir)-e.radio;
	if(dif<=0.0f)
	{
		Vector2D v_inicial=e.velocidad;
		e.velocidad=v_inicial-dir*2.0*(v_inicial*dir);
		e.centro=e.centro-dir*dif;
		return true;
	}
	return false;
}
bool Plano::Rebota(Plano& p)
{
	Esfera e;
	e.centro.x=(p.x1+p.x2)/2;
	e.centro.y=(p.y1+p.y2)/2;
	e.radio=0.5*sqrt((p.x1-p.x2)*(p.x1-p.x2)+(p.y1-p.y2)*(p.y1-p.y2));

	if(Rebota(e))
	{
		p.x1=e.centro.x;
		p.x2=e.centro.x;
		p.y1=e.centro.y+e.radio;
		p.y2=e.centro.y-e.radio;
		return true;
	}
	return false;
}
