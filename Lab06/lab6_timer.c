#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h> 
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"

int fd,ret;
float count=0;
unsigned short key;
void timer_handler(int signum)
{ 
  /*_7seg_info_t data;
  
  data.Mode = _7SEG_MODE_PATTERN;
  data.Which = _7SEG_ALL;
  data.Value = ;
  ioctl(fd, _7SEG_IOCTL_SET, &data);*/
  	//printf("timer expired %d times\n",++count);
	
	count++;
	int c=0;
	_7seg_info_t data;

	
	int t;
	int i = 1;
	int n,a;
	n=count;
	if(n > 9)
        {
	  c += 32768;
        }
	do
	{
        a=n%10;
	if (a==0)
          t = 63;
        if (a==1)
          t = 6;
        if (a==2)
          t = 91;
        if (a==3)
          t = 79;
        if (a==4)
          t = 102;
        if (a==5)
          t = 109;
        if (a==6)
          t = 125;
        if (a==7)
          t = 7;
        if (a==8)
          t = 127;
        if (a==9)
          t = 103;  
		c += t*i;
		n = n/10;
		i *= 256;
	}
	while(n > 0);
	data.Mode = _7SEG_MODE_PATTERN;
	data.Which = _7SEG_ALL;
	data.Value = c;
	ioctl(fd, _7SEG_IOCTL_SET, &data);
} 

void handler(int signo, siginfo_t *info, void *context)
{
  int x;
  
  struct sigaction SA_SIGALRM;
  struct itimerval timer;

  memset(&SA_SIGALRM,0,sizeof (SA_SIGALRM));
  SA_SIGALRM.sa_handler=&timer_handler;
  sigaction(SIGALRM,&SA_SIGALRM,NULL);

  timer.it_value.tv_sec=0;
  timer.it_value.tv_usec=0;

  timer.it_interval.tv_sec=0;
  timer.it_interval.tv_usec=0;
    
  setitimer(ITIMER_REAL,&timer,NULL);  
  
  ioctl(fd,KEY_IOCTL_CLEAR,key);
  
  printf("Handler take work.\n");
  while(1)
  {
  ret=ioctl(fd,KEY_IOCTL_CHECK_EMPTY,&key);
      if(ret<0)
      {
       
        sleep(1);
        continue;
      }
      ret=ioctl(fd,KEY_IOCTL_GET_CHAR,&key);
      if((key&0xff)=='*')
      {
        printf("start\n");
        timer.it_value.tv_sec=0;
        timer.it_value.tv_usec=100000;

        timer.it_interval.tv_sec=0;
        timer.it_interval.tv_usec=100000;
    
        setitimer(ITIMER_REAL,&timer,NULL);
        break;
      }
      else if((key&0xff)=='0')
      {
        printf("set count=0\n");
        count=0;
      }

  }
} 

int main()
{ 

  int a;
  

  struct sigaction my_action;
  struct sigaction SA_SIGALRM;
  struct itimerval timer;

  memset(&my_action, 0, sizeof (struct sigaction));
  my_action.sa_flags = SA_SIGINFO;
  my_action.sa_sigaction = handler;
  sigaction(SIGUSR1, &my_action, NULL);
  
  memset(&SA_SIGALRM,0,sizeof (SA_SIGALRM));
  SA_SIGALRM.sa_handler=&timer_handler;
  sigaction(SIGALRM,&SA_SIGALRM,NULL);
  
  
  timer.it_value.tv_sec=0;
  timer.it_value.tv_usec=100000;

  timer.it_interval.tv_sec=0;
  timer.it_interval.tv_usec=100000;
    
  setitimer(ITIMER_REAL,&timer,NULL);

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
      if((key&0xff)=='#')
      {
      kill(getpid(),10);
      }
      
    }
   
  return 0;
}
