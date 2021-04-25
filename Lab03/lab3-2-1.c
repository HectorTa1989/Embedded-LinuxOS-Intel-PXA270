#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include"asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"

pid_t childpid;
pid_t pid;
int x;
int fd;
int retval;
unsigned short data;

void childfunc(void)
{
		printf("enter 1-15 or 0 to exit\n");
		scanf("%d",&x);

	exit(x);
}

int parentfunc()
{


	do
	{
		pid = waitpid(childpid,&x,WNOHANG);



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


	}while(pid != childpid);
	return 0;
}

int main(int argc,char *argv[])
{

	if((fd = open("/dev/lcd",O_RDWR))<0)
	{
		printf("open /dev/lcd fail.\n");
		exit(-1);
	}
	data = LED_ALL_ON;
	ioctl(fd,LED_IOCTL_SET,&data);
	do
	{
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
	}while(x!=0);
	return 0;
}


		

		
