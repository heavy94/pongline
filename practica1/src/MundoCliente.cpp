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

#define MAX_PUNTOS 3


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMundo::CMundo():contador(0),wait_p2(0),flag(0)
{
	static char tb_sc[] = "/tmp/fifo_sc";
	static char tb_cs[] = "/tmp/fifo_cs";
	tuberia_sc = tb_sc;
	tuberia_cs = tb_cs;
	Init();
}

CMundo::~CMundo()
{
	/*Cerrar y destruir la tuberia SC*/
	close(fd_fifo_sc);
	unlink(tuberia_sc);
	/*Cerrar y destruir la tuberia CS*/
	close(fd_fifo_cs);
	unlink(tuberia_cs);

	listaEsferas.clear();
	bot1->esfera.radio = -1;
	munmap(bot1,bstat.st_size);
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
	int bytes_leidos;
	int n_esferas;

	bytes_leidos=read(fd_fifo_sc, buffer, 300);
	if(bytes_leidos == 0) {	//Salir del bucle cuando no hay datos en la tuberia
		//printf("Tuberia SC vacia\n");
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

	int index;
	//Control del jugador1 por parte del bot
	index = 0;
	float dist = listaEsferas[0].centro.x - jugador1.x1;
	for(int i=1;i<listaEsferas.size();i++) {
		if(dist > listaEsferas[i].centro.x - jugador1.x1) {
			index = i;
			dist = listaEsferas[i].centro.x - jugador1.x1;
		}
	}
	bot1->esfera = listaEsferas[index];
	bot1->raqueta = jugador1;
	if(bot1->accion == 1)
		OnKeyboardDown('w', 0, 0);
	if(bot1->accion == 0)
		OnKeyboardDown('x', 0, 0);
	if(bot1->accion == -1)
		OnKeyboardDown('s', 0, 0);
	//Control del jugador2 por parte del bot si esta inactivo mas de 5 segundos
	if(wait_p2>=200) {
		index = 0;
		float dist = jugador2.x1 - listaEsferas[0].centro.x;
		for(int i=1;i<listaEsferas.size();i++) {
			if(dist > jugador2.x1 - listaEsferas[i].centro.x) {
				index = i;
				dist = jugador2.x1 - listaEsferas[i].centro.x;
			}
		}
		bot2->esfera = listaEsferas[index];
		bot2->raqueta = jugador2;
		if(bot2->accion == 1)
			OnKeyboardDown('o', 0, 0);
		if(bot2->accion == 0)
			OnKeyboardDown('.', 0, 0);
		if(bot2->accion == -1)
			OnKeyboardDown('l', 0, 0);
		wait_p2=200;
	}
	else
		wait_p2++;
	
}

void CMundo::OnKeyboardDown(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 's':break;
	case 'w':break;
	case 'x':break;
	case 'l':wait_p2=0;break;
	case '.':wait_p2=0;break;
	case 'o':wait_p2=0;break;
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


	bot1 = new DatosMemCompartida;
	bot1->esfera = listaEsferas[0];
	bot1->raqueta = jugador1;
	bot1->accion = 0;
	bot2 = new DatosMemCompartida;
	bot2->esfera = listaEsferas[0];
	bot2->raqueta = jugador1;
	bot2->accion = 0;

	creat("/tmp/bot", 0777);
	fd_fichero = open("/tmp/bot", O_RDWR);
	write(fd_fichero, bot1, sizeof(DatosMemCompartida));
	write(fd_fichero, bot2, sizeof(DatosMemCompartida));
	delete bot1;
	delete bot2;
	/* Averigua la longitud del fichero */ 
  	if (fstat(fd_fichero, &bstat)<0) {
   		perror("Error en fstat del fichero"); close(fd_fichero);
   		exit(1);
 	}
  	/* Se proyecta el fichero */
	if((bot1 = (DatosMemCompartida*)mmap(NULL, bstat.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd_fichero, 0)) == MAP_FAILED) {
		printf("Error en la proyeccion del fichero");
		exit(1);
	}
		bot2 = bot1 + 1;
	close(fd_fichero);
}
