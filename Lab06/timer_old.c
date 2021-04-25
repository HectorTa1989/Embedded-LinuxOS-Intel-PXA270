/*
 *timer.c
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int count = 0;

void timer_handler (int signum)
{
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
	//printf("timer expired %d times\n",++count);
}

void handler(int signo, siginfo_t *info, void *context)
{	
	struct itimerval timer;

	/* Configure the timer to expire after 250 msec */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 250000;

	/* Reset the timer back to 250 msec after expired */
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 250000;
	
	printf("start...\n");
	setitimer (ITIMER_VIRTUAL, &timer,NULL);
	
	while(1){
		if(count == 10){
			kill(getpid(),SIGUSR2);
			break;		
		}
		if(count == 25){
			printf("reset...\n");
			count = 0;	
		}
			
	}

}

void handler2(int signo, siginfo_t *info, void *context)
{	
	struct itimerval timer2;
	
	timer2.it_value.tv_sec = 1;
	timer2.it_value.tv_usec = 0;

	timer2.it_interval.tv_sec = 1;
	timer2.it_interval.tv_usec = 0;

	printf("slow...\n");

	setitimer (ITIMER_VIRTUAL, &timer2,NULL);

	while(1){
		if(count == 15){
			kill(getpid(),SIGUSR1);
			break;
	
		}
			
	}


}

int main(int argc, char **argv)
{
	struct sigaction sa, my_action, my_action2;

	memset (&sa , 0 , sizeof(sa)) ;
        sa.sa_handler = &timer_handler;
        sigaction (SIGVTALRM, &sa , NULL) ;

	memset(&my_action, 0, sizeof (struct sigaction));
	my_action.sa_flags = SA_SIGINFO;
   	my_action.sa_sigaction = handler;
   	sigaction(SIGUSR1, &my_action, NULL);

	memset(&my_action2, 0, sizeof (struct sigaction));
	my_action2.sa_flags = SA_SIGINFO;
   	my_action2.sa_sigaction = handler2;
   	sigaction(SIGUSR2, &my_action2, NULL);

	while(1)
	{
		kill(getpid(),SIGUSR1);
	}

	
	return 0;
}
