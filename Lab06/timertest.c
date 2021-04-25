/*
* timer.c
*/
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

static int count = 0 ;

void timer_handler(int signum)
{

printf("handler pid:%d\n",getpid());
printf("timer expired %d times \n",++count) ;
}

int main ( int argc , char **argv)
{
struct sigaction sa ;
struct itimerval timer ;
int x;

/* Install timer_handler as the signal handler for SIGVTALRM */


signal(SIGALRM, timer_handler ) ;
/* Configure the timer to expire after 250 msec */
timer.it_value.tv_sec = 1 ;
timer.it_value.tv_usec = 0;
/* Reset the timer back to 250 msec after expired */
timer.it_interval.tv_sec = 1 ;
timer.it_interval.tv_usec = 0;



/* Start a virtual timer */
setitimer(ITIMER_REAL, &timer , NULL) ;



/* Do busy work */
while (1)
{
printf("so hot.\n");
printf("key in a number.\n");
scanf("%d",&x);
printf("so cold: %d.\n",x);
}



return 0 ;
}
