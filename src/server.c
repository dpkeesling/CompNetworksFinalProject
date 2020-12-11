// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <stdbool.h>
#define PORT 8080 

int create_socket_connection();

int main(int argc, char const *argv[]) 
{ 
    char buffer[1024] = {0}; 
    char *ack_message = "Acknowledged";
    int sock = create_socket_connection();
    int valread;
    while(true) {
        valread = read(sock , buffer, 1024);
        if(strcmp(buffer, "Exit chat\n") == 0) {
            ack_message = "Exit";
            send(sock , ack_message , 1024, 0);
            printf("Exit ack message sent\n"); 
            return 0;
        }
        else {        
            printf("%s\n",buffer );
            printf("Ack message sent\n"); 
            send(sock , ack_message , 1024, 0);
        }
    }
} 

int create_socket_connection() {

    int server_fd, new_socket;
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    return new_socket; 
}