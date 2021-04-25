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
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<sys/shm.h>
#include"asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"


#define SEM_MODE 0666
#define SEM_KEY 1122334455
#define SHMSZ 27

int sem;
int shmid1,shmid2;
int *shm1,*shm2;

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
	int i=100,t;
	int retval1,retval2;
	int a1,a2;
	int *sum1;
	int *sum2;

	key_t key1,key2;
	
	key1 = 1111;
	key2 = 6666;

	if((shmid1 = shmget(key1,SHMSZ,0666))<0)
	{
		perror("shmget");
		exit(1);
	}
	
	if((shm1 = shmat(shmid1,NULL,0)) == (int *) -1)
	{
		perror("shmat");
		exit(1);
	}
			
	sum1 = shm1;
	
	if((shmid2 = shmget(key2,SHMSZ,0666))<0)
	{
		perror("shmget");
		exit(1);
	}
	
	if((shm2 = shmat(shmid2,NULL,0)) == (int *) -1)
	{
		perror("shmat");
		exit(1);
	}
	sum1 = shm1;
	sum2 = shm2;

	while(i)
	{		
		P(sem);
		a1 = *sum1;
		a2 = *sum2;
		*sum1 = a2;
		*sum2 = (a1+a2)%10000;	
		printf("%d\n",*sum2);	
		
		V(sem);
		i--;
	}

	
	shmdt(shm1);
	shmdt(shm2);
	

}

int main(int argc,char **argv)
{
	int childpid;
	int status;

	int retval1,retval2;

	int a1=1,a2=1,t;
	int *sum1;
	int *sum2;

	key_t key1,key2;
	
	key1 = 1111;
	key2 = 6666;
	
	if((shmid1 = shmget(key1,SHMSZ,IPC_CREAT|0666))<0)
	{
		perror("shmget");
		exit(1);
	}
	
	if((shm1 = shmat(shmid1,NULL,0)) == (int *) -1)
	{
		perror("shmat");
		exit(1);
	}
			
	sum1 = shm1;
	
	if((shmid2 = shmget(key2,SHMSZ,IPC_CREAT|0666))<0)
	{
		perror("shmget");
		exit(1);
	}
	
	if((shm2 = shmat(shmid2,NULL,0)) == (int *) -1)
	{
		perror("shmat");
		exit(1);
	}
			
	sum2 = shm2;
	
	*sum1 = 1;
	*sum2 = 1;
	
	

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


	int x=0,y;
	int fd;

	x = *sum2;
	printf("x = %d\n",x);
	


		
	_7seg_info_t data;
	if((fd = open("/dev/lcd",O_RDWR))<0)
	{
		printf("Open /dev/lcd faild \n");
	}
	
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
	ioctl(fd,_7SEG_IOCTL_SET,&data);

	close(fd);

	shmdt(shm1);
	shmctl(shmid1,IPC_RMID,NULL);
	shmdt(shm2);
	shmctl(shmid2,IPC_RMID,NULL);
	return 0;
}



	
