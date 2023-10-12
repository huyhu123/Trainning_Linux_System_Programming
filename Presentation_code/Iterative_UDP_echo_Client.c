#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 5000
#define BUFFER_SIZE 1024

int main()
{
    int client_socket, recv_len;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // create a UDP socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // set the server address and port
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);

    while (1)
    {
        // get input from the user
        printf("Enter a message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // remove the newline character

        // send the message to the server
        if (sendto(client_socket, buffer, str_len(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
        {
            perror("sendto");
            exit(EXIT_FAILURE);
        }

        printf("Sent %ld bytes to %s:%d\n", str_len(buffer), SERVER_IP, PORT);

        // receive the response from the server
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t server_address_len = sizeof(server_address);
        if ((recv_len = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_address, &server_address_len)) == -1)
        {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        printf("Received %d bytes from %s:%d\n", recv_len, SERVER_IP, PORT);
        printf("Response: %s\n", buffer);
    }

    close(client_socket);
    return 0;
}