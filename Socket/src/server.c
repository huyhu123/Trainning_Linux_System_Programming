#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_CLIENTS 5
#define BUFFER      1024

void reverse_string(char *str)
{
    int i = 0;
    int j = 0;
    char temp = 0;

    for (i = 0, j = strlen(str) - 1; i < j; i++, j--)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

void *handle_client(void *arg)
{
    int connfd = *(int *)arg;
    char recv_buff[BUFFER] = {0};
    int read_val = 0;

    while (1)
    {
        // Clear receive buffer
        memset(recv_buff, '0', sizeof(recv_buff));

        // Read from client
        read_val = read(connfd, recv_buff, sizeof(recv_buff) - 1);

        if (read_val > 0)
        {
            recv_buff[read_val] = 0;
            printf("Received message from client: %s\n", recv_buff);

            // Reverse recieved message
            reverse_string(recv_buff);

            // Send reverse message to client
            write(connfd, recv_buff, strlen(recv_buff));
            printf("Send message to client: %s\n", recv_buff);
        }
        else if (read_val == 0) // Client close connection
        {
            printf("Client closed connection\n");
            break;
        }
        else // Read error
        {
            printf("Read error\n");
            break;
        }
    }

    // Close client fd
    close(connfd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int listenfd = 0; 
    int connfd = 0;
    pthread_t tid = 0;
    struct sockaddr_in serv_addr;

    // Creating socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // Clear adress struct
    memset(&serv_addr, '0', sizeof(serv_addr));

    // Initialize address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    // Binding Socket to address
    bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Listening for incoming connection
    listen(listenfd, MAX_CLIENTS);

    while (1)
    {
        // Accepting a Connection
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);

        // Create a new thread to handle the client
        if (pthread_create(&tid, NULL, handle_client, (void *)&connfd) != 0)
        {
            printf("Error creating thread\n");
            close(connfd);
        }
    }

    return 0;
}

