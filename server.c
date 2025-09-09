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
        int sockfd, newsockfd, portno, clilen;
        char buffer[256];
        struct sockaddr_in serv_addr, cli_addr;
        int n;
        portno = 5001;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
                perror("ERROR opening socket");
                exit(1);
        }
        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serv_addr.sin_port = htons(portno);
        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
                perror("ERROR on building");
                exit(1);
        }
        printf("Server starts listening: \n");
        while (1)
        {
                listen(sockfd, 5);
                clilen = sizeof(cli_addr);
                newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
                if (newsockfd < 0)
                {
                        perror("ERROR on accept");
                        exit(1);
                }
                bzero(buffer, 256);
                n = read(newsockfd, buffer, 255);
                if (n < 0)
                {
                        perror("ERROR reading from socket");
                        exit(1);
                }

                printf("client: %s\n", buffer);
                n = write(newsockfd, "server received msg", 40);
                if (n < 0)
                {
                        perror("Error writing to socket");
                        exit(1);
                }
        }
        return 0;
}
