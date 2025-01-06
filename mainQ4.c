/*
 * mainQ1.c
 * 
 * Copyright 2024 ensea <ensea@StudentLab>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>



#define BSIZE 1024
#define GET "gettftp"
#define PUT "puttftp"
#define IPADD "getaddrinfo"
#define TFTP_PORT 69
#define MODE "octet"


void requestinfo(int argc, char *argv[], char *command,char *host, char *file){
	if( (strcmp(argv[1], GET) == 0 || strcmp(argv[1], PUT) == 0) && argc == 4 ){
		strcpy(command, argv[1]);
		strcpy(host, argv[2]);
		strcpy(file, argv[3]);
	}
}

void ipadress(int argc,char *argv[], char *address){
	
	if( strcmp(argv[1], IPADD) == 0 && argc == 3){

	struct addrinfo hints = {0};
	struct addrinfo *res, *p;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(argv[2], NULL, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return;
    } else {

		for (p = res; p != NULL; p = p->ai_next) {
			void *addr;
			char *ipver;
	
			// Get pointer to the address itself
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
			char ipstr[INET6_ADDRSTRLEN];
			inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
			
			if(strcmp(ipver,"IPv4") == 0){
				strcpy(address,ipstr);
			}
		}
    
	}
    freeaddrinfo(res); // Free the linked list

	}
}

int connection(int argc,char *argv[]){
	
	int sock = -1;
	
	if( ( strcmp(argv[1], GET) == 0 || strcmp(argv[1], PUT) == 0 ) && argc == 4){
	
	struct addrinfo hints = {0};
    struct addrinfo *res, *p;

    hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP Socket

	int status = getaddrinfo(argv[2], "80", &hints, &res); // Using "80" for HTTP port
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1; // Return error code
    }
    
    for (p = res; p != NULL; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == -1) {
            perror("socket error");
            continue; // Try the next address if this one fails
        }
	
		status = connect(sock, p->ai_addr, p->ai_addrlen);
        if (status == -1) {
            perror("connect error");
            close(sock);
            continue; // Try the next address if connection fails
        }

		break; // Successful connection, break out of the loop
    }
    
    freeaddrinfo(res); // Free the linked list of addresses

    if (sock == -1) {
        fprintf(stderr, "Failed to connect to the server.\n");
    }
    
	}
    return sock; // Return the connected socket or -1 if failed

}

void get(char *host, char *file){
	int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BSIZE];
    int message_length;

	//Socket creation
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        return;
    }
    //Server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TFTP_PORT);
	//Resolve host
	if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid host IP address format: %s\n", host);
        close(sockfd);
        return;
    }
    //RRQ
    buffer[0] = 0;
    buffer[1] = 1;
    //Filename
    strcpy(&buffer[2], file);
    int filename_length = strlen(file) + 1; // Including the null terminator
	//Transfermode
	strcpy(&buffer[2 + filename_length], MODE);
    int mode_length = strlen(MODE) + 1; // Including the null terminator
	//Message length
	message_length = 2 + filename_length + mode_length;
	//Send to the server
	if (sendto(sockfd, buffer, message_length, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto failed");
        close(sockfd);
        return;
    }
	printf("RRQ sent to %s for file: %s\n", host, file);

}

int main(int argc, char *argv[]){
		
	char command[BSIZE];
	char host[BSIZE];
	char file[BSIZE];
	char address[INET6_ADDRSTRLEN];
		
	requestinfo(argc, argv, command, host, file);
	
	ipadress(argc, argv, address);
	
	int sock = connection(argc,argv);
	
	if (sock != -1) {
        printf("Connected\n");
        
        close(sock);
    }
	
	return 0;

}
