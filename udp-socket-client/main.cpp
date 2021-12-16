#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 6789
#define MAXLINE 1024

int main(int argc, const char *argv[])
{
    int sockfd;
    char buffer[MAXLINE];
    const char *hello = "Hello from client!";
    struct sockaddr_in serverAddress;

    printf("Starting UDP client...\n");

    // Use message from argument, or test message
    const char *stringToSend = argc > 1 ? argv[1] : "Test Message";

    // Creates ipv4(AF_INET) datagram(SOCK_DGRAM) connection, which implies UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;         // Set ipv4
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Set any input interface
    serverAddress.sin_port = htons(PORT);       // Set port

    unsigned int n, len;

    // Send message to server, flag 0 as is a normal message
    sendto(sockfd, (const char *)stringToSend, strlen(stringToSend), 0, (const struct sockaddr *)&serverAddress, sizeof(serverAddress));

    printf("Message sent: %s\n", stringToSend);

    // Receive response from server
    // Block until all data is received
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&serverAddress, &len);
    buffer[n] = '\0';

    printf("Server response: %s\n", buffer);

    close(sockfd);

    return 0;
}
