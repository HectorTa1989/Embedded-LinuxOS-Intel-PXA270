/*
* timer_diff.c
*/

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* counter */
int SIGALRM_count = 0 ;
int SIGVTALRM_count = 0 ;
int SIGPROF_count = 0 ;

/* handler of SIGALRM */
void SIGALRM_handler(int signum)
{
SIGALRM_count++;
}

/* handler of SIGVTALRM */
void SIGVTALRM_handler ( int signum)
{
SIGVTALRM_count++;
}
/* handler of SIGPROF */
void SIGPROF_handler ( int signum)
{
SIGPROF_count++;
}

void IO_WORKS();

int main ( int argc , char **argv)
{
struct sigaction SA_SIGALRM, SA_SIGVTALRM, SA_SIGPROF;
struct itimerval timer;

/* Install SIGALRM_handler as the signal handler for SIGALRM */
memset(&SA_SIGALRM, 0 , sizeof (SA_SIGALRM));
SA_SIGALRM.sa_handler = &SIGALRM_handler ;
sigaction(SIGALRM, &SA_SIGALRM, NULL) ;

/* Install SIGVTALRM_handler as the signal handler for SIGVTALRM */
memset(&SA_SIGVTALRM, 0 , sizeof (SA_SIGVTALRM) ) ;
SA_SIGVTALRM.sa_handler = &SIGVTALRM_handler ;
sigaction(SIGVTALRM, &SA_SIGVTALRM, NULL) ;

/* Install SIGPROF_handler as the signal handler for SIGPROF */
memset(&SA_SIGPROF, 0 , sizeof (SA_SIGPROF));
SA_SIGPROF.sa_handler = &SIGPROF_handler ;
sigaction(SIGPROF, &SA_SIGPROF, NULL) ;

/* Configure the timer to expire after 100 msec */
timer.it_value.tv_sec = 0 ;
timer.it_value.tv_usec = 100000;

/* Reset the timer back to 100 msec after expired */
timer.it_interval.tv_sec = 0 ;
timer.it_interval.tv_usec = 100000;

/* Start timer */
setitimer(ITIMER_REAL, &timer , NULL) ;
setitimer(ITIMER_VIRTUAL, &timer , NULL) ;
setitimer(ITIMER_PROF, &timer , NULL) ;

/* Do some I/O operations */
IO_WORKS();

printf("SIGALRM_count = %d\n",SIGALRM_count);
printf("SIGVTALRM_count = %d\n",SIGVTALRM_count);
printf("SIGPROF_count = %d\n",SIGPROF_count);

return 0;
}
void IO_WORKS()
{
int fd, ret;
char buffer[100];
int i ;

/* Open/Read/Close file 300000 times */
for(i = 0; i < 300000; i++) {
if((fd = open ( "/etc/init.d/networking" , O_RDONLY)) < 0) {
perror("Open /etc/init.d/networking " ) ;
exit(EXIT_FAILURE) ;
}

do {
ret = read (fd ,buffer, 100) ;
}while (ret);

close(fd);
}
}
