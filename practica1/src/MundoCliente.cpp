// Mundo.cpp: implementation of the CMundo class.
//
//////////////////////////////////////////////////////////////////////
#include <fstream>
#include "MundoCliente.h"
#include "glut.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <signal.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMundo::CMundo()
{
	Init();
}

CMundo::~CMundo()
{
	listaEsferas.clear();
	//Cerrar Socket
	comunicacion.Close();
}

void CMundo::InitGL()
{
	//Habilitamos las luces, la renderizacion y el color de los materiales
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 40.0, 800/600.0f, 0.1, 150);
}

void print(char *mensaje, int x, int y, float r, float g, float b)
{
	glDisable (GL_LIGHTING);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT) );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glColor3f(r,g,b);
	glRasterPos3f(x, glutGet(GLUT_WINDOW_HEIGHT)-18-y, 0);
	int len = strlen (mensaje );
	for (int i = 0; i < len; i++) 
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, mensaje[i] );
		
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
}
void CMundo::OnDraw()
{
	//Borrado de la pantalla	
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Para definir el punto de vista
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	
	gluLookAt(0.0, 0, 17,  // posicion del ojo
		0.0, 0.0, 0.0,      // hacia que punto mira  (0,0,0) 
		0.0, 1.0, 0.0);      // definimos hacia arriba (eje Y)    

	/////////////////
	///////////
	//		AQUI EMPIEZA MI DIBUJO
	char cad[100];
	sprintf(cad,"Jugador1: %d",puntos1);
	print(cad,10,0,1,1,1);
	sprintf(cad,"Jugador2: %d",puntos2);
	print(cad,650,0,1,1,1);
	int i;
	for(i=0;i<paredes.size();i++)
		paredes[i].Dibuja();

	fondo_izq.Dibuja();
	fondo_dcho.Dibuja();
	jugador1.Dibuja();
	jugador2.Dibuja();
	for(i=0;i<listaEsferas.size();i++)
		listaEsferas[i].Dibuja();

	/////////////////
	///////////
	//		AQUI TERMINA MI DIBUJO
	////////////////////////////

	//Al final, cambiar el buffer
	glutSwapBuffers();
}

void CMundo::OnTimer(int value)
{
	int i, j;
	char buffer[300];
	char lectura[300];
	char aux[50];
	int n_esferas;

	comunicacion.Receive(buffer, 300);
	if(strcmp(buffer,"end")==0) {	
		exit(0);
	}
	else {
		sscanf(buffer,"%d ", &n_esferas);
		sprintf(lectura, "%%*d ");
		if(n_esferas < listaEsferas.size()){
			listaEsferas.clear();
			for(int k = 0; k < n_esferas; k++){
				Esfera* nueva_esfera = new Esfera;
				listaEsferas.push_back(*nueva_esfera);
			}
		}
		else if(n_esferas > listaEsferas.size()){
			Esfera* nueva_esfera = new Esfera;
			listaEsferas.push_back(*nueva_esfera);
		}
		for(int k = 0; k < listaEsferas.size(); k++){
			sprintf(aux, "%%f %%f %%f ");
			strcat(lectura,aux);	
			sscanf(buffer,lectura, &listaEsferas[k].centro.x, &listaEsferas[k].centro.y, &listaEsferas[k].radio);
			sprintf(lectura, "%%*d ");
			for(int i = 0; i <= k; i++){
				sprintf(aux, "%%*f %%*f %%*f ");
				strcat(lectura,aux);
			}
		}
		sprintf(aux,"%%f %%f %%f %%f %%f %%f %%f %%f %%d %%d");
		strcat(lectura,aux);
		sscanf(buffer,lectura, &jugador1.x1, &jugador1.y1, &jugador1.x2, &jugador1.y2, &jugador2.x1, &jugador2.y1, &jugador2.x2, &jugador2.y2, &puntos1, &puntos2);
	}
}

void CMundo::OnKeyboardDown(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 's':comunicacion.Send("s",2);break;
	case 'w':comunicacion.Send("w",2);break;
	case 'x':comunicacion.Send("x",2);break;
	case 'l':comunicacion.Send("l",2);break;
	case '.':comunicacion.Send(".",2);break;
	case 'o':comunicacion.Send("o",2);break;
	}
}

void CMundo::Init()
{
	Esfera* nueva_esfera = new Esfera;	
	listaEsferas.push_back(*nueva_esfera);
	
	Plano p;
//pared inferior
	p.x1=-7;p.y1=-5;
	p.x2=7;p.y2=-5;
	paredes.push_back(p);

//superior
	p.x1=-7;p.y1=5;
	p.x2=7;p.y2=5;
	paredes.push_back(p);

	fondo_izq.r=0;
	fondo_izq.x1=-7;fondo_izq.y1=-5;
	fondo_izq.x2=-7;fondo_izq.y2=5;

	fondo_dcho.r=0;
	fondo_dcho.x1=7;fondo_dcho.y1=-5;
	fondo_dcho.x2=7;fondo_dcho.y2=5;

	//a la izq
	jugador1.g=0;
	jugador1.x1=-6;jugador1.y1=-1;
	jugador1.x2=-6;jugador1.y2=1;

	//a la dcha
	jugador2.g=0;
	jugador2.x1=6;jugador2.y1=-1;
	jugador2.x2=6;jugador2.y2=1;

	//Creacion Socket
	char mensaje[20];
	printf("Introduzca nombre: ");
	scanf("%s", mensaje);
	char user_id[20];
	printf("Introduzca la IP del servidor: ");
	scanf("%s", user_id);
	comunicacion.Connect(user_id,2000);
	comunicacion.Send(mensaje,strlen(mensaje));
}
