/*@File: Client.cpp
* @Name: Melinda Stannah Stanley Jothiraj
* @Number: 1978413
* @Date: 27May2020
* @Program Name:  Socket Programming
*
* Program Purpose:
*    This assignment is intended for three purposes:
*    (1) to utilize various socket-related system calls,
*    (2) to create a multi-threaded server and
*    (3) to evaluate the throughput of different mechanisms when using TCP/IP do to do point-to-point communication over a network.
* Client-Server Model:
* In this program we will use the client-server model where a client process establishes a connection to a server,
* sends data or requests, and closes the connection.  The server will accept the connection and create a thread to
* service the request and then wait for another connection on the main thread.  Servicing the request consists of
* (1) reading the number of iterations the client will perform,
* (2) reading the data sent by the client, and
* (3) sending the number of reads which the server performed.
*/


// * Your client program will take the following six arguments:
//1.	serverName:  the name of the server
//2.	port: the IP port number used by server (use the last 5 digits of your student id)
//3.	repetition: the repetition of sending a set of data buffers
//4.	nbufs: the number of data buffers
//5.	bufsize: the size of each data buffer (in bytes)
//6.	type: the type of transfer scenario: 1, 2, or 3
//For example,
//// validate the arguments
//	if ( argc != 7 )
//	{
//		printf( "usage: client #serverName #port #repetition #nbufs #bufsize #type \n" );
//		return -1;
//	}
