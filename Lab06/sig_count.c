/*
* sig_count.c
*/
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

sig_atomic_t sigusr1_count = 0 ;

void handler(int signal_number)
{
printf("here is handler.\n");
++sigusr1_count; /* add one, protected atomic action */
}
int main ()
{
struct sigaction sa ;
struct timespec req ;
int retval;

/* set the sleep time to 10 sec */
memset(&req, 0 , sizeof (struct timespec));
req.tv_sec = 10 ;
req.tv_nsec = 0 ;

/* register handler to SIGUSR1 */
memset(&sa , 0 , sizeof(sa));
sa.sa_handler = handler;

sigaction(SIGUSR1 , &sa , NULL);
printf( "Process (%d) is catching SIGUSR1 . . . \n", getpid());

/* sleep 10 sec */
do{
retval = nanosleep(&req, &req+2);
printf("retval:%d\n",retval );
}while(-1);

printf("SIGUSR1 was raised %d times \n" , sigusr1_count);
return 0 ;
}
