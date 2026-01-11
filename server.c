#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT "8080"
#define BACKLOG 10

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
    strftime(buf, size, "%Y-%m-%d %H:%M:%S\n", tm);
    return buf;
}

/**
 * Main function
 */
int main(void)
{
    int sockfd, accepted_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage client_addr;
    socklen_t sin_size;
    int receive;
    int yes = 1;
    char client_ip[INET6_ADDRSTRLEN];
    char current_time[26];
    char *time_str;
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

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    err = listen(sockfd, BACKLOG);
    if (err == -1)
    {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    sin_size = sizeof client_addr;
    accepted_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
    if (accepted_fd == -1)
    {
        perror("accept");
        exit(1);
    }

    inet_ntop(
        client_addr.ss_family,
        get_in_addr((struct sockaddr *)&client_addr),
        client_ip,
        sizeof client_ip);
    printf("server: got connection from %s\n", client_ip);

    printf("server: sent response to %s\n", client_ip);
    time_str = get_current_time(current_time, sizeof current_time);
    send(accepted_fd, time_str, strlen(time_str), 0);

    printf("server: connection closed %s\n", client_ip);
    close(accepted_fd);
    close(sockfd);

    printf("server: shutdown\n");
    return 0;
}