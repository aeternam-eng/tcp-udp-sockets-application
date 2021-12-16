#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define BUFFER_SIZE 1024
#define PORT 5678

int main(int argc, char const *argv[])
{
    int clientFd = 0;
    struct sockaddr_in serverAddress;
    const char *hello = "Hello from client!\n";
    char buffer[BUFFER_SIZE] = {0};

    // Get message from args or default message
    const char *messageToSend = argc > 1 ? argv[1] : hello;

    // Create ipv4(AF_INET) connection based stream connection, which implies TCP
    if ((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nUnable to create socket\n");
        return -1;
    }

    serverAddress.sin_family = AF_INET;   // Set ipv4
    serverAddress.sin_port = htons(PORT); // Set port

    // Set server ipv4 address from string
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0)
    {
        printf("Invalid/Unsupported ip address\n");
        return -1;
    }

    // Connect to server
    if (connect(clientFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Connection failed\n");
        return -1;
    }

    send(clientFd, messageToSend, strlen(messageToSend), 0);

    printf("Message sent: %s\n", messageToSend);

    recv(clientFd, buffer, BUFFER_SIZE, 0);

    printf("Response received: %s\n", buffer);

    close(clientFd);

    return 0;
}
