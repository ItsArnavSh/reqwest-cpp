#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
int main(int argc,char** argv){
    if(argc!=2){
        std::cout<<"The Format is ./reqwest_link";
        return 1;
    }
    const char *hostName = argv[1];
    const char *port = "80";
    struct addrinfo hints, *res, *p;
    int status;
    //Lets empty out hint
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;//No Fixed ipv4 or 6
    hints.ai_socktype = SOCK_STREAM;//TCP
    if((status = getaddrinfo(hostName, port, &hints, &res))!=0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
                return 1;
    }
        printf("IP addresses for %s:\n", hostName);
    // Iterate through the results
        for (p = res; p != NULL; p = p->ai_next) {
            char ipstr[INET6_ADDRSTRLEN];
            void *addr;
            const char *ipver;

            // Get the address and determine the address family
            if (p->ai_family == AF_INET) { // IPv4
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
                addr = &(ipv4->sin_addr);
                ipver = "IPv4";
            } else { // IPv6
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                addr = &(ipv6->sin6_addr);
                ipver = "IPv6";
            }

            // Convert the IP to a string and print it
            inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
            printf("  %s: %s\n", ipver, ipstr);
            //Great now we can simply Send a GET Request to this domain name, and we should probably get what we want

        }

}
