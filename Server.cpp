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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
using namespace std;

const int BUFFSIZE = 1500;
const int NUM_CONNECTIONS = 5;

struct clientDataArgs{
    int clientSocket;
    char* databuf;
    //int iterations;
};

void *thread_runner (void *args)
{
    int number_of_reads = 0;
    int total_bytes_read = 0;
    //Receive a message by the client with the number of iterations to perform
    clientDataArgs clientData = *(clientDataArgs*) args;
    int bytesRead = read(clientData.clientSocket, clientData.databuf, BUFFSIZE);
    //Receive a message by the client with the number of iterations to perform
    int iteration = clientData.databuf[0];
    cout << "iteration: " << iteration << endl;
    bzero(clientData.databuf, BUFFSIZE); //zeroed out buffer

    //Read from the client the appropriate number of iterations of BUFSIZE amounts of data
    //Note: the read system call may return without reading the entire data buffer.
    //You must repeat calling read until you have read a BUFSIZE amount of data.
    //This should be done for each repetition on the server.
 //iteration 20000*1500 loop
    while(total_bytes_read != BUFFSIZE){
        bytesRead = read(clientData.clientSocket, clientData.databuf, BUFFSIZE);
        total_bytes_read += bytesRead;
        if(bytesRead > 0){
            number_of_reads++ ;
        }
    }

    //	Send the number of read() calls made as an acknowledgment to the client
    clientData.databuf[0] = number_of_reads;
    int bytesWritten = write(clientData.clientSocket, clientData.databuf, BUFFSIZE);
  //  cout << "Bytes Written: " << bytesWritten << endl;

    close(clientData.clientSocket);

}

int main(int argc, char *argv[])
{
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

    //validate the arguments
    if ( argc != 2 )
    {
        printf( "usage: server #port\n" );
        return -1;
    }

    int serverPort =  atoi(argv[1]);
    //char *serverName;

    pthread_t thread[NUM_CONNECTIONS];
    int thread_num = 0;

    //Build address
    //int port = 12345;
    sockaddr_in acceptSocketAddress;
    bzero((char *)&acceptSocketAddress, sizeof(acceptSocketAddress));
    acceptSocketAddress.sin_family = AF_INET; //use the internet
    acceptSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY); //server listen for any ip address thats trying to connect
    acceptSocketAddress.sin_port = htons(serverPort); //port address


    //Create socket
    int serverSD = socket(AF_INET, SOCK_STREAM, 0); //sock_stream is use tcp_ip
    const int on = 1;
    setsockopt(serverSD, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(int));
    cout << "Socket #: " << serverSD << endl;

    //Bind
    int rc = bind(serverSD, (sockaddr *)&acceptSocketAddress, sizeof(acceptSocketAddress));
    if (rc < 0)
    {
        cerr << "Bind Failed" << endl;
    }

//    The main thread of a server will run in a dispatch loop which:
//    •	Accepts a new connection.
//    •

    //listen and accept
    listen(serverSD, NUM_CONNECTIONS);       //setting number of pending connections

    while(1)
    {
        sockaddr_in newSockAddr;
        socklen_t newSockAddrSize = sizeof(newSockAddr);
        int newSD = accept(serverSD, (sockaddr *) &newSockAddr, &newSockAddrSize); //socket of client is returned
        if (newSD == -1) {
            std::cerr << "Error while Accepting on socket\n";
            continue;
        }

       // cout << "Accepted Socket #: " << newSD <<endl;

        //Creates a struct with client information
        struct clientDataArgs *args = new clientDataArgs;
        args->clientSocket = newSD;
        char databuf[BUFFSIZE];
        bzero(databuf, BUFFSIZE); //zeroed out buffer
        args->databuf = databuf;

        //Creates a thread to service that request
        pthread_create(&thread[thread_num], NULL, thread_runner, (void*) args);
        thread_num++;
        //terminate thread
        pthread_detach(pthread_self());

    }

    return 0;
}

