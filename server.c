#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PORT "8080"

/**
 * get sockaddr, IPv4 or Ipv6;
 * return : struct in_addr*
 * return : struct in6_addr*
 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)sa;
        return &ipv4->sin_addr;
    }

    struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)sa;
    return &ipv6->sin6_addr;
}

/**
 * get current time
 * return string
 */
char *get_current_time(char *buf, size_t size)
{
    time_t unix_current_time = time(NULL);
    struct tm *tm = localtime(&unix_current_time);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm);
    return buf;
}

/**
 * Main function
 */
int main(void)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int receive;
    int yes = 1;
    int err = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;

    receive = getaddrinfo(NULL, PORT, &hints, &servinfo);
    if (receive != 0)
    {
        fprintf(stderr, "gai error: %s\n", gai_strerror(receive));
        exit(1);
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (sockfd == -1)
        {
            perror("server: socket");
            continue;
        }

        err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (err == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        err = bind(sockfd, p->ai_addr, p->ai_addrlen);
        if (err == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    return 0;
}