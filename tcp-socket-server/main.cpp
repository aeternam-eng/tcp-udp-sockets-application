#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#define BUFFER_SIZE 1024
#define PORT 5678

int main(int argc, char const *argv[])
{
    int serverFd, clientConnectionFd;
    struct sockaddr_in address;
    struct sockaddr_in clientAddress;
    char buffer[BUFFER_SIZE] = {0};
    int opt = 1;
    int addressLength = sizeof(address);

    printf("Starting server...\n");

    // Create ipv4(AF_INET) connection based stream connection, which implies TCP
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        printf("Socket failed\n");
        exit(EXIT_FAILURE);
    }

    // Set port and address as reusable on file descriptor
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        printf("Error setting socket options\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;         // Set ipv4
    address.sin_addr.s_addr = INADDR_ANY; // Set any input interface
    address.sin_port = htons(PORT);       // Set port

    // Bind configured address to file descriptor
    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Failed to bind socket\n");
        exit(EXIT_FAILURE);
    }

    // Listen on socket with 3 maximum connections
    if (listen(serverFd, 3) < 0)
    {
        printf("Failed to listen\n");
        exit(EXIT_FAILURE);
    }

    printf("Server started, listening...\n");

    while (1)
    {
        // Accept connection from client, blocking until connection is established
        if ((clientConnectionFd = accept(serverFd, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLength)) < 0)
        {
            printf("Failed to accept client\n");
            exit(EXIT_FAILURE);
        }

        read(clientConnectionFd, buffer, BUFFER_SIZE);

        // Getting client IP
        struct sockaddr_in *pv4Addr = (struct sockaddr_in *)&clientAddress;
        struct in_addr ipAddress = pv4Addr->sin_addr;

        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddress, clientIp, INET_ADDRSTRLEN);

        printf("Received from client %s: %s\n", clientIp, buffer);

        if (strcmp("stop", buffer) == 0)
        {
            printf("Closing connection from client request...\n");

            send(clientConnectionFd, "Stopping server...", strlen("Stopping server..."), 0);

            close(clientConnectionFd);
        }

        send(clientConnectionFd, buffer, strlen(buffer), 0);

        printf("Echo sent: %s\n", buffer);

        memset(&buffer, 0, BUFFER_SIZE);
    }

    return 0;
}
