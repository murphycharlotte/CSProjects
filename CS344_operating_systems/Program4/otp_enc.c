/*
*	File: otp_enc.c
*	Name: Charlotte Murphy
*	Course: CS 344-400
*	Assignment: Program 4
*	Due: June 9, 2017
*	Desc: **Code is largely from TopHat lecture 4.3 server.c, client.c, and multiserver.c examples**
*		A client program to transmit plain text and a cipher key to and receive a cipher text from an ENC server
*	Usage: For use with encoding server otp_enc_d.c
*/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include "encode.h"

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[]) 
{
	INPUT pInput, kInput;
	MESSAGE complete, encoded;

	initClientInput(&pInput, argv[1], &kInput, argv[2]);

	int socketFD, portNumber, charsWritten, charsRead; 
	struct sockaddr_in serverAddress; 
	struct hostent* serverHostInfo; 

	if (argc < 4) { fprintf(stderr, "USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args
																					  
	// Set up the server address struct 
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct 
	portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string 
	serverAddress.sin_family = AF_INET; // Create a network-capable socket 
	serverAddress.sin_port = htons(portNumber); // Store the port number 
	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address 
	if (serverHostInfo == (NULL)) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(2); } 
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	// Set up the socket 
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket 
	if (socketFD < 0) error("CLIENT: ERROR opening socket"); 

	// Connect to server 
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to addy 
		error("CLIENT: ERROR connecting");
	
	getLineFromFile(&pInput);
	getLineFromFile(&kInput);
	validateInput(&pInput, &kInput);
	initClientMessages(&complete, &encoded, &pInput, &kInput);
	buildCompleteMessage(&complete, &pInput, &kInput);
	
	int sentSoFar = 0;
	charsWritten = -5;
	do
	{
		// Send message to server 
		charsWritten = send(socketFD, complete.message + sentSoFar, 1024, 0); 	// Write to the server 
		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
		sentSoFar += charsWritten;
	} while (sentSoFar < complete.length + 1);

	int readSoFar = 0;
	charsRead = -5;
	// Get return message from server 
	do
	{
		charsRead = recv(socketFD, encoded.message + readSoFar, 1024, 0); // Read data from the socket, leaving \0 at end 
		if (charsRead < 0) error("CLIENT: ERROR reading from socket");

		readSoFar += charsRead;
	} while (strstr(encoded.message, "&") == NULL);


	validateServerConnection(&encoded);
	printMessage(&encoded);

	close(socketFD); 
	return 0;
}



/*	Additional Sources
*		https://www.daniweb.com/programming/software-development/code/216411/reading-a-file-line-by-line
*		http://www.sanfoundry.com/c-program-size-file-handling/
*		http://www.crasseux.com/books/ctutorial/getline.html
*		https://cboard.cprogramming.com/c-programming/134497-socket-programming-send-loop.html
*		https://stackoverflow.com/questions/32390478/what-is-the-return-value-of-strcspn-when-str1-does-not-contain-str2
*		http://www.c4learn.com/c-programming/c-arithmetic-operations-on-character/
*/