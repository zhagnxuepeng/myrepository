/*
 *chrdrvtest驱动的测试程序
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <error.h>

#include "chrdrvtest.h"

#define N 128

int main(int argc, const char *argv[])
{
	int fd;
	int ret;
	char buf[N];

	if ((fd = open("/dev/memzone", 0666)) < 0) {//必须是666
		printf("open fail \n");
		return -1;
	}

	bzero(buf, N);

	printf("Please input a string to the char device：\n");

	fgets(buf, sizeof(buf), stdin);

	if (write(fd,buf,sizeof(buf)) < 0) {
		perror("ret");
		close(fd);
		return -1;
	}

	printf("\nA:start thread A, and print the memroy info you just input \n  \
			B:start thread B, and print the memroy info \n \
			C:Thread A increase the value of the memroy,and thread B stoped, and after the thread B restart \n \
			D:Thread B finished by thread A \n \
			E:Both thread A and B ended \n");

	while(1) {
		int temp; 
		temp = getchar();

		switch (temp) {
		case 'A':
			ioctl(fd, A);
			bzero(buf, N);
			read(fd, buf, N);
			printf("info from kernel:%s \n", buf);
			break;
		case 'B':
			ioctl(fd, B);
			bzero(buf, N);
			read(fd, buf, N);
			printf("info from kernel:%s \n", buf);
			break;
		case 'C':
			ioctl(fd, C);
			bzero(buf, N);
			read(fd, buf, N);
			printf("info from kernel:%s \n", buf);
			break;
		case 'D':
			ioctl(fd, D);
			bzero(buf, N);
			read(fd, buf, N);
			printf("info from kernel:%s \n", buf);
			break;
		case 'E':
			ioctl(fd, E);
			bzero(buf, N);
			read(fd, buf, N);
			printf("info from kernel:%s \n", buf);
			break;
		default:
			break;
		}
	}
	return 0;
}
