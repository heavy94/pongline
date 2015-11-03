#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>



char tuberia[]="logger.txt";
char buffer[200];
int n_bytes = 1000;

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
		printf("%s\n", buffer);
		if(buffer[0]=='%') break;
	}
	close(fd_fifo);
	unlink(tuberia);
}
