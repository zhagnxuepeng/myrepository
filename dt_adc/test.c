
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char *argv[])
{

	int  fd;
	int  data  =0 ;

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


	for(;;)
	{
		read(fd,&data,sizeof(data));
		printf("data  = %fv\n",data*1.8/4096);

		sleep(1);
	}
	return 0;
}
