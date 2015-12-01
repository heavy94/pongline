#include "glut.h"
#include "MundoServidor.h"

//el unico objeto global
CMundo mundo;

//los callback, funciones que seran llamadas automaticamente por la glut
//cuando sucedan eventos
//NO HACE FALTA LLAMARLAS EXPLICITAMENTE
void OnDraw(void); //esta funcion sera llamada para dibujar
void OnTimer(int value); //esta funcion sera llamada cuando transcurra una temporizacion
void OnKeyboardDown(unsigned char key, int x, int y); //cuando se pulse una tecla	

int main(int argc,char* argv[])
{
	//Inicializar el gestor de ventanas GLUT
	//y crear la ventana
	glutInit(&argc, argv);
	glutInitWindowSize(800,600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Servidor");


	//Registrar los callbacks
	glutDisplayFunc(OnDraw);
	//glutMouseFunc(OnRaton);

	glutTimerFunc(25,OnTimer,0);//le decimos que dentro de 25ms llame 1 vez a la funcion OnTimer()
	glutKeyboardFunc(OnKeyboardDown);
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
	
	mundo.InitGL();

	
	//pasarle el control a GLUT,que llamara a los callbacks
	glutMainLoop();	

	return 0;   
}

void OnDraw(void)
{
	mundo.OnDraw();
}
void OnTimer(int value)
{
	mundo.OnTimer(value);
	glutTimerFunc(25,OnTimer,0);
	glutPostRedisplay();
}
void OnKeyboardDown(unsigned char key, int x, int y)
{
	//mundo.OnKeyboardDown(key,x,y);
	glutPostRedisplay();
}
