# SocketConnection

C Program illustrating system calls to create socket endpoints in a network system using socket, connect and fork. 

server.c : Waits for a connection from 2 clients requesting for a service (Here in this case it just serves as a multiplier)
client.c : sends two numbers to server using socket communication.

Usage in command line : compile :  gcc -o server.out server.c
./server.out <portnumber>
eg : ./server.out 3333
At Client Side: gcc -o client1.out client1.c
./client1.out <IPaddress> <portnumber of client>
eg: ./client1.out localhost 3333

server.c uses socket(), bind() , listen() and accept() system calls to listen to TCP connections and returns the product of two numbers received through character buffer from client. It also handles multiple clients ( Here, 2 clients) by forking the process using fork() system call to serve two simultaneous requests.

client1.c  sends the two numbers one after another using the character buffer and uses connect system call to establish TCP connection with server.

A Simple Java Application to demonstrate intercommunication of client and server systems in a network with Swing UI explaining what happens under the hood.
A Java application to demonstrate connection and communication between multiple servers and client can be run using the zip file in the repository.

