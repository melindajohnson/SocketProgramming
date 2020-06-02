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
using namespace std;

const int BUFFSIZE=1500;

/**
 * Multiple write test
 * @param clientSd file descriptor of client socket
 * @param databuf data buffer
 * @param nbufs number of buffers
 * @param bufsize buffer size
 */
void Test1(int clientSd,char **databuf,int nbufs, int bufsize)
{
    for (int j = 0; j < nbufs; j++)
    {
        write(clientSd, databuf[j], bufsize);
    }
}

/**
 * Writev test
 * @param clientSd file descriptor of client socket
 * @param databuf data buffer
 * @param nbufs number of buffers
 * @param bufsize buffer size
 */
void Test2(int clientSd,char **databuf,int nbufs, int bufsize)
{
    struct iovec vector[nbufs];
    for (int j = 0; j < nbufs; j++)
    {
        vector[j].iov_base = databuf[j];
        vector[j].iov_len = bufsize;
    }
    writev(clientSd, vector, nbufs);

}

/**
 * Single write test
 * @param clientSd file descriptor of client socket
 * @param databuf data buffer
 * @param nbufs number of buffers
 * @param bufsize buffer size
 */
void Test3(int clientSd,char **databuf,int nbufs, int bufsize)
{
    write(clientSd, databuf, nbufs * bufsize);
}


int main(int argc, char *argv[])
{

     // Argument validation
    if ( argc != 7 )
	{
		printf( "usage: client #serverName #port #repetition #nbufs #bufsize #type \n" );
		return -1;
	}

    char *serverName = argv[1];   //the name of the server
    char *serverPort = argv[2]; //the IP port number used by server (use the last 5 digits of your student id)
    int repetition = atoi(argv[3]); //the repetition of sending a set of data buffers
    int nbufs = atoi(argv[4]); //the number of data buffers
    int bufsize = atoi(argv[5]); //the size of each data buffer (in bytes)
    int type = atoi(argv[6]); //the type of transfer scenario: 1, 2, or 3
    char databuf[nbufs*bufsize];
    //char *databuf;
    int clientSD = -1;

    /*
     * Use getaddrinfo() to get addrinfo structure corresponding to serverName / Port
	 * This addrinfo structure has internet address which can be used to create a socket too
     */

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;					/* Allow IPv4 or IPv6*/
    hints.ai_socktype = SOCK_STREAM;					/* TCP */
    hints.ai_flags = 0;							/* Optional Options*/
    hints.ai_protocol = 0;						/* Allow any protocol*/
    int rc = getaddrinfo(serverName, serverPort, &hints, &result);
    if (rc != 0)
    {
        cerr << "ERROR: " << gai_strerror(rc) << endl;
        exit(EXIT_FAILURE);
    }

    /*
     * Iterate through addresses and connect
     */
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        clientSD = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (clientSD == -1)
        {
            continue;
        }
        /*
        * A socket has been successfully created
        */
        rc = connect(clientSD, rp->ai_addr, rp->ai_addrlen);
        if (rc < 0)
        {
            cerr << "Connection Failed" << endl;
            close(clientSD);
            return -1;
        }
        else	//success
        {
            break;
        }
    }

    if (rp == NULL)
    {
        cerr << "No valid address" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Client Socket: " << clientSD << "  connected"<< endl;
    }
    freeaddrinfo(result);

    //databuf = new char[BUFFSIZE]; was used in previous code
    // Write and read data over network


    //	Send a message to the server letting it know the number of iterations of the test it will perform
    for (int i = 0; i < BUFFSIZE; i++)
    {
        databuf[i] = 'z';
    }

    //Perform the appropriate number of tests with the server (measure the time this takes;
    // I would recommend the  chrono time library)
    int bytesWritten = write(clientSD, databuf, BUFFSIZE);
    cout << "Bytes Written: " << bytesWritten << endl;

    //Receive from the server a message with the number read() system calls it performed
    int bytesRead = read(clientSD, databuf, BUFFSIZE);
    cout << "Bytes Read: " << bytesRead << endl;
    cout << databuf[13] << endl;


    //Print information about the test (use chrono library to time the
    //Test (1,2, or 3): time = xx usec, #reads = yy, throughput zz Gbps
    close(clientSD);
    return 0;
}



