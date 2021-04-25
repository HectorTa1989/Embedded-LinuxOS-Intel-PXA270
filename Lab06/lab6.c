#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h> 

#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#include "linux-2.4.x/include/asm-armnommu/arch-creator/lib/creator_s3c4510_lcd.h"

//arm-unknown-linux-gnu-gcc -o lab6 lab6.c -L /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/lib/ -I /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/include/ -I /home/lab616/microtime/linux/include/
//insmod lib/modules/2.6.15.3/kernel/drivers/char/creator-pxa270-lcd.ko

sig_atomic_t count = 0;

void timer_handler (int signum)
{	
	//_7seg_info_t data;
	int fd;			
	int n,a,b,c,d;
	int x,y,z,w;
	int sum;

	++count;	
	n=count;

	///// 4 /////////////////////////
	a = n%10;
	if (a==0)   x = 0x3F;
	if (a==1)   x = 0x06;
	if (a==2)   x = 0x5B;
	if (a==3)   x = 0x4F;
	if (a==4)   x = 0x66;
	if (a==5)   x = 0x6D;
	if (a==6)   x = 0x7D;
	if (a==7)   x = 0x07;
	if (a==8)   x = 0x7F;
	if (a==9)   x = 0x6F;

	///// 3 ////////////////////////
	b = (n%100)/10;
	if (b==0)   y = 0xBF00;
	if (b==1)   y = 0x8600;
	if (b==2)   y = 0xDB00;
	if (b==3)   y = 0xCF00;
	if (b==4)   y = 0xE600;
	if (b==5)   y = 0xED00;
	if (b==6)   y = 0xFD00;
	if (b==7)   y = 0x8700;
	if (b==8)   y = 0xFF00;
	if (b==9)   y = 0xEF00;

	///// 2 ////////////////////////
	c = (n%1000)/100;
	if (c==0)   z = 0x3F;
	if (c==1)   z = 0x06;
	if (c==2)   z = 0x5B;
	if (c==3)   z = 0x4F;
	if (c==4)   z = 0x66;
	if (c==5)   z = 0x6D;
	if (c==6)   z = 0x7D;
	if (c==7)   z = 0x07;
	if (c==8)   z = 0x7F;
	if (c==9)   z = 0x6F;

	///// 1 ////////////////////////
	d = n/1000;
	if (d==0)   w = 0x3F;
	if (d==1)   w = 0x06;
	if (d==2)   w = 0x5B;
	if (d==3)   w = 0x4F;
	if (d==4)   w = 0x66;
	if (d==5)   w = 0x6D;
	if (d==6)   w = 0x7D;
	if (d==7)   w = 0x07;
	if (d==8)   w = 0x7F;
	if (d==9)   w = 0x6F;


	sum = x + y + z*0x10000 +w*0x1000000;
	printf("sum= %X\n", sum);
	printf("count=%d,%d,%d.%d\n",d,c,b,a);

	
	///// LCD //////
/*	if((fd = open("/dev/lcd", O_RDWR)) < 0)
	{
		printf("open failed\n");
	}

	ioctl(fd,_7SEG_IOCTL_ON,NULL);
	
	data.Mode = _7SEG_MODE_PATTERN;
	data.Which =_7SEG_ALL;
	data.Value = sum;
	ioctl(fd,_7SEG_IOCTL_SET,&data);
*/	
}

void handler1(int signo, siginfo_t *info, void *context)
{
	struct itimerval timer1;

	timer1.it_value.tv_sec = 0;
	timer1.it_value.tv_usec = 250000;

	timer1.it_interval.tv_sec = 0;
	timer1.it_interval.tv_usec = 250000;
	
	printf("start...\n");
	setitimer (ITIMER_VIRTUAL, &timer1,NULL);
	
	ioctl(fd,KEY_IOCTL_CLEAR,key);	

	while(1)
	{
		ret=ioctl(fd,KEY_IOCTL_CHECK_EMPTY,&key);
		if(ret<0)
		{       
			sleep(1);
			continue;
		}
		ret=ioctl(fd,KEY_IOCTL_GET_CHAR,&key);
		if((key & 0xff) == '#') //pause
		{
			kill(getpid(),SIGUSR2);
			break;
		}
		if((key & 0xff) == '0') //reset
		{
			printf("reset...\n");
			count = 0;
		}
	}
			
	}
}

void handler2(int signo, siginfo_t *info, void *context)
{	
	struct itimerval timer2;

	timer2.it_value.tv_sec = 0;
	timer2.it_value.tv_usec = 0;

	timer2.it_interval.tv_sec = 0;
	timer2.it_interval.tv_usec = 0;
	
	printf("start...\n");
	setitimer (ITIMER_VIRTUAL, &timer2,NULL);
	
	ioctl(fd,KEY_IOCTL_CLEAR,key);	

	while(1)
	{
		ret=ioctl(fd,KEY_IOCTL_CHECK_EMPTY,&key);
		if(ret<0)
		{       
			sleep(1);
			continue;
		}
		ret=ioctl(fd,KEY_IOCTL_GET_CHAR,&key);
		if((key & 0xff) == '*') //start
		{
			kill(getpid(),SIGUSR1);
			break;
		}
		if((key & 0xff) == '0') //reset
		{
			printf("reset...\n");
			count = 0;
		}
			
	}
}

int main(int argc, char **argv)
{
	int fd, ret;
	struct sigaction sa, my_action1, my_action2;
	
	
	// DISPLAY
	memset (&sa , 0 , sizeof(sa)) ;
        sa.sa_handler = &timer_handler;
        sigaction (SIGVTALRM, &sa , NULL) ;
	// START & RESET
	memset(&my_action, 0, sizeof (struct sigaction));
	my_action1.sa_flags = SA_SIGINFO;
   	my_action1.sa_sigaction = handler1;
   	sigaction(SIGUSR1, &my_action1, NULL);
	// PAUSE & RESET
	memset(&my_action2, 0, sizeof (struct sigaction));
	my_action2.sa_flags = SA_SIGINFO;
   	my_action2.sa_sigaction = handler2;
   	sigaction(SIGUSR2, &my_action2, NULL);
	
	// KEYBOARD
	if ((fd = open("/dev/lcd", O_RDWR)) < 0)
  	{
    		printf("open faild\n");
    		return (-1);
  	}
  	ioctl(fd,KEY_IOCTL_CLEAR,key);	
	
	while(1)
	{
		ret=ioctl(fd,KEY_IOCTL_CHECK_EMPTY,&key);
		if(ret<0)
		{       
			sleep(1);
			continue;
		}
		ret=ioctl(fd,KEY_IOCTL_GET_CHAR,&key);
		if((key & 0xff) == '*') //start
		{
			kill(getpid(),SIGUSR1);
		}
		
	}
	
	return 0;
	
}
