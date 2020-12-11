// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#define PORT 8080 

int connect_to_server();

int main(int argc, char const *argv[]) 
{ 
    int valread;  
    char message[1024] = {0};
    char buffer[1024] = {0};
    int sock = connect_to_server();
    while(true) {
        printf("Enter your message. It cannot be longer that 1024 characters\n");
        fgets(message, 1024, stdin);
        send(sock , message , 1024, 0); 
        printf("Message sent\n"); 
        valread = read( sock , buffer, 1024);
        printf("%s\n", buffer);
        if(strcmp(buffer, "Exit") == 0) {
            return 0;
        }
    }
    return 0; 
} 

int connect_to_server() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        perror("Socket creation error"); 
        exit(EXIT_FAILURE); 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        perror("Invalid address/ Address not supported"); 
        exit(EXIT_FAILURE); 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        perror("Connection Failed "); 
        exit(EXIT_FAILURE); 
    }
    return sock;
}
