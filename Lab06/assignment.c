/*
 *lab6 assignment.c
arm-unknown-linux-gnu-gcc -o assignment assignment.c -L /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/lib/ -I /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/include/ -I /home/twc/microtime/linux/include/ -lpthread
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define number_input
//#define keybord

#ifdef keybord
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#include "linux-2.4x/include/asm-armnommu/arch-creator/lib/creator_s3c4510_lcd.h"
#endif




int state = 0;
float counter = 0;
pthread_mutex_t 	mutex = PTHREAD_MUTEX_INITIALIZER;


void ChildProcess()
{
	while(1)
	{
		//printf("State is %d.\n",state);	
		switch(state)
		{
			case 1:
				counter += 1;
				printf("%f\n",counter/10);
				state = 4;
				break;
			case 2:
				counter += 0;
				printf("%f\n",counter/10);
				state = 5;
				break;
			case 3:
				counter = 0;
				printf("%f\n",counter/10);
				state = 0;
				break;
		}
		//sleep(1);                 
    	}
                         
}

void handler_start(int signum)
{
	state = 1;
	printf("State is \'start\'\n");
}

void handler_pause(int signum)
{
	state = 2;
	printf("State is \'pause\'\n");
}

void handler_reset(int signum)
{
	state = 3;
	printf("State is \'reset\'\n");
}

int main(int argc, char **argv)
{
	struct sigaction sa,usr1,usr2;
	struct itimerval timer;
	int temp;
	int inp=0;

	pthread_t start;
	
	pthread_create( &start, NULL, (void *) ChildProcess,NULL);	
	
	#ifdef keybord
	unsigned short key;
	int fd , ret;
	if ( (fd = open("/dev/lcd",O_RDWR) ) < 0 )
	return -1;
	ioctl(fd , KEY_IOCTL_CLEAR , key);
	#endif

    	

    	//Install handler_start as thr signal handler for SIGVTALRM
	memset(&sa, 0 ,sizeof(sa) );
	sa.sa_handler = &handler_start;
	usr1.sa_handler = &handler_pause;	
	usr2.sa_handler = &handler_reset;
	
	
	/* Configure the timer to expire after 250 msec */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 100000;

	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 100000;
	
	
	//setitimer (ITIMER_REAL, &timer , NULL );
	
	
	while(1)
	{
		#ifdef number_input
		pthread_mutex_lock(&mutex);

		scanf("%d",&inp);
		temp = state;
		state = inp;
		//while(1);
		
		if( (temp == 1 || temp == 4 ) && inp == 3)
			state = 1;
		pthread_mutex_unlock(&mutex);
		
		#endif

		#ifdef keybord
			
		ret = ioctl(fd , KEY_IOCTL_CHECK_EMPTY , &key);
		if(ret < 0 )
			continue;
		ret = ioctl(fd , KEY_IOCTL_GET_CHAR , &key);

		pthread_mutex_lock(&mutex);
		if( (key & 0xff) == '*' )
		inp = 1;
		if( (key & 0xff) == '#' )
		inp = 2;
		if( (key & 0xff) == '0' )
		inp = 3;
		
		/*------- prevent from 'start' to 'reset'-------*/
		temp = state;
		state = inp;
	
		if( (temp == 1 || temp == 4 ) && inp == 3)
		state = 1;
		/*----------------------------------------------*/
		pthread_mutex_unlock(&mutex);
		
		#endif
		

		//printf("Input is %d.\n",inp);
		//printf("State is %d.\n",state);
		if(state == 1)
		{
			sigaction (SIGALRM, &sa ,   NULL);
			sigaction (SIGUSR1, &usr1 , NULL);
			sigaction (SIGUSR1, &usr2 , NULL);
		}
		else if(state ==2)
		{
			sigaction (SIGUSR1, &sa ,   NULL);
			sigaction (SIGALRM, &usr1 , NULL);
			sigaction (SIGUSR1, &usr2 , NULL);
		}
		else if(state == 3)
		{
			sigaction (SIGUSR1, &sa ,   NULL);
			sigaction (SIGUSR1, &usr1 , NULL);
			sigaction (SIGALRM, &usr2 , NULL);
		}else if(state == 5)
		{
			sigaction (SIGUSR1, &sa ,   NULL);
			sigaction (SIGUSR1, &usr1 , NULL);
			sigaction (SIGUSR1, &usr2 , NULL);
		}
		//sleep(1);
		setitimer (ITIMER_REAL, &timer , NULL );
		
		#ifdef keybord

		int fd_7seg;
		int a,b,c,d;
		int result;
		_7seg_info_t	data,data2;
		
		result = counter;

		if((fd_7seg = open("/dev/lcd",O_RDWR)) < 0 )
		{
		printf("Open /dev/lcd faild.\n");
		}

		ioctl(fd_7seg, _7SEG_IOCTL_ON,NULL);
		
		data2.Mode = _7SEG_MODE_PATTERN;
        	data2.Which = _7SEG_D6_INDEX;
        	data2.Value = 0x80;
		
		ioctl(fd_7seg, _7SEG_IOCTL_SET,&data2);

		data.Mode = _7SEG_MODE_HEX_VALUE;
		data.Which = _7SEG_ALL;
		d = (result%10);
		c = (result/10)%10;
		b = (result/100)%10;
		a = result/1000;
	
		data.Value = d+c*16+b*16*16+a*16*16*16;
		ioctl(fd_7seg, _7SEG_IOCTL_SET,&data);
	
		#endif
	}

	pthread_join( start, NULL );

	
	
	return 0;
}
