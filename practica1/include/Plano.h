// Plano.h: interface for the Plano class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANO_H__D5740FCC_9B1B_48DD_8642_77B83D54C6A2__INCLUDED_)
#define AFX_PLANO_H__D5740FCC_9B1B_48DD_8642_77B83D54C6A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Esfera.h"
#include "Vector2D.h"

class Plano  
{
public:
	bool Rebota(Esfera& e);
	bool Rebota(Plano& p);
	void Dibuja();
	Plano();
	virtual ~Plano();

	float x1,y1;
	float x2,y2;
	float r,g,b;
protected:
	float Distancia(Vector2D punto, Vector2D *direccion);

};

#endif // !defined(AFX_PLANO_H__D5740FCC_9B1B_48DD_8642_77B83D54C6A2__INCLUDED_)
