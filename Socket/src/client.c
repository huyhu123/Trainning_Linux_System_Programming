#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define BUFFER 1024

int main(int argc, char *argv[])
{
    int sockfd = 0; 
    int n = 0;
    char sendBuff[BUFFER] = {0};
    char recv_buff[BUFFER] = {0};
    char server_ip[BUFFER] = {0};
    struct sockaddr_in serv_addr;
    int read_val = 0;

    // Handle environment argument
    if (argc == 1)
    {
        strcpy(server_ip, "127.0.0.1");
    }
    else if (argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }
    else
    {
        strcpy(server_ip, argv[1]);
    }

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    // Clear adress struct
    memset(&serv_addr, '0', sizeof(serv_addr));

    // Initialize address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    // Converts an Internet address in its standard text format into its numeric binary form
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    // Connecting server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    while (1)
    {
        // Get input message
        printf("Enter a message: ");
        fgets(sendBuff, sizeof(sendBuff), stdin);

        // Send message to server
        write(sockfd, sendBuff, strlen(sendBuff));

        // Clear reveive buffer
        memset(recv_buff, '0', sizeof(recv_buff));

        // Read message
        read_val = read(sockfd, recv_buff, sizeof(recv_buff) - 1);
        if (read_val > 0)
        {
            recv_buff[read_val] = 0;
            printf("Received message from server: %s\n", recv_buff);
        }
        else if (read_val == 0) // Server close connection
        {
            printf("Server closed connection\n");
            break;
        }
        else // Read error
        {
            printf("Read error\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}