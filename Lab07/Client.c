#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/wait.h>
//#include "sockop.h"
#define MAXDATASIZE 9000
int main(int argc, char *argv[])
{
int sockfd,clientfd;
struct sockaddr_in dest;
char buffer[300];
char input;
int s,r;
int numbytes;
char buf[MAXDATASIZE];
struct hostent *he;
// connector’s address information

 
// if no command line argument supplied
if(argc != 2)
{
    fprintf(stderr, "Client-Usage: %s the_client_hostname\n", argv[0]);
    // just exit
    exit(1);
}

// get the host info
if((he=gethostbyname(argv[1])) == NULL)
{
    perror("gethostbyname()");
    exit(1);
}
else
    printf("Client-The remote host is: %s\n", argv[1]);
	
/* create socket */

	sockfd = socket(PF_INET, SOCK_STREAM, 0);

/* initialize value in dest */
	bzero(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(9000);
	dest.sin_addr.s_addr = inet_addr("192.168.140.130");
// zero the rest of the struct
	memset(&(dest.sin_zero), '\0', 8);
/* Connecting to server */
	clientfd = connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));
	printf("connect success\n");

/* Receive message from the server and print to screen */
	bzero(buffer, 128);
	printf("please input :\n");
	scanf("%s",&input);
	s = send(clientfd, &input, sizeof(input), 0);
	//s = write(clientfd, &input, sizeof(input));
	printf("send success %d\n",s);
	r = recv(clientfd, &buffer, sizeof(buffer), 0);
	//r = read(clientfd, &buffer, sizeof(buffer));
	printf("receive success %d\n",r);
	printf("%s\n",buffer);

/* Close connection */
	close(sockfd);

	return 0;
}
