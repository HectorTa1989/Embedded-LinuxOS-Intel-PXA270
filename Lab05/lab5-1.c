#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include"asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"

pid_t childpid;
pid_t pid;
int pfd[2];
int fd;
int retval;
unsigned short data;
int max_fd;
int fd_index;
fd_set rfds,arfds;
struct timeval tv;

void childfunc(void)
{
	int x;
	char buffer[100];
	close(pfd[0]);
	while(1)
	{

		printf("enter 1-15 or 0 to exit QQQQQQQ\n");
		scanf("%d",&x);
		sprintf(buffer,"%d",x);
		write(pfd[1],buffer,strlen(buffer));

	}
	close(pfd[1]);
	exit(EXIT_SUCCESS);
}

void parentfunc()
{
	int retval;
	int x=16;
	//int i;
	char buffer[100];
	
	
	FD_ZERO(&rfds);
	FD_ZERO(&arfds);
	FD_SET(pfd[0],&arfds);
	max_fd = pfd[0]+1;
	tv.tv_sec = 0;
	tv.tv_usec = 10000;

	//close(pfd[1]);
	while(1)
	{
		//i=1;

		memcpy(&rfds,&arfds,sizeof(rfds));
		retval = select(max_fd,&rfds,NULL,NULL,&tv);
		if(retval == -1)
		{
			perror("select()");
			exit(EXIT_FAILURE);
		}
		else if(retval)
			printf("data is available");
		else
		{
			printf("no data");
			display(x);
		}
		
		for(fd_index=0;fd_index<max_fd;fd_index++)
		{
			if(!FD_ISSET(fd_index,&rfds))
				continue;
			retval = read(fd_index,buffer,100);


			if(retval == 0)
				close(fd_index);
				close(pfd[0]);
			FD_CLR(fd_index,&arfds);
		}
		//pid = waitpid(childpid,NULL,WNOHANG);

			read(pfd[0],buffer,100);
		//if (i==1)
			x = atoi(buffer);
			display(x);
		//else
		//	x=x;	

		//printf("x = %d\n",&x);
	}
	exit(0);
}
	
int display(int x)
{
		
		if(x==0)
		{
			close(pfd[0]);
			exit(0);
		}
		if (x==1)
		{
			data = LED_D16_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==2)
		{
			data = LED_D15_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==3)
		{
			data = LED_D16_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D15_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==4)
		{
			data = LED_D14_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==5)
		{
			data = LED_D16_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D14_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==6)
		{
			data = LED_D15_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D14_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==7)
		{
			data = LED_D15_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D14_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D16_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==8)
		{
			data = LED_D13_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==9)
		{
			data = LED_D16_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D13_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==10)
		{
			data = LED_D13_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D15_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==11)
		{
			data = LED_D13_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D16_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D15_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);

		}
		if (x==12)
		{
			data = LED_D13_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D14_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==13)
		{
			data = LED_D13_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D16_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D14_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}
		if (x==14)
		{
			data = LED_D15_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D14_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D13_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);

		}
		if (x==15)
		{
			data = LED_D15_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D14_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D16_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
			data = LED_D13_INDEX;
			ioctl(fd,LED_IOCTL_BIT_SET,&data);
		}

	
		sleep(1);
		data = LED_ALL_OFF;
		ioctl(fd,LED_IOCTL_SET,&data);
		sleep(1);	
		if(x>255)
		{
			x=x/256;
		}
		sleep(1);

	return 0;
}

int main(int argc,char *argv[])
{
	pid_t cpid;
	
	if(pipe(pfd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	if((fd = open("/dev/lcd",O_RDWR))<0)
	{
		printf("open /dev/lcd fail.\n");
		exit(EXIT_FAILURE);
	}
	
	data = LED_ALL_ON;
	ioctl(fd,LED_IOCTL_SET,&data);

	childpid = fork();
	if (childpid>=0)
	{
		if (childpid == 0)
		{
			childfunc();
		}
		else
		{
			parentfunc();
		}
	}
	else
	{
		perror("fork");
		exit(0);
	}
	return 0;
}


		

		
