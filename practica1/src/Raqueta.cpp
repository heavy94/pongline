// Raqueta.cpp: implementation of the Raqueta class.
//
//////////////////////////////////////////////////////////////////////

#include "Raqueta.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Raqueta::Raqueta()
{
	
}

Raqueta::~Raqueta()
{

}

void Raqueta::Mueve(float t)
{
 y1+=velocidad.y*t;
 y2+=velocidad.y*t;
}
