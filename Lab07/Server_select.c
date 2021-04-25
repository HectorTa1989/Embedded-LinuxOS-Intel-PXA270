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

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in dest;
/* for setsockopt() SO_REUSEADDR, below */
int yes = 1;
int i, j;
/* newly accept()ed socket descriptor */
int newfd;
	
	/*lcd_write_info_t display;
	
	if((fd = open("/dev/lcd",O_RDWR))<0)
	return (-1);
	//ioctl(fd,KEY_IOCTL_CLEAR,key);
	ioctl(fd,LCD_IOCTL_CLEAR,NULL);*/

	/* create socket , same as client */
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	/*"address already in use" error message */
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
	perror("Server-setsockopt() error lol!");
	exit(1);
	}
	printf("Server-setsockopt() is OK...\n");
	/* initialize structure dest */
	bzero(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(9000);
	/* this line is different from client */
	dest.sin_addr.s_addr = inet_addr("192.168.140.130");

	/* Assign a port number to socket */
if(bind(sockfd, (struct sockaddr *)&dest, sizeof(dest)) < 0)
{
    perror("Server-bind() error lol!");
    exit(1);
}
printf("Server-bind() is OK...\n");

	/* make it listen to socket with max 10 connections */
	if(listen(sockfd, 10) < 0)
	{
     		perror("Server-listen() error lol!");
     		exit(1);
	}
		printf("Server-listen() is OK...\n");
	
	//signal(SIGCHLD,SIG_IGN);

	/* infinity loop -- accepting connection from client forever */
	while(1)
	{
		int clientfd, n, nbytes;;
		char input[MAXDATASIZE];
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);
		
struct timeval tv;
int fdmax, fd_index, pfd1, pfd2;
/* master file descriptor list */
fd_set master;
/* temp file descriptor list for select() */
fd_set rfds;
tv.tv_sec = 1;
tv.tv_usec = 0;
/* clear the master and temp sets */
FD_ZERO(&master);
FD_ZERO(&rfds);
/* add the listener to the master set */
FD_SET(sockfd, &master);
/* keep track of the biggest file descriptor */
fdmax = sockfd; /* so far, it's this one*/
FD_SET(fdmax, &rfds);

/* loop */
for(;;)
{
/* copy it */
rfds = master;

		//while(1)
		//{
/* don’t care about writefds and exceptfds: */
if(select(fdmax+1, &rfds, NULL, NULL, &tv) ==-1)
{
    perror("Server-select() error lol!");
    exit(1);
}
//printf("Server-select() is OK...\n");
//for(fd_index = 0; fd_index < STDIN; fd_index++)

/*run through the existing connections looking for data to be read*/
for(i = 0; i <= fdmax; i++)
{
    if(FD_ISSET(i, &rfds))
    { /* we got one... */
    if(i == sockfd)
	{
//if (FD_ISSET(STDIN, &rfds))
//{
		/* handle new connections */
		/* Wait and Accept connection */
if((clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen)) == -1)
{
    perror("Server-accept() error lol!");
}
else
{
    printf("Server-accept() is OK...\n");

FD_SET(clientfd, &master); /* add to master set */
if(clientfd > fdmax)
{ /* keep track of the maximum */
    fdmax = clientfd;
}
// inet_ntoa(const struct in_addr in): converts long to dotted-decimal notation
printf("%s: New connection from %s on socket %d\n", argv[0], inet_ntoa(client_addr.sin_addr), clientfd);
}
}
else
{
/* handle data from a client */
bzero(&buf, sizeof(buf));
if((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0)
{
/* got error or connection closed by client */
if(nbytes == 0)
 /* connection closed */
 printf("%s: socket %d hung up\n", argv[0], i);
 
else
perror("recv() error lol!");
/* close it... */
close(i);
/* remove from master set */
FD_CLR(i, &master);
}
else
{
/* we got some data from a client*/
				printf(" Input from Client: %s \n", buf);
				/*display.Count = sprintf((char *)display.Msg,"%c\n",buf);
				ioctl(fd,LCD_IOCTL_WRITE,&display);*/
for(j = 0; j <= fdmax; j++)
{
/* send to everyone! */
if(FD_ISSET(j, &master))
{
       /* except the listener and ourselves */
       if(j != sockfd && j != i)
       {
              if(send(j, buf, nbytes, 0) == -1)
                     perror("send() error lol!");
	      else 
		     printf("send back to Client: %s \n", j);
       }
}
}
                        }
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
return 0;
}
