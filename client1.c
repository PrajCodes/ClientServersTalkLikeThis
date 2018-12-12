/* Author: Prajna Prabhakara 
Client program requesting a TCP connection and product service from server */

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	int c_soc_fd, port_num, msg, result;
	struct sockaddr_in servent_addr;
	struct hostent *server;

	char msg_buf[256];

	if (argc < 3)
	{
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}

	port_num = atoi(argv[2]);
//create socket at client > endpoint
	c_soc_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (c_soc_fd <0)
	{
		perror("ERROR !! Could not open client socket");
		exit(0);
	}
//Identifies the server address to connect to 
	server = gethostbyname(argv[1]);
	
	if(server ==NULL)
	{
		fprintf(stderr, "ERROR !! The server with %s name does not exist\n",argv[1]);
		exit(0);
	}



//clear socket buffer
	bzero((char *) &servent_addr, sizeof(servent_addr));
	servent_addr.sin_family = AF_INET;
	
//copying values
	bcopy((char *)server->h_addr, (char *)&servent_addr.sin_addr.s_addr, server->h_length);	
	servent_addr.sin_port = htons(port_num);

	if (connect(c_soc_fd,(struct sockaddr *) &servent_addr,sizeof(servent_addr)) < 0)
	{
		perror("ERROR!! Could not connect");
		exit(0);
	}
	

	bzero(msg_buf,256);	
	printf("Please enter a number: ");
	
	fgets(msg_buf,255,stdin);	
	msg = write(c_soc_fd,msg_buf,strlen(msg_buf));
	if (msg < 0)
	{
		perror("ERROR !! Could not write to client socket");
		exit(0);
	}
	bzero(msg_buf,256);
	msg = read(c_soc_fd,msg_buf,256);
	if (msg < 0)
	{
		perror("ERROR !! Could not read from socket");
		exit(0);
	}
//Send two numbers independently after recieving ack from server that first number is recieved	
	result=atoi(msg_buf);
	if(result==0)
	{

		printf("Please enter the second number: ");
	
		fgets(msg_buf,255,stdin);	
		msg = write(c_soc_fd,msg_buf,strlen(msg_buf));
		if (msg < 0)
		{
			perror("ERROR !! Could not write to client socket");
			exit(0);
		}
	}
	
	bzero(msg_buf,256);
	msg = read(c_soc_fd,msg_buf,256);
	if (msg < 0)
	{
		perror("ERROR !! Could not read from socket");
		exit(0);
	}
//Product recieved through buffer 	
	result=atoi(msg_buf);
	printf("Result recieved from Server : %d\n",result);
	return 0;
	}
