#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define MAX_BUFFER_SIZE 1024

void test_speed(char *host, int port, char *message) {
    struct sockaddr_in server_addr;
    int sockfd, bytes_sent, bytes_received, total_bytes_received = 0;
    char response[MAX_BUFFER_SIZE];
    clock_t start_time, end_time;
    double elapsed_time;

    // Create a socket for the client
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        perror("Error converting IP address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Send the message
    bytes_sent = send(sockfd, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("Error sending message");
        exit(EXIT_FAILURE);
    }

    // Receive the response
    start_time = clock();
    while ((bytes_received = recv(sockfd, response, MAX_BUFFER_SIZE, 0)) > 0) {
        total_bytes_received += bytes_received;
    }
    end_time = clock();

    // Calculate the elapsed time and print the results
    elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Speed test: %.2f seconds, %d bytes received\n", elapsed_time, total_bytes_received);

    // Close the socket
    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <host> <port> <message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *host = argv[1];
    int port = atoi(argv[2]);
    char *message = argv[3];

    test_speed(host, port, message);

    return EXIT_SUCCESS;
}
