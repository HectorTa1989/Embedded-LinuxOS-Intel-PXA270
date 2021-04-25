#include<errno.h>
#include<pthread.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include"asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *calculate()
{
	int ret1,ret2;
	int fd1,fd2;
	int a1,a2;
	char buffer1[100],buffer2[100];
	int i= 100;
	
	while(i)
	{	
		int rc;	
		fd1 = open("./file1.txt",O_RDWR);
		fd2 = open("./file2.txt",O_RDWR);
		rc=pthread_mutex_lock(&mutex);
		if(rc)
		{
			printf("Failed lock");
		}
		if(fd1<0)
		{
			printf("Opne file1.txt error \n");
			exit(-1);
		}
		if(fd2<0)
		{
			printf("Opne file2.txt error \n");
			exit(-1);
		}


	memset(buffer1,0,100);
	memset(buffer2,0,100);
	ret2 = read(fd2,buffer2,100);
	if(ret2<0)
	{
		perror("read file2.txt");
		exit(-1);
	}
	ret1 = read(fd1,buffer1,100);
	if(ret1<0)
	{
		perror("read file1.txt");
		exit(-1);
	}
	a1 = atoi(buffer1);
	a2 = atoi(buffer2);



	lseek(fd1,0,SEEK_SET);
	lseek(fd2,0,SEEK_SET);
	memset(buffer1,0,100);
	memset(buffer2,0,100);
	sprintf(buffer1,"%4d",a2%10000);
	ret1 = write(fd1,buffer1,strlen(buffer1));
	if(ret1<0)
	{
	perror("write file1.txt");
	exit(-1);
	}
	sprintf(buffer2,"%4d",(a2+a1)%10000);
	ret2 = write(fd2,buffer2,strlen(buffer2));	
	if(ret2<0)
	{
	perror("write file2.txt");
	exit(-1);
	}
	
	rc = pthread_mutex_unlock(&mutex);
	if(rc)
	{
		printf("Failed unlock");
	}


	close(fd1);
	close(fd2);
	i--;
	}

}

int main(int argc,char **argv)
{
	pthread_t t1,t2;
	int rc=0;
	
	rc = pthread_mutex_lock(&mutex);
	if(rc)
	{
		printf("Failed lock");
	}
	pthread_create(&t1,NULL,calculate,NULL);
	pthread_create(&t2,NULL,calculate,NULL);
	rc = pthread_mutex_unlock(&mutex);
	if(rc)
	{
		printf("Failed unlock");
	}	

	sleep(3);

	int a=1,x,y;
	int fd,fd1;
	int ret;
	char buffer[100];
	_7seg_info_t data;
	if((fd1 = open("/dev/lcd",O_RDWR))<0)
	{
		printf("Open /dev/lcd faild \n");
	}
	fd = open("./file2.txt",O_RDWR);
	if(fd<0)
	{
		printf("Opne file2.txt error \n");
		exit(-1);
	}
	memset(buffer,0,100);
	ret = read(fd,buffer,100);
	if(ret<0)
	{
		perror("read file2.txt");
		exit(-1);
	}
	x = atoi(buffer);
	y=(x%10);
	x=x/10;
	y=y+(x%10)*16;
	x=x/10;
	y=y+(x%10)*16*16;
	x=x/10;
	y=y+x*16*16*16;
	data.Mode =_7SEG_MODE_HEX_VALUE;
	data.Which=_7SEG_ALL;
	data.Value=y;
	ioctl(fd1,_7SEG_IOCTL_SET,&data);

	close(fd);
	close(fd1);

	return 0;
}



	
