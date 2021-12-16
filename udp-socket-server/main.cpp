#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define PORT 6789

int main(int argc, char const *argv[])
{
    int serverFd;
    char buffer[BUFFER_SIZE] = {0};
    const char *hello = "Hello from server!";
    struct sockaddr_in serverAddress, clientAddress;

    printf("Starting UDP server...\n");

    // Creates ipv4(AF_INET) datagram(SOCK_DGRAM) connection, which implies UDP
    if ((serverFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Socket failed\n");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));

    serverAddress.sin_family = AF_INET;         // Set ipv4
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Set any input interface
    serverAddress.sin_port = htons(PORT);       // Set port

    // Bind the created file descriptor to the address, which creates a listening server
    if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Failed to bind socket\n");
        exit(EXIT_FAILURE);
    }

    unsigned int len, n;
    len = sizeof(clientAddress);

    while (1)
    {
        // Receive message from UDP client
        // Block until all data has been received
        n = recvfrom(serverFd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&clientAddress, &len);
        buffer[n] = '\0';

        printf("Received from client: %s\n", buffer);

        // if client has requested server to stop
        if (strcmp("stop", buffer) == 0)
        {
            printf("Stopping server from client request! \n");

            sendto(serverFd, "Stopping server...", strlen("Stopping server..."), MSG_CONFIRM, (const struct sockaddr *)&clientAddress, len);

            close(serverFd);

            break;
        }

        // Send echo response to client
        // MSG_CONFIRM as is a direct response to a datagram from a peer
        sendto(serverFd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&clientAddress, len);

        printf("Echo sent: %s.\n", buffer);
    }

    return 0;
}
