#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>

pid_t childpid;


int main()
{
	int x=1;

		childpid = fork();
		if (childpid>=0)
		{
			if (childpid == 0)
			{			
				printf("child process x = %d\n",x);
				sleep(2);
				printf("child process x = %d\n",x);
				
			}
			else
			{
				x=99;
				printf("father process x = %d\n",x);
				sleep(3);
			}
		}
		else
		{
			perror("fork");
			exit(0);
		}
	return 0;

}

