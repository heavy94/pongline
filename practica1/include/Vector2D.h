// Vector2D.h: interface for the Vector2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR2D_H__EC63CCAD_9EB8_450F_A7F0_89A2DCBC2E28__INCLUDED_)
#define AFX_VECTOR2D_H__EC63CCAD_9EB8_450F_A7F0_89A2DCBC2E28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>

class Vector2D  
{
	friend std::ifstream& operator>>(std::ifstream& f,Vector2D& v);

public:
	Vector2D Normal();
	Vector2D(float x=0.0f,float y=0.0f);
	virtual ~Vector2D();

	float x,y;

	float modulo();					//modulo del vector
	float argumento();				//argumento del vector
	Vector2D Unitario();			//devuelve un vector unitario
	Vector2D operator - (const Vector2D &);//resta de vectores
	Vector2D operator + (const Vector2D &);//suma de vectores
	float operator *(const Vector2D &);	//producto escalar
	Vector2D operator *(float);		//producto por un escalar
};

#endif // !defined(AFX_VECTOR2D_H__EC63CCAD_9EB8_450F_A7F0_89A2DCBC2E28__INCLUDED_)
