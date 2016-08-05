#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <error.h>
#include "bsp.h"
int main(int argc, const char *argv[])
{

	int  fd;
	char  buf[1024];
	char  buff[1024] = "hello world";
	/*
	if(argc  <  2)
	{
		printf("usage  <file  name argv[1]>  = %s\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	*/
	fd  = open("/dev/zhangxp",O_RDWR);
	if(fd  <  0)
	{
		perror("open fail ");
		exit(EXIT_FAILURE);
	}

	printf("open  success   \n");
	{
		write(fd,buff,sizeof(buff));
		read(fd,buf,sizeof(buf));
		printf("buf  = %s\n",buf);
	}

	while(1)
	{
		int temp;

		temp = getchar();
		switch (temp) {
		case 'A':
			ioctl(fd, A);
			break;
		case 'B':
			ioctl(fd, B);
			break;
		case 'C':
			ioctl(fd, C);
		case 'Q':
			break;
		default:
			break;
		}
	}
	return 0;
}
