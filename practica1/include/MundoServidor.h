// Mundo.h: interface for the CMundo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUNDO_H__9510340A_3D75_485F_93DC_302A43B8039A__INCLUDED_)
#define AFX_MUNDO_H__9510340A_3D75_485F_93DC_302A43B8039A__INCLUDED_

#include <vector>
#include "Plano.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Esfera.h"
#include "Raqueta.h"
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "Socket.h"

class CMundo  
{
public:
	void Init();
	CMundo();
	virtual ~CMundo();	
	
	void InitGL();	
//	void OnKeyboardDown(unsigned char key, int x, int y);
	void OnTimer(int value);
	void OnDraw();
	void RecibeComandosJugador();	

	std::vector<Esfera> listaEsferas;
	std::vector<Plano> paredes;
	Plano fondo_izq;
	Plano fondo_dcho;
	Raqueta jugador1;
	Raqueta jugador2;

	int puntos1;
	int puntos2;
	int contador;
	int wait_p2; //Cuenta cuanto tiempo lleva el jugador 2 inactivo
	int flag; //Inhabilita la interaccion entre esferas durante un tiempo cuando se crea una nueva
	//FIFO logger
	char *tuberia;
	int fd_fifo;
	//Identificador de thread
	pthread_t thread_id;
	pthread_t thread_server;
	//Sockets
	Socket servidor;
	std::vector<Socket> conexiones;
	void GestionaConexiones();
};

#endif // !defined(AFX_MUNDO_H__9510340A_3D75_485F_93DC_302A43B8039A__INCLUDED_)
