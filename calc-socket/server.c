#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>  // For isdigit()

float calc(float a, char o, float b);
void parse_expression(char *buffer, float *a, char *op, float *b);

int main() {
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    portno = 5001;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Set up the server address structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(portno);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // Listen for incoming connections
    printf("Server starts listening: \n");
    while (1) {
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        // Clear the buffer and read the message from the client
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        // Parse the expression from the buffer
        float a, b;
        char op;
        parse_expression(buffer, &a, &op, &b);

        // Calculate the result
        float result = calc(a, op, b);
        printf("Received expression: %f %c %f = %f\n", a, op, b, result);

        // Send the result back to the client
        snprintf(buffer, sizeof(buffer), "Result: %f", result);
        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("Error writing to socket");
            exit(1);
        }
    }

    return 0;
}

// Function to parse the expression in the format "a op b" from the string
void parse_expression(char *buffer, float *a, char *op, float *b) {
    char *token;
    token = strtok(buffer, "+-*/");  // Extract the first number
    if (token != NULL) {
        *a = atof(token);
    }

    *op = buffer[strlen(token)];  // The operator will be the next character

    token = strtok(NULL, "+-*/");  // Extract the second number
    if (token != NULL) {
        *b = atof(token);
    }
}

// Function to perform calculation based on the operator
float calc(float a, char o, float b) {
    switch (o) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b != 0) {
                return a / b;
            } else {
                printf("Error: Division by zero!\n");
                return -1;  // Return an error value
            }
        default:
            printf("Invalid operator!\n");
            return -1;  // Return an error value for invalid operator
    }
}


