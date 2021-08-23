//
//  tcp_echo_client.c
//  TCP client server program
//
//  Created by Pratham Goyal on 2021-08-23.
//
//CLIENT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFLEN 512

int main (int argc, char* argv[ ])        // Three arguments to be checked later
{
    struct sockaddr_in servAddr;        // Server socket address data structure
    char *servIP = argv[1];                // Server IP address from command line
    int servPort = atoi(argv[2]);        // Server port number from command line
    char* message;
    message = malloc(BUFLEN);           // Allocated space, avoiding segmentation error
    strcpy(message, argv[3]);            // Copying argv[3] to message
    char buffer[BUFLEN];                // Buffer array to store argu[3]

    // Check for correct number of command line arguments
    if(argc != 4) {
        printf("tcp-echo-client [IP address] [Port] [Message]\n");
        exit (1);
    }
    
    // Populate socket address for the server
    memset (&servAddr, 0, sizeof(servAddr));        // Initialize data structure
    servAddr.sin_family = AF_INET;                     // This is an IPv4 address
    servAddr.sin_addr.s_addr = inet_addr(servIP);     // Server IP address
    servAddr.sin_port = servPort;                    // Server port number
    
    // Create a TCP socket stream
    int sock;
    if ((sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Error: socket creation failed!\n");
        exit (1);
    }
    else
        printf("Socket successfully created.\n");

    // Connect  to the server
    if ((connect (sock, (struct sockaddr*)&servAddr, sizeof(servAddr))) == -1) {
        perror("Connect Failed: ");
        exit (-1);
    }
     else
        printf("Connected to the server.\n");
    
    // Send data to the server
    send(sock, message, strlen(message), 0);
    
    // Receive and format the data.
    size_t byte_len = 0;
    size_t byte_limiter = BUFLEN;
    
    //waits to recv back from server and also end the connection.
    while ((byte_len = recv(sock, buffer, byte_limiter, 0)) > 0) {
        byte_limiter -= byte_len;
        buffer[byte_len] = '\0';
        printf("server sent back: %s\n", buffer);
        break;
    }
    
    // Close socket
    close (sock);
    printf("connection closed");
    // Stop program
    exit (0);
    
} // End main
