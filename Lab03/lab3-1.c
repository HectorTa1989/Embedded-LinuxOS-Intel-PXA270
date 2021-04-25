#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/fcntl.h>
#include<sys/ioctl.h>

int x=1;

void *pid1()
{
	printf("the 2 thread x =%d\n",x);

}
void *pid2()
{
	printf("the 2 thread x =%d\n",x);

}


int main()
{	
	printf("the process x = %d\n",x);
	pthread_t t1,t2;
	pthread_create(&t1,NULL,pid1,NULL);
	sleep(1);
	pthread_create(&t2,NULL,pid2,NULL);
	sleep(1);
	return 0;

}

