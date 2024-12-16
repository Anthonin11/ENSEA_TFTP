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

#define LINE "$ "
#define GET "gettftp host file"
#define PUT "puttftp host file"
#define BSIZE 1024

void line(){
	write(STDOUT_FILENO, LINE, strlen(LINE));
}

void request(char *input, char *command, char *host, char *file){
    
    char buffer[BSIZE];
    // The user command
	ssize_t usercommand = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    // Remove newline and null-terminate
    buffer[usercommand - 1] = '\0';	
    
    // Split input into 3 parts: command, host and file
	if (sscanf(buffer, "%s %s %s", command, host, file) <3){
		fprintf(stderr,"Invalid input (<command>tftp <host> <file>)\n");
	// Check the command
	} else if (strcmp(command, "gettftp") != 0 && strcmp(command, "puttftp") != 0) {
		fprintf(stderr, "Unknown command (gettftp or puttftp)\n");
	}
}

int main(){
	while(1){
		
		line();
		
		char input[BSIZE];
		char command[BSIZE];
		char host[BSIZE];
		char file[BSIZE];
		request(input, command, host, file); // QUESTION 1
				
	}
	return 0;
}

