/*
* reaper.c
* Demonstrate the work of process reaper
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define FORKCHILD 5

volatile sig_atomic_t reaper_count = 0 ;

/* signal handler for SIGCHLD */
void Reaper ( int sig )
{
pid_t pid ;
int status ;

while ( ( pid = waitpid (-1 , &status , WNOHANG) ) > 0)
{
printf( "Child %d is terminated . \n" , pid ) ;
reaper_count++;
}
}

void ChildProcess( )
{
int rand ;

/* rand a sleep time */
srand ( time (NULL) ) ;
rand = random() % 10 + 2 ;

printf ( "Child %d sleep %d sec . \n" , getpid() , rand ) ;
sleep( rand ) ;
printf( "Child %d exit. \n" , getpid() ) ;

exit(0);
}

int main ( int argc , char *argv[] )
{
int cpid ;
int i ;

/* regist signal handler */
signal(SIGCHLD, Reaper ) ;

/* fork child processes */
for (i = 0 ; i < FORKCHILD; i++)
{
if((cpid = fork()) > 0) /* parent */
printf( "Parent forkchild process %d . \n" , cpid ) ;
else /* child */
ChildProcess();

sleep(1);
}

/* wait all child exit */
while (reaper_count != FORKCHILD)
{
printf("i am parent. \n");
sleep(1);
}
return 0 ;
}
