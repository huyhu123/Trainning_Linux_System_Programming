#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main()
{
    int server_socket, client_socket, recv_len;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];

    // create a UDP socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // bind the socket to a specific IP address and port
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("UDP echo server is listening on port %d...\n", PORT);

    while (1)
    {
        // receive data from a client
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t client_address_len = sizeof(client_address);
        if ((recv_len = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_address_len)) == -1)
        {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        printf("Received %d bytes from %s:%d\n", recv_len, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // send the data back to the client
        if (sendto(server_socket, buffer, recv_len, 0, (struct sockaddr *)&client_address, client_address_len) == -1)
        {
            perror("sendto");
            exit(EXIT_FAILURE);
        }

        printf("Sent %d bytes back to %s:%d\n", recv_len, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    }

    close(server_socket);
    return 0;
}
