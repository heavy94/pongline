// Esfera.h: interface for the Esfera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ESFERA_H__8D520BAF_8208_423B_BD91_29F6687FB9C3__INCLUDED_)
#define AFX_ESFERA_H__8D520BAF_8208_423B_BD91_29F6687FB9C3__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector2D.h"

class Esfera  
{
public:	
	Esfera();
	virtual ~Esfera();
		
	Vector2D centro;
	Vector2D velocidad;
	float radio;

	void Mueve(float t);
	void Dibuja();
	bool Rebota(Esfera &e);
};

#endif // !defined(AFX_ESFERA_H__8D520BAF_8208_423B_BD91_29F6687FB9C3__INCLUDED_)
