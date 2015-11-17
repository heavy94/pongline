#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "DatosMemCompartida.h"

int main(){
	int fd_fichero;
	DatosMemCompartida *bot1;
	DatosMemCompartida *bot2;
    	struct stat bstat;

	fd_fichero = open("/tmp/bot", O_RDWR);
	/* Averigua la longitud del fichero */ 
  	if (fstat(fd_fichero, &bstat)<0) {
   		perror("Error en fstat del fichero"); close(fd_fichero);
   		exit(1);
 	}
  	/* Se proyecta el fichero */
	if((bot1 = (DatosMemCompartida*)mmap(NULL, bstat.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd_fichero, 0)) == MAP_FAILED) {
		printf("Error en la proyeccion del fichero\n");
		exit(1);
	}
	bot2 = bot1 + 1;
	close(fd_fichero);

	while(1) {
		if(bot1->esfera.radio < 0)
			break;
		//bot jugador 1
		if(bot1->esfera.centro.y > (bot1->raqueta.y2+(bot1->raqueta.y1-bot1->raqueta.y2)/2.0f))
			bot1->accion = 1;
		else if(bot1->esfera.centro.y < (bot1->raqueta.y2+(bot1->raqueta.y1-bot1->raqueta.y2)/2.0f))
			bot1->accion = -1;
		else
			bot1->accion = 0;
		//bot jugador 2
		if(bot2->esfera.centro.y > (bot2->raqueta.y2+(bot2->raqueta.y1-bot2->raqueta.y2)/2.0f))
			bot2->accion = 1;
		else if(bot2->esfera.centro.y < (bot2->raqueta.y2+(bot2->raqueta.y1-bot2->raqueta.y2)/2.0f))
			bot2->accion = -1;
		else
			bot2->accion = 0;
		usleep(25000);
	}
	munmap(bot1,bstat.st_size);
}
