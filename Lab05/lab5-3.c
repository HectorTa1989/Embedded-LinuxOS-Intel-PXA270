#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<time.h>
#include<unistd.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"

#define max(a,b)((a>b)?a:b)

void ChildProcess(int *pfd,int n,int M)
{
	int sum=0,i;
	char buffer[100];
	close(pfd[0]);
	

	for(i=n*M+1;i<=(n+1)*M;i++)
		sum+=i;
	printf("sum = %d\n",sum);
	memset(buffer,0,100);
	sprintf(buffer,"%d",sum);
	write(pfd[1],buffer,sizeof(buffer));
	close(pfd[1]);
	exit(EXIT_SUCCESS);
}


int main(int argc,char *argv[])
{
	int N,M,totalsum=0,sum,i;
	char buffer[100];
	printf("N = : ");
	scanf("%d",&N);
	printf("\nM = : ");
	scanf("%d",&M);
	printf("\n");
	
	int pfd[N][2];
	int cpid[N];
	fd_set rfds,arfds;
	struct timeval tv;
	int retval;
	int max_fd;
	int fd_index;
	
	for(i=0;i<N;i++)
		pipe(pfd[i]);
	for(i=0;i<N;i++)
	{	
		cpid[i] = fork();
		if (cpid[i] == 0)
		ChildProcess(pfd[i],i,M);
	}
	
	for(i=0;i<N;i++)
		close(pfd[N][1]);
	
	FD_ZERO(&rfds);
	FD_ZERO(&arfds);
	for(i=0;i<N;i++)
		FD_SET(pfd[i][0],&arfds);
	
	max_fd = max(pfd[0][0],pfd[1][0]);
	for(i=2;i<N;i++)
		max_fd = max(max_fd,pfd[i][0]);
	max_fd+=1;
	

	
	while(1)
	{
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		memcpy(&rfds,&arfds,sizeof(rfds));
		retval = select(max_fd,&rfds,NULL,NULL,&tv);

		if(retval == -1)
		{
			perror("select()");
			exit(EXIT_FAILURE);
		}
		else if(retval)
			printf("data is available\n");
		else
		{
			printf("no data");
			break;
		}
		
		for(fd_index=0;fd_index<max_fd;fd_index++)
		{
			if(!FD_ISSET(fd_index,&rfds))
				continue;
			retval = read(fd_index,buffer,sizeof(buffer));
			sum = atoi(buffer);
			if(retval > 0)
			{
				sum = sum%10000;
				totalsum = totalsum%10000;
				totalsum += sum;
			}
			else if(retval < 0)
				perror("pipe read()");
			else
				close(fd_index);
			FD_CLR(fd_index,&arfds);
		}
	}
	
	_7seg_info_t data;
	int fd;
	int x;
	if((fd = open("/dev/lcd",O_RDWR))<0)
	{
		printf("Open /dev/lcd faild \n");
	}
	
	x=(totalsum%10);
	totalsum=totalsum/10;
	x=x+(totalsum%10)*16;
	totalsum=totalsum/10;
	x=x+(totalsum%10)*16*16;
	totalsum=totalsum/10;
	x=x+totalsum*16*16*16;
	//printf("x = %d\n",x);
	data.Mode =_7SEG_MODE_HEX_VALUE;
	data.Which=_7SEG_ALL;
	data.Value=x;
	ioctl(fd,_7SEG_IOCTL_SET,&data);

	close(fd);
	
	return 0;
}
	
	
	
	
	
	
	