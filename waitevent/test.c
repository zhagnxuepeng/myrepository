
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char *argv[])
{

	int  fd;
	char  buf[1024];
	char  buff[1024] = "hello world";

	if(argc  <  2)
	{
	
		printf("usage  <file  name argv[1]>  = %s\n",argv[0]);
		exit(EXIT_FAILURE);
	
	}

	fd  = open(argv[1],O_RDWR);
	if(fd  <  0)
	{
	
		perror("open fail ");
		exit(EXIT_FAILURE);
	
	}

	printf("open  success   \n");


	read(fd,buf,sizeof(buf));
	printf("buf  = %s\n",buf);


	write(fd,buff,sizeof(buff));
	return 0;
}
