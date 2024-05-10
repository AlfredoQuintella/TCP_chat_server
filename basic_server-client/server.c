#include <stdio.h> 
#include <stdlib.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>

#define MAX 1024
#define SA struct sockaddr

int PORT = 8080;

// chating
void chat(int connfd) {
    char buffer[MAX];
    int n;

    for(;;) {
        bzero(buffer, MAX);

        read(connfd, buffer, sizeof(buffer));

        if(strncmp("exit", buffer, 4) == 0) {
            printf("Messages will be deleted in 2 seconds...\n");
            sleep(2);
            printf("\033[H\033[J"); // Limpa a tela do terminal
            break;
        }

        printf("From client: %s\t To client: ", buffer);
        bzero(buffer, MAX);
        n = 0;
        while((buffer[n++] = getchar()) != '\n');
        write(connfd, buffer, sizeof(buffer));

        if(strncmp("exit", buffer, 4) == 0) {
            printf("Messages will be deleted in 2 seconds...\n");
            sleep(2);
            printf("\033[H\033[J"); // Limpa a tela do terminal
            break;
        }
    }
}

int main() {
    int sockfd, connfd, len;
    struct sockaddr_in serveraddr, clientaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        printf("Socket cration failed...\n");
        exit(0);
    } else {
        printf("Socket successfully created...\n");
    }

    bzero(&serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);

    if((bind(sockfd, (SA*)&serveraddr, sizeof(serveraddr))) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    } else {
        printf("Socket successfully binded...\n");
    }

    if((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else {
        printf("Server listening...\n");
    }

    len = sizeof(clientaddr);

    connfd = accept(sockfd, (SA*)&clientaddr, &len);
    if(connfd < 0) {
        printf("Server accept failed...\n");
        exit(0);
    } else {
        printf("Server accept the client...\n");
    }

    chat(connfd);

    close(sockfd);
}