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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define LINE "$ "
#define GET "gettftp"
#define PUT "puttftp"
#define BSIZE 1024

void line(){
	write(STDOUT_FILENO, LINE, strlen(LINE));
}

int request(char *input, char *command, char *host, char *file){
    
    char buffer[BSIZE];
    // The user command
	ssize_t usercommand = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    // Remove newline and null-terminate
    buffer[usercommand - 1] = '\0';	
    
    // Split input into 3 parts: command, host and file
	if (sscanf(buffer, "%s %s %s", command, host, file) <3){
		fprintf(stderr,"Invalid input (<command>tftp <host> <file>)\n");
		return 1;
	// Check the command
	} else if (strcmp(command, GET) != 0 && strcmp(command, PUT) != 0) {
		fprintf(stderr, "Unknown command (gettftp or puttftp)\n");
		return 1;
	}
	return 0;
}

void ipadress(const char *host){
	struct addrinfo hints = {0};
	struct addrinfo *res, *p;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(host, NULL, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return;
    } else {
		printf("IP addresses for %s:\n", host);

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
			printf("  %s: %s\n", ipver, ipstr);
		}
    
	}
    freeaddrinfo(res); // Free the linked list
}

int main(){
	while(1){
		
		line();
		
		char input[BSIZE];
		char command[BSIZE];
		char host[BSIZE];
		char file[BSIZE];
		
		if( request(input, command, host, file) ){ continue; } // QUESTION 1
				
		ipadress(host);	
	}
	return 0;
}
