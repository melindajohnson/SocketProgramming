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

// *
// validate the arguments
//	if ( argc != 2 )
//	{
//		printf( "usage: server #port\n" );
//		return -1;
//	}

/*
 * The servicing thread will do the following:
1.	Allocate dataBuf[BUFSIZE], where BUFSIZE = 1500 to read in data being sent by client
2.	Receive a message by the client with the number of iterations to perform
3.	Read from the client the appropriate number of iterations of BUFSIZE amounts of data
Note: the read system call may return without reading the entire data buffer.
 You must repeat calling read until you have read a BUFSIZE amount of data. This should be done for each repetition on the server.
4.	Send the number of read() calls made as an acknowledgment to the client
5.	Close this connection.
6.	Terminate the thread

 */