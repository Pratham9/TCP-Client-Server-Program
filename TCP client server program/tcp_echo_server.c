//
//  tcp_echo_server.c
//  TCP client server program
//
//  Created by Pratham Goyal on 2021-08-23.
//
//SERVER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define    BUFLEN    512                                            // Maximum length of buffer
#define PORT    8899                                        // Fixed server port number

int main (void)
{
    struct sockaddr_in server_address;                        // Data structure for server address
    struct sockaddr_in client_address;                        // Data structure for client address
    socklen_t sockaddr_len = sizeof (struct sockaddr_in);
    
    // Populate socket address for the server
    memset (&server_address, 0, sizeof (server_address));    // Initialize server address data structure
    server_address.sin_family = AF_INET;                     // Populate family field - IPV4 protocol
    server_address.sin_port = PORT;                            // Set port number
    server_address.sin_addr.s_addr = INADDR_ANY;            // Set IP address to IPv4 value for loacalhost
    
    char buffer[BUFLEN];
         
    // Create a TCP socket; returns -1 on failure
    int listen_sock;
    if ((listen_sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("server socket: ");
        exit (-1);
    }

    // Bind the socket to the server address; returns -1 on failure
    if ((bind(listen_sock, (struct sockaddr *)&server_address, sockaddr_len)) == -1) {
        perror("bind: ");
        exit (-1);
    }
    
    /* Helpful status message */
    printf("Server is listening on port: %d\n", PORT);
    printf("Waiting for client request...\n");
    printf("Press CTRL + C to exit\n\n");
    
    // Listen for connections...
    if ((listen(listen_sock, 10)) == -1) {
        perror("listen: ");
        exit (-1);
    };
    
    // Accept connections from client...
    while(1) {
        int client_sock;
        if((client_sock = accept(listen_sock, (struct sockaddr *)&client_address, &sockaddr_len)) == -1) {
                perror("Client socket:     ");
            exit(-1);
        }
        
        printf("Client connection accepted from Port No %d and IP %s\n", ntohs(client_address.sin_port), inet_ntoa(client_address.sin_addr));
        
        // Receive and format the data...
        int byte_len = 0;
        int byte_limiter = BUFLEN;
        while ((byte_len = recv(client_sock, buffer, byte_limiter, 0)) > 0) {
            byte_limiter -= byte_len;
            buffer[byte_len] = '\0';
            printf("received: %s\n", buffer);
            
            // sends back data received from client back.
            send(client_sock, buffer, strlen(buffer), 0);
        }
        // After client receives data connection is ended.
        printf("Client disconnected.");
        close(client_sock);
        break;
    }
    
    close (listen_sock);                                    // Close descriptor referencing server socket
    exit(0);
    
} // End main
