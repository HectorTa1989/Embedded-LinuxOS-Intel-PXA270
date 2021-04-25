#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/sem.h>
int main(int argc,char **argv)
{
	int s;
	long int key;
	key=1122334455;
	s=semget(key,1,0);
	semctl(s,0,IPC_RMID,0);
	return 0;
}
