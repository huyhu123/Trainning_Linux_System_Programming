#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_sent;

    // Create a TCP socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(5000);

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Send data to the server and receive the echoed data
    while (1)
    {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if ((bytes_sent = send(client_fd, buffer, strlen(buffer), 0)) != strlen(buffer))
        {
            perror("send failed");
            exit(EXIT_FAILURE);
        }

        if ((bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0)) < 0)
        {
            perror("recv failed");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';
        printf("Echoed message: %s", buffer);
    }

    // Close the client socket
    close(client_fd);

    return 0;
}
