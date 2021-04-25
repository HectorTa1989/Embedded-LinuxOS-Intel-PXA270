/*
* assign.c 0260021
* creat a counter.
*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#ifdef show
int fd,ret;
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#include "linux-2.4.x/include/asm-armnommu/arch-creator/lib/creator_s3c4510_lcd.h"
#endif

sig_atomic_t count = 0;
int x;
void timer_handler()
{
	++count;

}

void handler2 ()
{

	struct itimerval timer2 ;
	/* Configure the timer to expire after 0.1 sec */
	timer2.it_value.tv_sec = 0 ;
	timer2.it_value.tv_usec = 0;
	/* Reset the timer back to 0.1 sec after expired */
	timer2.it_interval.tv_sec = 0 ;
	timer2.it_interval.tv_usec = 0;
	/* Start a virtual timer */
	setitimer(ITIMER_REAL, &timer2 , NULL) ;

#ifdef show
		ret=ioctl(fd,KEY_IOCTL_CHECK_EMPTY,&key);
		if(ret<0)
		{       
			sleep(1);
			continue;
		}
		ret=ioctl(fd,KEY_IOCTL_GET_CHAR,&key);
		if((key & 0xff) == '*')
		kill(getpid(),SIGUSR1);
		else if((key & 0xff) == '0')
		count = 0;
#endif		
#ifdef noshow
		scanf("%d",&x);
		if (x == 1)
		kill(getpid(), SIGUSR1);
		else if(x == 0)
		count = 0;
#endif
}


void handler1 ()
{

	struct itimerval timer1 ;
	/* Configure the timer to expire after 0.1 sec */
	timer1.it_value.tv_sec = 0 ;
	timer1.it_value.tv_usec = 100000;
	/* Reset the timer back to 0.1 sec after expired */
	timer1.it_interval.tv_sec = 0 ;
	timer1.it_interval.tv_usec = 100000;
	/* Start a virtual timer */
	setitimer(ITIMER_REAL, &timer1 , NULL) ;

	 
}

int main (int argc, char *argv[])
{
	int time,timesmall;

////////////////////////////////////////////
	struct sigaction start_action;
	/* register handler to SIGUSR1 */
	memset(&start_action, 0, sizeof (struct sigaction));
	start_action.sa_flags = SA_SIGINFO;
	start_action.sa_sigaction = handler1;
	/* start handler1 */
	sigaction(SIGUSR1, &start_action, NULL);
//////////////////////////////////////////////
	struct sigaction stop_action;
	/* register handler to SIGUSR2 */
	memset(&stop_action, 0, sizeof (struct sigaction));
	stop_action.sa_flags = SA_SIGINFO;
	stop_action.sa_sigaction = handler2;
	/* start handler2 */
	sigaction(SIGUSR2, &stop_action, NULL);
/////////////////////////////////////////////
	struct sigaction sa ;
	/* Install timer_handler as the signal handler for SIGALRM */
	memset (&sa, 0, sizeof(sa));
	sa.sa_handler = &timer_handler;
	sigaction(SIGALRM, &sa , NULL) ;
#ifdef show
	if ((fd = open("/dev/lcd", O_RDWR)) < 0)
  	{
    		printf("open faild\n");
    			return (-1);
  	}
  	ioctl(fd,KEY_IOCTL_CLEAR,key);	
#endif


	while (1)
	{	
		time = count;
		timesmall = count % 10;
		time = time / 10;
		printf("now is %d.%d secs. \n",time,timesmall) ;
#ifdef show
		ret=ioctl(fd,KEY_IOCTL_CHECK_EMPTY,&key);
		if(ret<0)
		{       
			sleep(1);
			continue;
		}
		ret=ioctl(fd,KEY_IOCTL_GET_CHAR,&key);
		if((key & 0xff) == '*')
		kill(getpid(),SIGUSR1);
		else if((key & 0xff) == '#')
		kill(getpid(), SIGUSR2);
#endif
#ifdef noshow
		// 1 for start,2 fot stop //		
		scanf("%d",&x);
		if (x == 1)
		kill(getpid(), SIGUSR1);
		else if (x == 2)
		kill(getpid(), SIGUSR2);		
#endif
	}




	printf("Done. \n");
	return 0;
}
