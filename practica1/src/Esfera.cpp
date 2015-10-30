// Esfera.cpp: implementation of the Esfera class.
//
//////////////////////////////////////////////////////////////////////

#include "Esfera.h"
#include "glut.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Esfera::Esfera()
{
	radio=0.5f;
	velocidad.x=3;
	velocidad.y=3;
}

Esfera::~Esfera()
{

}



void Esfera::Dibuja()
{
	glColor3ub(255,255,0);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(centro.x,centro.y,0);
	glutSolidSphere(radio,15,15);
	glPopMatrix();
}

void Esfera::Mueve(float t)
{
	centro.x += velocidad.x*t;
	centro.y += velocidad.y*t;
}

bool Esfera::Rebota(Esfera &e)
{
	//Vector que une los centros
	Vector2D dif = e.centro - this->centro;
	float d = dif.modulo();
	float dentro = d - (this->radio + e.radio);

	if (dentro<0.0f)//si hay colision
	{
		//El modulo y argumento de la velocidad de la pelota1
		float v1 = this->velocidad.modulo();
		float ang1 = this->velocidad.argumento();

		//El modulo y argumento de la velocidad de la pelota2
		float v2 = e.velocidad.modulo();
		float ang2 = e.velocidad.argumento();

		//el argumento del vector que une los centros
		float angd = dif.argumento();

		//Separamos las esferas, lo que se han incrustado
		//la mitad cada una
		Vector2D desp(dentro / 2 * cos(angd), dentro / 2 * sin(angd));
		this->centro = this->centro + desp;
		e.centro = e.centro - desp;

		angd = angd - 3.14159f / 2;//la normal al choque

		//El angulo de las velocidades en el sistema relativo antes del choque
		float th1 = ang1 - angd;
		float th2 = ang2 - angd;

		//Las componentes de las velocidades en el sistema relativo ANTES del choque
		float u1x = v1*cos(th1);
		float u1y = v1*sin(th1);
		float u2x = v2*cos(th2);
		float u2y = v2*sin(th2);

		//Las componentes de las velocidades en el sistema relativo DESPUES del choque
		//la componente en X del sistema relativo no cambia
		float v1x = u1x;
		float v2x = u2x;

		//en el eje Y, rebote elastico
		float m1 = this->radio;
		float m2 = e.radio;
		float py = m1*u1y + m2*u2y;//Cantidad de movimiento inicial ejey
		float ey = m1*u1y*u1y + m2*u2y*u2y;//Energia cinetica inicial ejey

		//los coeficientes y discriminante de la ecuacion cuadrada
		float a = m2*m2 + m1*m2;
		float b = -2 * py*m2;
		float c = py*py - m1*ey;
		float disc = b*b - 4 * a*c;
		if (disc<0)disc = 0;

		//las nuevas velocidades segun el eje Y relativo
		float v2y = (-b + sqrt(disc)) / (2 * a);
		float v1y = (py - m2*v2y) / m1;

		//Modulo y argumento de las velocidades en coordenadas absolutas
		float modv1, modv2, fi1, fi2;
		modv1 = sqrt(v1x*v1x + v1y*v1y);
		modv2 = sqrt(v2x*v2x + v2y*v2y);
		fi1 = angd + atan2(v1y, v1x);
		fi2 = angd + atan2(v2y, v2x);

		//Velocidades en absolutas despues del choque en componentes
		this->velocidad.x = modv1*cos(fi1);
		this->velocidad.y = modv1*sin(fi1);
		e.velocidad.x = modv2*cos(fi2);
		e.velocidad.y = modv2*sin(fi2);
	}
}
