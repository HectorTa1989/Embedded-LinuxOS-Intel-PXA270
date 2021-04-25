#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

double count = 0;

void timer_handler (int signum)
{
	++count;
	
	printf("timer expired %f times\n",count/10);
}

void handler(int signo, siginfo_t *info, void *context)
{
    printf ("Process (%d) send SIGUSR1.n", info->si_pid);
}

int main(int argc, char **argv)
{
	struct sigaction sa;
	struct itimerval timer;
	struct itimerval timer2;

	/* Install timer_handler as the signal handler for SIGVTALRM */
	memset (&sa , 0 , sizeof(sa)) ;
    sa.sa_handler = &timer_handler;
    sigaction (SIGVTALRM, &sa , NULL) ;
    //printf ("Process (%d) is catching SIGVTALRM ....\n", getpid());
    
    struct sigaction my_action;
    
	memset(&my_action, 0, sizeof (struct sigaction));
	my_action.sa_flags = SA_SIGINFO;
    my_action.sa_sigaction = handler;
    sigaction(SIGUSR1, &my_action, NULL);
    //printf ("Process (%d) is catching SINGUSR1 ....\n", getpid());

	/* Configure the timer to expire after 100 msec */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 100000;

	/* Reset the timer back to 100 msec after expired */
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 100000;
	
	timer2.it_value.tv_sec=0;
  	timer2.it_value.tv_usec=0;

  	timer2.it_interval.tv_sec=0;
  	timer2.it_interval.tv_usec=0;
	
	
	/* Start a virtual timer */
	setitimer (ITIMER_VIRTUAL, &timer,NULL);

	/* Do busy work */
	while(1){
		if(count == 10)
			setitimer (ITIMER_VIRTUAL, &timer2,NULL);
	}			

	return 0;
}
