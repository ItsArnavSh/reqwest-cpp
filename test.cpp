#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    const char *hostname = "www.example.com";
    const char *port = "80";
    struct addrinfo hints, *res;
    int sockfd;

    // Set up hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;       // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // Use TCP

    // Resolve hostname to IP
    if (getaddrinfo(hostname, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    // Create socket
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    // Connect to server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }
    printf("Connected to %s\n", hostname);

    // Send GET request
    char request[] = "GET / HTTP/1.1\r\n"
                     "Host: www.example.com\r\n"
                     "Connection: close\r\n"
                     "\r\n";

    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("send");
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }
    printf("GET request sent!\n");

    // Receive response
    char buffer[1024];
    int bytes_received;

    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the buffer
        printf("%s", buffer);
    }

    if (bytes_received == -1) {
        perror("recv");
    }

    // Clean up
    close(sockfd);
    freeaddrinfo(res);
    printf("\nConnection closed.\n");

    return 0;
}
