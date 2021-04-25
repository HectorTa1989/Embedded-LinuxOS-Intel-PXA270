#include<netdb.h>
#include<netinet/in.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/fcntl.h>
#include<sys/ioctl.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
//#include "sockop.h"
//#include"asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#define MAXDATASIZE 9000
char buffer[] = "No copy";
char buffer1[] = "button1";
char buffer2[] = "button2";
char buf[MAXDATASIZE];
char snd[MAXDATASIZE], rcv[MAXDATASIZE];
int fd,ret,numbytes, x;
unsigned short key;

int main()
{
	int sockfd;
	struct sockaddr_in dest;

	
	/*lcd_write_info_t display;
	
	if((fd = open("/dev/lcd",O_RDWR))<0)
	return (-1);
	//ioctl(fd,KEY_IOCTL_CLEAR,key);
	ioctl(fd,LCD_IOCTL_CLEAR,NULL);*/

	/* create socket , same as client */
	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	/* initialize structure dest */
	bzero(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(9000);
	/* this line is different from client */
	dest.sin_addr.s_addr = inet_addr("192.168.140.130");

	/* Assign a port number to socket */
	bind(sockfd, (struct sockaddr*)&dest, sizeof(dest));

	/* make it listen to socket with max 20 connections */
	listen(sockfd, 20);
	printf("listen success\n");
	
	//signal(SIGCHLD,SIG_IGN);

	/* infinity loop -- accepting connection from client forever */
	while(1)
	{
		int clientfd, n;
		char input[MAXDATASIZE];
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);
		
		while(1)
		{
		/* Wait and Accept connection */
			clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
			printf("accept success with %d\n",clientfd);

			if(fork() == 0)
			{
				//printf("test1\n");
				//bzero(&input, sizeof(input));
				bzero(&snd, sizeof(snd));
				bzero(&rcv, sizeof(rcv));
/* read message from client */
				if((n = read(clientfd, rcv, MAXDATASIZE)) == -1)
					//errexit(" Error: read()\n");
				//recv(clientfd, &input, sizeof(input), 0);
				printf("Server-The recv() is OK...\n");
				/*display.Count = sprintf((char *)display.Msg,"%c\n",input);
				ioctl(fd,LCD_IOCTL_WRITE,&display);*/
				
/* write message back to the client */
				n = sprintf(snd, "Server: %.*s", n, rcv);
				if((n = write(clientfd, snd, n)) == -1)
					//errexit(" Error: write()\n");
				//send(clientfd, &input, sizeof(input), 0);
				//send(buf, snd, sizeof(input), 0);
				printf("Server-The send() is OK...\n");
				printf("Input from Client-Received: %s \n", snd);
				printf("send success\n");
				

				close(clientfd);
			}
			else
		//close(client) 
				close(clientfd);
		}
	}

	/* close(server) , but never get here because of the loop */
	close(sockfd);
	return 0;
}
