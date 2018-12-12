/* Author: Prajna Prabhakara 
Server program that creates a socket to recieve TCP Connections
and sends the product of two numbers received from clients*/
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

//Method to accept data from clients and send response
void serve_client(int s_soc_fd_tcp, int client_num){

	struct sockaddr_in client_addr;
	int client_soc_fd, client_length, msg;
	char send_buf[256], recvbuf[256];
	int num_input1, num_input2, result;
	
	client_length = sizeof(client_addr);
//accept from client socket 
	client_soc_fd = accept(s_soc_fd_tcp,(struct sockaddr *) &client_addr, &client_length);

	if (client_soc_fd < 0)
	{
		perror("ERROR!! Could not accept");
		exit(1);
	}

	bzero(recvbuf,256);
	bzero(send_buf,256);
	msg = read(client_soc_fd,recvbuf,256);
	num_input1 = atoi(recvbuf);
	printf("First number received from client number %d : %d\n",client_num,num_input1);
	sprintf(send_buf,"%d",0);
	msg = write(client_soc_fd,send_buf,256);


	bzero(recvbuf,256);
	msg = read(client_soc_fd,recvbuf,256);
	num_input2 = atoi(recvbuf);
	printf("Second number received from client number %d : %d\n",client_num,num_input2);

	result = num_input1 * num_input2;
	
	if (msg < 0)
	{
		perror("ERROR reading from socket");
		exit(1);
	}
	bzero(send_buf,256);
	printf("Result : %d\n",result);
	
	sprintf(send_buf,"%d", result);
	msg = write(client_soc_fd,send_buf,256);

	if (msg < 0)
	{
		perror("ERROR writing to socket");
		exit(1);
	}
}

int main(int argc, char *argv[])
{

	int s_soc_fd_tcp, port_num, pid, news_soc_fd_tcp, client_num;
//Character buffer that stores the values sent and recieved at the server
	struct sockaddr_in servent_addr;
	pid_t child_pid;

//Checking number of arguments entered in command line
	if(argc < 2)
	{
		fprintf(stderr, "Please enter the server port number \n");
		exit(1);	
	}

	
//Socket system call : Creating an endpoint for communication at Server
//Connection oriented : 
	s_soc_fd_tcp = socket(AF_INET, SOCK_STREAM,0);
	

	if(s_soc_fd_tcp <0)
	{
		perror("ERROR!! Socket is not opened ");
		exit(1);
	}
	
	bzero((char *) &servent_addr, sizeof(servent_addr));

	port_num =atoi(argv[1]);

//Looking up and ensuring that user hasnt chosen one of the well known port numbers
	if(port_num>=0 && port_num<=1024)
	{
		fprintf(stderr, "Port number cannot be 0 to 1024, Please choose a port number greater than 1025 to 64K  \n");
		exit(1);		
	}

//Defining server socket parameters
	servent_addr.sin_family = AF_INET;
	servent_addr.sin_addr.s_addr = INADDR_ANY;
	servent_addr.sin_port = htons(port_num);
//Bind TCP socket 
	if (bind(s_soc_fd_tcp, (struct sockaddr *) &servent_addr,sizeof(servent_addr)) < 0)
	{
		perror("ERROR !!! Could not bind TCP");
		exit(1);
	} 


//allowing 2 pending TCP connections
	listen(s_soc_fd_tcp,2);
	
	pid = fork();
	printf("Server Listening .... ");
	if(pid==0)
	{
//child process
		client_num=2;
		//close(s_soc_fd_tcp);
		//news_soc_fd_tcp = socket(AF_INET, SOCK_STREAM,0);
		//bind(news_soc_fd_tcp, (struct sockaddr *) &servent_addr,sizeof(servent_addr));
		//listen(news_soc_fd_tcp,1);
		serve_client(s_soc_fd_tcp, client_num);
		printf("Served Child client with PID %d \n",pid);
		close(s_soc_fd_tcp);
		exit(0);
	}	
	if(pid>0){
//parent process
		client_num=1;
		//close(news_soc_fd_tcp);
		serve_client(s_soc_fd_tcp, client_num);
		printf("Served Parent client with PID %d \n ",pid);	
//wait until server serves child process
		child_pid = wait(NULL);
		close(s_soc_fd_tcp);
	}
	return 0;	
}
