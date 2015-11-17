#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>



char tuberia[]="/tmp/logger";
char buffer[10];
int n_bytes = 4;

int main()
{
	int fd_fifo;
	int bytes_leidos;

	if(mkfifo(tuberia, 0777)) {
		printf("Error al crear la tuberia\n");
		exit(1);
	}
	fd_fifo = open(tuberia, O_RDONLY);
	
	while(1) {
		bytes_leidos=read(fd_fifo, buffer, n_bytes);
		if(bytes_leidos == 0)	//Salir del bucle cuando no hay datos en la tuberia
			break;
		if(bytes_leidos < 4)
			printf("Jugador %c marca 1 punto, lleva un total de %c puntos.\n", buffer[0],  buffer[2]);
		else
			printf("Jugador %c marca 1 punto, lleva un total de %s puntos.\n", buffer[0],  buffer + 2);
	}
	close(fd_fifo);
	unlink(tuberia);
}
