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


#define BSIZE 1024
#define GET "gettftp"
#define PUT "puttftp"
#define IPADD "getaddrinfo"

void requestinfo(int argc, char *argv[], char *host, char *file){
	if( (strcmp(argv[1], GET) == 0 || strcmp(argv[1], PUT) == 0) && argc == 4 ){
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

int main(int argc, char *argv[]){
		
	char host[BSIZE];
	char file[BSIZE];
	char address[INET6_ADDRSTRLEN];
		
	requestinfo(argc, argv, host, file);
	
	ipadress(argc, argv, address);
	
	printf("IP: %s\n", address);
	
	return 0;

}

