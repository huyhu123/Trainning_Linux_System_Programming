#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void *handle_client(void *arg);

int main(int argc, char *argv[])
{
    int server_fd, client_fd, opt = 1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t threads[MAX_CLIENTS];

    // Create a TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse address and port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a specific IP address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5000);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 5000...\n");

    // Accept incoming connections and create a new thread to handle each client
    while (1)
    {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void *)&client_fd) != 0)
        {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }

        // Detach the thread to allow it to run independently
        pthread_detach(thread);
    }

    return 0;
}
void *handle_client(void *arg)
{
    int client_fd = *(int *)arg;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Receive data from the client and echo it back
    while ((bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        if (send(client_fd, buffer, bytes_read, 0) != bytes_read)
        {
            perror("send failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close the client socket
    close(client_fd);

    printf("Client disconnected\n");

    return NULL;
}