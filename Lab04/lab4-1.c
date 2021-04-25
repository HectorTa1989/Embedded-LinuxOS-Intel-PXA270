#include<errno.h>
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


#define SEM_MODE 0666
#define SEM_KEY 1122334455

int sem;

int P(int s)
{
	struct sembuf sop;
	sop.sem_num = 0;
	sop.sem_op = -1;
	sop.sem_flg = 0;
	
	if(semop(s,&sop,1)<0)
	{
		fprintf(stderr,"P() semop failed : %s \n",strerror(errno));
		return -1;
	}
	else
	{
		return 0;
	}
}

int V(int s)
{
	struct sembuf sop;
	sop.sem_num = 0;
	sop.sem_op  = 1;
	sop.sem_flg = 0;
	
	if(semop(s,&sop,1)<0)
	{
		fprintf(stderr,"V() semop failed : %s \n",strerror(errno));
		return -1;
	}
	else
	{
		return 0;
	}
}

void calculate()
{
	int ret1,ret2;
	int fd1,fd2;
	int a1,a2;
	char buffer1[100],buffer2[100];
	int i= 100;
	
	while(i)
	{		
		fd1 = open("./file1.txt",O_RDWR);
		fd2 = open("./file2.txt",O_RDWR);
		P(sem);
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
	

	V(sem);

	close(fd1);
	close(fd2);
	i--;
	}

}

int main(int argc,char **argv)
{
	int childpid;
	int status;
	

	sem = semget(SEM_KEY,1,IPC_CREAT|IPC_EXCL|SEM_MODE);
	if(sem<0)
	{
		fprintf(stderr,"Creation of semaphore %ld failed : %s \n",SEM_KEY,strerror(errno));
		exit(-1);
	}
	if(semctl(sem,0,SETVAL,1)<0)
	{
		fprintf(stderr,"Unable to initialize semaphore : %s \n",strerror(errno));
		exit(0);	
	}

	printf("semaphore %ld has been created & initialize to 1 \n",SEM_KEY);


	if((childpid = fork())>0)
	{
		calculate();
		waitpid(childpid,&status,0);
	}
	else if (childpid == 0)
	{
		calculate();
		exit(0);
	}
	else
	{
		perror("fork");
		exit(-1);
	}
	

	if(semctl(sem,0,IPC_RMID,0)<0)
	{
		fprintf(stderr,"%s : unable to remove semaphore %ld \n",argv[0],SEM_KEY);
		exit(1);
	}
	printf("semaphore %ld has been remove \n",SEM_KEY);



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



	
