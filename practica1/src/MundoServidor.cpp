// Mundo.cpp: implementation of the CMundo class.
//
//////////////////////////////////////////////////////////////////////
#include <fstream>
#include "MundoServidor.h"
#include "glut.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_PUNTOS 5

void* hilo_comandos_1(void* d)
{
    CMundo* p=(CMundo*) d;
    p->RecibeComandosJugador1();
    pthread_exit(0);
}
void* hilo_comandos_2(void* d)
{
    CMundo* p=(CMundo*) d;
    p->RecibeComandosJugador2();
    pthread_exit(0);
}
void* hilo_conexiones(void* d)
{
    CMundo* p=(CMundo*) d;
    p->GestionaConexiones();
    pthread_exit(0);
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMundo::CMundo():acabar(0),contador(0),wait_p2(0),flag(0)
{
	char tb[] = "/tmp/logger";
	tuberia = tb;
	Init();
}

CMundo::~CMundo()
{
	listaEsferas.clear();
	/*Cerrar tuberia logger*/
	close(fd_fifo);
	//Cerrar hilos
	acabar = 1;
	pthread_join(thread_jugador1,NULL);
	pthread_join(thread_jugador2,NULL);
	servidor.Close();
	pthread_join(thread_server,NULL);
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

	jugador1.Mueve(0.025f);
	jugador2.Mueve(0.025f);
	for(i=0;i<listaEsferas.size();i++) 
		listaEsferas[i].Mueve(0.025f);
	for(i=0;i<paredes.size();i++)
	{
		for(j=0;j<listaEsferas.size();j++)
			paredes[i].Rebota(listaEsferas[j]);
		paredes[i].Rebota(jugador1);
		paredes[i].Rebota(jugador2);
	}
	//Interaccion entre las esferas
	if(flag<=0)
	{
		for(i=0;i<listaEsferas.size();i++)
		{
			for(j=i+1;j<listaEsferas.size();j++)
			{
				listaEsferas[i].Rebota(listaEsferas[j]);
			}
		}
	}
	else
		flag--;

	for(i=0;i<listaEsferas.size();i++)
	{
		jugador1.Rebota(listaEsferas[i]);
		jugador2.Rebota(listaEsferas[i]);

		if(fondo_izq.Rebota(listaEsferas[i]))
		{
			Esfera* nueva_esfera = new Esfera;
			listaEsferas.clear();
			listaEsferas.push_back(*nueva_esfera);
			listaEsferas[0].centro.x=0;
			listaEsferas[0].centro.y=rand()/(float)RAND_MAX;
			listaEsferas[0].velocidad.x=2+2*rand()/(float)RAND_MAX;
			listaEsferas[0].velocidad.y=2+2*rand()/(float)RAND_MAX;
			contador=0;
			puntos2++;
			char buffer[10];
			sprintf(buffer, "2 %d", puntos2);			
			write(fd_fifo,buffer,strlen(buffer));
		}
		if(fondo_dcho.Rebota(listaEsferas[i]))
		{
			Esfera* nueva_esfera = new Esfera;
			listaEsferas.clear();
			listaEsferas.push_back(*nueva_esfera);
			listaEsferas[0].centro.x=0;
			listaEsferas[0].centro.y=rand()/(float)RAND_MAX;
			listaEsferas[0].velocidad.x=-2-2*rand()/(float)RAND_MAX;
			listaEsferas[0].velocidad.y=-2-2*rand()/(float)RAND_MAX;
			contador=0;
			puntos1++;
			char buffer[10];
			sprintf(buffer, "1 %d", puntos1);
			write(fd_fifo,buffer,strlen(buffer));
		}
	}
	//Finaliza el juego cuando uno de los jugadores alcanza MAX_PUNTOS
	if(puntos1 >= MAX_PUNTOS)
	{
		printf("[TENIS] El jugador 1 gano la partida %d a %d\n", puntos1, puntos2);
		for(int i=0; i<conexiones.size(); i++)
			conexiones[i].Send("end",4);
		exit(0);
	}
	if(puntos2 >= MAX_PUNTOS)
	{
		printf("[TENIS] El jugador 2 gano la partida %d a %d\n", puntos2, puntos1);
		for(int i=0; i<conexiones.size(); i++)
			conexiones[i].Send("end",4);
		exit(0);
	}
	//Añade una nueva esfera
	if(++contador>=400) //cada 10s
	{
		int indice = 0;
		for(int i=0;i<listaEsferas.size();i++)
			if(listaEsferas[i].radio > listaEsferas[indice].radio)
				indice = i;
		listaEsferas[indice].radio*=0.8f;

		Esfera* nueva_esfera = new Esfera;
		listaEsferas.push_back(*nueva_esfera);
		listaEsferas.back().centro.x=listaEsferas[indice].centro.x;
		listaEsferas.back().centro.y=listaEsferas[indice].centro.y;
		listaEsferas.back().radio=listaEsferas[indice].radio;
		listaEsferas.back().velocidad.x=listaEsferas[indice].velocidad.x*(rand()/(float)RAND_MAX+0.65f);
		listaEsferas.back().velocidad.y=-listaEsferas[indice].velocidad.y*(rand()/(float)RAND_MAX+0.65f);
		flag = 40; // 1s
		contador = 0;
	}
	
	char buffer[300];
	char aux[300];
	sprintf(buffer,"%d ", (int)listaEsferas.size());
	for(int k = 0; k < listaEsferas.size(); k++) {
		sprintf(aux,"%f %f %f ", listaEsferas[k].centro.x, listaEsferas[k].centro.y, listaEsferas[k].radio);
		strcat(buffer,aux);
	}
	sprintf(aux,"%f %f %f %f %f %f %f %f %d %d", jugador1.x1, jugador1.y1, jugador1.x2, jugador1.y2, jugador2.x1, jugador2.y1, jugador2.x2, jugador2.y2, puntos1, puntos2);
	strcat(buffer,aux);

	for (int i = conexiones.size() - 1; i >= 0; i--) {
		if (conexiones[i].Send(buffer,300) <= 0) {
			conexiones.erase(conexiones.begin()+i);
			printf("Cliente desconectado. Quedan: %d clientes\n\n", conexiones.size());
			if (i < 2) puntos1 = puntos2 = 0; //Si se desconecta uno de los clientes que estan jugando el marcador se reinicia
		}
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

	if((fd_fifo = open(tuberia, O_WRONLY)) == -1) {
		printf("Error al abrir tuberia. Iniciar primero el logger\n");
		exit(1);
	}

	//Creacion de sockets
	char user_ip[20];
	printf("Introduzca su IP: ");
	scanf("%s", user_ip);
	printf("\n");
	servidor.InitServer(user_ip,2000);
	//Creacion hilo gestor conexiones
	pthread_create(&thread_server, NULL, hilo_conexiones, this);
	//Creacion hilos teclado
	pthread_create(&thread_jugador1, NULL, hilo_comandos_1, this);
	pthread_create(&thread_jugador2, NULL, hilo_comandos_2, this);
}

void CMundo::RecibeComandosJugador1()
{
	int n1;
	while (!acabar) {
		usleep(10);
		char p1[10];
		if(conexiones.size() >= 1)
			n1 = conexiones[0].Receive(p1, sizeof(char));
		unsigned char key1;
		sscanf(p1,"%c",&key1);
		if(key1=='s')jugador1.velocidad.y=-6;
		if(key1=='w')jugador1.velocidad.y=6;
		if(key1=='x')jugador1.velocidad.y=0;
	}
}
void CMundo::RecibeComandosJugador2()
{
	int n2;
	while (!acabar) {
		usleep(10);
		char p2[10];
		if(conexiones.size() >= 2)
			n2 = conexiones[1].Receive(p2, sizeof(char));
		unsigned char key2;
		sscanf(p2,"%c",&key2);
		if(key2=='l')jugador2.velocidad.y=-6;
		if(key2=='o')jugador2.velocidad.y=6;
		if(key2=='.')jugador2.velocidad.y=0;
	}
}

void CMundo::GestionaConexiones()
{
	while(!acabar) {
		char nombre[20] = "\0";
		Socket s = servidor.Accept();
		if(s.getSock()!=INVALID_SOCKET) {
			conexiones.push_back(s);
			int r=conexiones.back().Receive(nombre,20);
			printf("Nuevo cliente: %s\n", nombre);
			printf("Numero de clientes: %d\n\n", conexiones.size());
			if (conexiones.size() <= 2) puntos1 = puntos2 = 0; /*Si el numero de clientes es menor que 2 el marcador 
																se reinicia al conectarse un nuevo cliente (jugador)*/
		}
	}	
}