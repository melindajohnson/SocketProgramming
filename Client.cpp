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
#include <algorithm>
#include <chrono>
using namespace std;

const int BUFFSIZE=1500;

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
    int iteration;
    int type = atoi(argv[6]); //the type of transfer scenario: 1, 2, or 3 ;
    // iteration value validation
    if ( atoi(argv[3]) <= 0)
    {
        printf( "Iteration usage: greater than 0 \n" );
        return -1;
    }
    else{
        iteration = atoi(argv[3]); //the repetition of sending a set of data buffers
    }

    int nbufs = atoi(argv[4]); //the number of data buffers
    int bufsize = atoi(argv[5]); //the size of each data buffer (in bytes)
    char databuf[nbufs][bufsize]; // create data buffer
    int clientSD = -1;

    //Use getaddrinfo() to get addrinfo structure corresponding to serverName / Port
    //This addrinfo structure has internet address which can be used to create a socket too


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

    //Iterate through addresses and connect
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

    //	Send a message to the server letting it know the number of iterations of the test it will per
    int msgSent = write(clientSD,&iteration, sizeof(iteration));
    if (msgSent == -1) {
        cout << "There was a problem sending the number of reps" << endl;
        return -1;
    }

    //Perform the appropriate number of tests with the server and measure the time this takes
    // initialize chrono variables and start timers before the tests
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    for(int i=0; i < iteration; i++) {
        switch (type) {
            case 1: {
             /* Multiple writes: invokes the write() system call for each data buffer, thus
             resulting in calling as many write()s as the number of data buffers, (i.e., nbufs) */
                for (int j = 0; j < nbufs; j++) {
                    write(clientSD, &databuf[j], bufsize);
                }
                break;
            }
            case 2:
            {
            /* writev: allocates an array of iovec data structures, each having its *iov_base field
            point to a different data buffer as well as storing the buffer size in its iov_len field;
            and thereafter calls writev() to send all data buffers at once. */
                struct iovec vector[nbufs];
                for (int j = 0; j < nbufs; j++) {
                    vector[j].iov_base = &databuf[j];
                    vector[j].iov_len = bufsize;
                }
                writev(clientSD, vector, nbufs);
                break;
            }
            case 3: {
             /* single write: allocates an nbufs-sized array of data buffers, and thereafter calls write()
             to send this array, (i.e., all data buffers) at once.*/
                write(clientSD, databuf, nbufs * bufsize);
                break;
            }
        }
    }
    // end chrono time
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    //Receive from the server a message with the number read() system calls it performed
    int number_of_reads = 0;
    read(clientSD,&number_of_reads, sizeof(number_of_reads));


    //Print information about the test, using chrono library
    cout << "Test type: " << type << "   nbufs: " << nbufs << "   bufsize: " << bufsize << endl;
    cout << "Time: " << elapsed_seconds.count() * 1000000 << " usec"
    << "   Number of reads: " <<  number_of_reads
    << "   Throughput: " << ( nbufs * bufsize * iteration * 8 * 0.000000001) / elapsed_seconds.count()  << "Gbps" << endl;

    close(clientSD);
    return 0;
}



