#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
int main()
{
        int sockfd, portno, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[256];
        portno = 5001;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
                perror("ERROR opening socket");
                exit(1);
        }

        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
                perror("ERROR connecting");
                exit(1);
        }
        while (1)
        {
                printf("Please enter the expression :");
                bzero(buffer, 256);
                fgets(buffer, 256, stdin);
                n = write(sockfd, buffer, strlen(buffer));
                if (n < 0)
                {
                        perror("ERROR writing to socket");
                        exit(1);
                }
                bzero(buffer, 256);
                n = read(sockfd, buffer, 256);
        }
        if (n < 0)
        {
                perror("ERROR reading from socket");
                exit(1);
        }
        printf("%s\n", buffer);
        printf("\n");
        close(sockfd);
        return 0;
}
