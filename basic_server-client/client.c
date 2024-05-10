#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <strings.h> 
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void chat(int sockfd)
{
    char buffer[MAX];
    int n;
    for (;;) {
        bzero(buffer, sizeof(buffer));

        printf("\tTo server : ");
        n = 0;

        while ((buffer[n++] = getchar()) != '\n')
            ;

        write(sockfd, buffer, sizeof(buffer));

        if(strncmp("exit", buffer, 4) == 0) {
            printf("Messages will be deleted in 2 seconds...\n");
            sleep(2);
            printf("\033[H\033[J"); // Limpa a tela do terminal
            break;
        }

        bzero(buffer, sizeof(buffer));

        read(sockfd, buffer, sizeof(buffer));

        printf("From Server : %s", buffer);

        if(strncmp("exit", buffer, 4) == 0) {
            printf("Messages will be deleted in 2 seconds...\n");
            sleep(2);
            printf("\033[H\033[J"); // Limpa a tela do terminal
            break;
        }
    }
}
 
int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
 
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
 
    // function for chat
    chat(sockfd);
 
    // close the socket
    close(sockfd);
}
