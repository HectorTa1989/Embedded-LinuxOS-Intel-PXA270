#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/fcntl.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
//#include"asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#include <termios.h>


static struct termios oldt;
sig_atomic_t SIGALRM_count = 0;
int enable,reset;

void restore_terminal_settings(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void disable_waiting_for_enter(void)
{
    struct termios newt;

    /* Make terminal read 1 char at a time */
    tcgetattr(0, &oldt);  /* Save terminal settings */
    newt = oldt;  /* Init new settings */
    newt.c_lflag &= ~(ICANON | ECHO);  /* Change settings */
    tcsetattr(0, TCSANOW, &newt);  /* Apply settings */
    atexit(restore_terminal_settings); /* Make sure settings will be restored when program ends  */
}


void SIGALRM_handler(int signum)
{
	//printf("test3\n");
	if (enable == 1){
		SIGALRM_count++;
		printf(" time = %d\n",SIGALRM_count);
}
	else if (enable == 2)
	{	printf(" time = %d\n",SIGALRM_count);
	/*if(reset == 0) {
		SIGALRM_count = 0;
		printf(" time = %d\n",SIGALRM_count);}*/
	}
}


int main(int argc,char **argv)
{

	struct sigaction SA_SIGALRM;
	struct itimerval timer;
	unsigned short key;
	int fd,ret;
	
	/*if((fd = open("/dev/lcd",O_RDWR))<0)
		return (-1);
	ioctl(fd,KEY_IOCTL_CLEAR,key);*/
	
	memset(&SA_SIGALRM,0,sizeof(SA_SIGALRM));
	SA_SIGALRM.sa_handler = &SIGALRM_handler;
	sigaction(SIGALRM,&SA_SIGALRM,NULL);

	timer.it_value.tv_sec = 1;
	timer.it_value.tv_usec = 0;

	timer.it_interval.tv_sec = 1;
	timer.it_interval.tv_usec = 0;
	
	setitimer(ITIMER_REAL,&timer,NULL);
	
	printf("Enter * , # or 0 to control timer\n");
	int x1,x2,y;
	
	/*_7seg_info_t data;
	if((fd = open("/dev/lcd",O_RDWR))<0)
	{
		printf("Open /dev/lcd faild \n");
	}

	
	data.Mode =_7SEG_MODE_PATTERN;
	data.Which=_7SEG_D7_INDEX;
	data.Value=0x80;
	ioctl(fd,_7SEG_IOCTL_SET,&data);*/

	while(1)
	{
		//sleep(1);
		x1 = SIGALRM_count/10;
		x2 = SIGALRM_count%10;

		
		y=(x1%10)*16;
		x1=x1/10;
		y=y+(x1%10)*16*16;
		y=y+x2;
//printf("Value on LED: %d\n",y);
//printf("Value of x1: %d\n",x1);
//printf("Value of x2: %d\n",x2);
//printf("Key input: %d\n",getchar());
//scanf("%d, &getchar()");
int i=0;
scanf("%d", &i);
		/*data.Mode =_7SEG_MODE_HEX_VALUE;
		data.Which=_7SEG_D5_INDEX|_7SEG_D6_INDEX|_7SEG_D8_INDEX;
		data.Value=y;
		ioctl(fd,_7SEG_IOCTL_SET,&data);

		ret = ioctl(fd,KEY_IOCTL_CHECK_EMPTY,&key);
		if (ret<0)
		{
			printf("test1\n");
			sleep(1);
			continue;
		}
		ret = ioctl(fd,KEY_IOCTL_GET_CHAR,&key);
		if((key & 0xff) == '*')
		{
			printf("test2\n");
			enable = 1;
		}
		if((key & 0xff) == '#')
			enable = 0;

		if((key & 0xff) == '0')
			if (enable == 0)
				SIGALRM_count = 0;*/

		/*printf("Input condition:\n");
		scanf("%d",&enable);*/
//if (getchar() == 42)
if (i == 1)
enable = 1;
//if (getchar() == 35)
if (i == 2)
enable = 2;
//if (getchar() == 48)
if (i == 3)
{
if(enable == 2) 
SIGALRM_count = 0;
printf(" time = %d\n",SIGALRM_count);
	}
	}
	return 0;
}
					
					
				
