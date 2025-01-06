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

#define BSIZE 1024
#define GET "gettftp"
#define PUT "puttftp"

void requestinfo(char *argv[], char *host, char *file){
	if( strcmp(argv[1], GET) == 0 || strcmp(argv[1], PUT) == 0){
		strcpy(host, argv[2]);
		strcpy(file, argv[3]);
	}
}


int main(int argc, char *argv[]){
		
	char host[BSIZE];
	char file[BSIZE];
		
	requestinfo(argv,host,file);
	
	printf("host: %s\n", host);
	printf("file: %s\n", file);

	return 0;

}

