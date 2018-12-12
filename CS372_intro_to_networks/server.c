/*
* File: server.c
* Author: Charlotte Murphy
* Course: CS 372 - 400
* Due:	March 11, 2018
* Description: Project 2 - TCP file transfer
*   TCP Server - functions for server code. Receives client requests for directory contents
*	or text files over a control connection.
*   Send requested data over a separate data connection.
*   Code based on TCP client example provided in lecture #15 and project 1
* Usage: For use with ft_server.c, ftclient, and server.h
*/



#include "server.h"

// Error function used for reporting issues
void error(const char *msg) { perror(msg); exit(1); } 


// Function: initServer
// Initializes COMMAND object buffer size
// memsets strings used
// initializes error strings
// INPUT: COMMAND object pointer and int buffer size
// Pre: COMMAND object declared, buffer size declared and initialized
// Post COMMAND strings prepped for functions and buffer size set
void initServer(COMMAND* in, int bufsize) {
	in->bufsize = bufsize;

	memset(in->host, '\0', sizeof(in->host));

	memset(in->service, '\0', sizeof(in->service));

	memset(in->request, '\0', sizeof(in->request));

	memset(in->data, '\0', sizeof(in->data));

	memset(in->fileName, '\0', sizeof(in->fileName));

	memset(in->ack, '\0', sizeof(in->request));
	strncpy(in->ack, "ACK", sizeof(in->ack));

	memset(in->errInvalidPort, '\0', sizeof(in->errInvalidPort));
	strncpy(in->errInvalidPort, "INVALID DATA PORT NUMBER", in->bufsize);

	memset(in->errInvalidCommand, '\0', sizeof(in->errInvalidCommand));
	strncpy(in->errInvalidCommand, "INVALID COMMAND", in->bufsize);

	memset(in->errIncorrectUsage, '\0', sizeof(in->errIncorrectUsage));
	strncpy(in->errIncorrectUsage, "INCORRECT USAGE", in->bufsize);

	memset(in->errFileNotFound, '\0', sizeof(in->errFileNotFound));
	strncpy(in->errFileNotFound, "FILE NOT FOUND OR FILE NOT ACCESSIBLE", in->bufsize);

	memset(in->errNoFileNameSent, '\0', sizeof(in->errNoFileNameSent));
	strncpy(in->errNoFileNameSent, "NO FILE NAME SENT", in->bufsize);

	memset(in->errInvalidFileType, '\0', sizeof(in->errInvalidFileType));
	strncpy(in->errInvalidFileType, "INVALID FILE TYPE", in->bufsize);
}


// Function: startup
// Creates a control socket, listens on control connection
// Loops through the following steps until interrupted:
//		If a connection, then receives a string representing a client request
//		Evaluates request, if request valid, ACK is sent to client on the
//		control connection, then connect to client's data connection to send requested data
//		If request is not valid, an error message is sent on control connection
// Input: port number and COMMAND object pointer
// Pre: COMMAND object strings initialized & buffer set and port number reveived
// via command line parameters and saved to portNumber variable & validated
// Post: Control socket created, listened on control socket for possible connections
// Exits when interrupted 
void startup(int portNum, COMMAND* in) 
{
	int controlSocketFD = -1, controlConnectionFD = -1, dataConnectionFD = -1;
	socklen_t sizeOfClientInfo;
	struct sockaddr_in serverAddress, clientAddress;

	// Set up the CONTROL address struct for this process (the server) 
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct 
	serverAddress.sin_family = AF_INET; // Create a network-capable socket 
	serverAddress.sin_port = htons(portNum); // Store the port number 
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the CONTROL socket 
	controlSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket 
	if (controlSocketFD < 0) error("SERVER: startup ERROR opening socket");
	if (setsockopt(controlSocketFD, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
		error("setsockopt(SO_REUSEADDR) failed");
	// Enable the CONTROL socket to begin listening 
	if (bind(controlSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port 
		error("ERROR on binding");
	if (listen(controlSocketFD, 5) < 0)	{
		error("listen() failed"); // Flip the socket on - it can now receive up to 5 connections
	}
	
	printf("\nServer open on %d\n", portNum);

	while (1)   // 4.3 multiserver example
	{

		//  Accept a connection, blocking if one is not available until one connects 
		sizeOfClientInfo = sizeof(clientAddress); //  Get the size of the address for the client that will connect 
		controlConnectionFD = accept(controlSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); //  Accept 
		if (controlConnectionFD < 0) { error("ERROR on accept"); }
		
		// Get client host name
		int s = getnameinfo((struct sockaddr *)&clientAddress, sizeOfClientInfo, in->host, NI_MAXHOST, in->service, NI_MAXSERV, NI_NUMERICSERV);
		if (s != 0) { error("getnameinfo() error"); }

		printf("\nConnection from %s\n", in->host); 
		
		pid_t spawnpid = -5;
		int childExitMethod = -5;
		int isCommandValid = -1;

		// fork process if connection is established
		spawnpid = fork();
		switch (spawnpid) 
		{
			case -1: 
			{
				error("fork() failed\n\n");
				break;
			}
			case 0: // child receives a command, validates command, and handles command
			{
				isCommandValid = getCommand(controlConnectionFD, in);
				if (isCommandValid != -1)
				{
					// send ACK to client
					sendData(controlConnectionFD, in->ack, strlen(in->ack), in->bufsize);
					
					// Set up the socket 
					dataConnectionFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket 
					if (dataConnectionFD < 0) error("SERVER: ERROR opening data socket");
					
					// change client port to port for data connection
					clientAddress.sin_port = htons(in->dataPort);
					
					// ?? reuse address
					if (setsockopt(dataConnectionFD, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
						error("SERVER: setsockopt(SO_REUSEADDR) failed");
					}
					
					int connected = -1;
					int tries = 0;
					do {
						// Connect to data server address
						connected = connect(dataConnectionFD, (struct sockaddr*)&clientAddress, sizeof(clientAddress));  
						tries++;
					} while (connected == -1 && tries < 200);
					if (connected == -1) { error("SERVER: ERROR connecting to data connection");}
					
					handleCommand(in, controlConnectionFD, dataConnectionFD, ".", &in->dataReply);
					close(dataConnectionFD);
				}
				// exit process once completed - instead of break 
				exit(0);
			}
			default:  // parent calls wait/no hang
			{
				waitpid(spawnpid, &childExitMethod, WNOHANG);
				break;
			}
		}
		if(controlConnectionFD != -1) { close(dataConnectionFD); }
	} // end while(1) statement

	close(controlSocketFD); //  Close the listening socket 
}


// Function: getCommand()
// Subfunction of startup
// Receives string representing client request components
// makes calls to parseCommand() and validateCommand()
// returns value of validateCommand
// INPUT: Control socket file descriptor and COMMAND object pointer
// Pre: Client connected to control socket
// Post: Request string received and passed to parseCommand & validateCommand
int getCommand(int ctrlSock, COMMAND* in) {
	//int charsRead = -5, readSoFar = 0;
	//charsRead = recv(ctrlSock, in->request + readSoFar, 1024, 0); // Read data from the socket, leaving \0 at end 
	//if (charsRead < 0) error("SERVER: ERROR reading from socket");
	//readSoFar += charsRead;

	// Get command from client 
	receiveData(ctrlSock, in->request, sizeof(in->request));

	// check for term delim in request
	if (strchr(in->request, '&') == '\0') {
		printf("%s\n", in->request);
		error("Terminating delimiter not found in request");
	}
	// parse request into respective components
	parseCommand(in);

	// validate command
	int valid = validateCommand(ctrlSock, in);

	return valid;
}


// Function: parseCommand()
// Subfunction of getCommand
// Extracts the individual components of the client request string
// and saves them to their respective variables
// INPUT: COMMAND* object
// Pre: Request string receives and saved
// Post: Request components saved to their respective strings
void parseCommand(COMMAND* in) {
	char tempReq[2048];
	memset(tempReq, '\0', sizeof(tempReq));
	char *tokenPtr = NULL;

	// Copy of cliend command request for tokenizing
	strcpy(tempReq, in->request);	

	// store data port number
	tokenPtr = strtok(tempReq, " ");
	if (tokenPtr == NULL) { fprintf(stderr, "SERVER: Identifier strtok failed\n"); exit(1); }
	in->dataPort = atoi(tokenPtr);
	//printf("parse data port %d  \n", in->dataPort);

	// store command
	tokenPtr = strtok(NULL, " &");
	if (tokenPtr == NULL) { fprintf(stderr,  "SERVER: Command strtok failed\n"); exit(1); }
	memset(in->command, '\0', sizeof(in->command)); //clear command memory
	strcpy(in->command, tokenPtr); //store command
	//printf("parse command %s  \n", in->command);

	// store file name	
	tokenPtr = strtok(NULL, " &");
	// if command -g and tokenPtr is null, send error
	if (strcmp(in->command, "-g") == 0 && tokenPtr == NULL) {
		error("Filename strtok failed");
	}
	// if token pointer not null, store file name
	if (tokenPtr != NULL) {
		memset(in->fileName, '\0', sizeof(in->fileName)); //clear filename memory
		strcpy(in->fileName, tokenPtr); //store filename
		//printf("parse filename %s \n", in->filename);
	}
}


// Function: validateCommand
// Subfunction of getCommand
// Compares the request components to expected values
// If the request components are all valid, return 0
// Else, send error message to client over control connection and return -1
// INPUT: Control socket file descriptor and COMMAND object pointer
// Pre: Client request string is parsed and stored in their respective strings
// Post: Request components are evaluated, and return 0 or -1 for valid or not
int validateCommand(int ctrlSock, COMMAND * in) {

	// validate data port number
	//    if not valid, send error, return -1
	if (in->dataPort < 1024 || 65535 < in->dataPort) {
		// Send invalid data port error
		sendData(ctrlSock, in->errInvalidPort, sizeof(in->errInvalidPort), in->bufsize);
		return -1;
	}
	
	// validate command is -l or -g
	//    if not valid, send error, return -1
	if (strcmp(in->command, "-g") != 0 && strcmp(in->command, "-l") != 0) {
		// send invalid command error
		sendData(ctrlSock, in->errInvalidCommand, sizeof(in->errInvalidCommand), in->bufsize);
		return -1;
	}

	// if command is -g, ensure filename string is not empty
	//    if not valid, send error, return -1
	if (strcmp(in->command, "-g") == 0 && in->fileName[0] == '\0') {
		// send no file name error
		sendData(ctrlSock, in->errNoFileNameSent, sizeof(in->errNoFileNameSent), in->bufsize);
		return -1;
	}

	// if command i -l and fileName is an empty string, send list directory message
	// else, if command = -l and a filename was received, incorrect usage
	if (strcmp(in->command, "-l") == 0) {
		if (in->fileName[0] != '\0') {
			// send incorrect usage error
			sendData(ctrlSock, in->errIncorrectUsage, sizeof(in->errIncorrectUsage), in->bufsize);
			return -1;
		}
		printf("List directory requested on port %d\n", in->dataPort);
	}
	
	// if command is -g, validate fileName
	//    send requested file message
	//    if fileName not valid, send error & return -1
	else if (strcmp(in->command, "-g") == 0) {\
		printf("File \"%s\" requested on port %d\n", in->fileName, in->dataPort);
		FILE* fp = fopen(in->fileName, "r");
		if (fp == NULL) {
			// send file not found error
			sendData(ctrlSock, in->errFileNotFound, sizeof(in->errFileNotFound), in->bufsize);
			return -1;
		}
		else { fclose(fp); } // close file

		// check last 4 chars of file name for .txt
		int len = strlen(in->fileName);
		char* tmp;
		tmp = in->fileName;
		if (tmp[len - 4] != '.' ||
			tmp[len - 3] != 't' ||
			tmp[len - 2] != 'x' ||
			tmp[len - 1] != 't') {
			sendData(ctrlSock, in->errInvalidFileType, sizeof(in->errInvalidFileType), in->bufsize);
			return -1;
		}
	}
	
	// if all elements of command are valid, return 0
	return 0;
}


// Function: handleCommand()
// Subfunction of startup()
// Evaluates client command and sends the requested data
// INPUT: COMMAND object pointer, Control socket file desc, data connection file desc, 
//		directory to check, pointer to a char array to hold requested data
// Pre: Client request components are all valid and connected to client's data connection
// Post: Requested data sent via data connection
void handleCommand(COMMAND * in, int ctrlSocket, int dConnection, char* path, char** dString) {
	// if cmd is -l, send directory contents
	if (strcmp(in->command, "-l") == 0) {
		sendDirectoryContents(in, ctrlSocket, dConnection, dString);
	}
	// else send file
	else if (strcmp(in->command, "-g") == 0) {
		sendFile(in, ctrlSocket, dConnection, dString);
	}
		
	return;
}


// Function: sendDirectoryContent()
// Subfunction of handleCommand()
// Sends directory contents to client as a single concatenated string
// INPUT: COMMAND object pointer, data connection file desc, pointer 
//		to a char array to hold requested data
// Pre: Client request components are all valid, connected to client's data connection, 
//		command is "-l"
// Post: Directory contents data sent via data connection
int sendDirectoryContents(COMMAND *in, int ctrlSocket, int dConnection, char** dString) {
	// Send list directory message
	printf("Sending directory contents to %s:%d\n", in->host, in->dataPort);

	// allocate dString memory
	int capacity = in->bufsize;
	*dString = malloc(capacity * sizeof(char));
	memset(*dString, '\0', sizeof(*dString));

	DIR* dirToCheck;
	struct dirent* fileInDir;
	dirToCheck = opendir("."); //open this directory.

	// loop to concatenate file names to a string separated by spaces
	// validate directory
	if (dirToCheck > 0)
	{
		// while there are files in directory, get the next file
		while ((fileInDir = readdir(dirToCheck)) != NULL)
		{
			// check available char[] memory, double the capacity if needed
			if (strlen(*dString) + strlen(fileInDir->d_name) + 3 > sizeof(*dString)) 
			{
				// allocate new memory, copy old contents to new memory, 
				// free old memeory, point char* to new memory
				capacity *= 2;
				char* temp = malloc(capacity * sizeof(char));
				memset(temp, '\0', sizeof(temp));
				strcat(temp, *dString);
				free(*dString);
				*dString = temp;
			}
			// if file name is not "." or "..", then append file name and a space char to string
			if (strcmp(fileInDir->d_name, "..") != 0)
			{
				if (strcmp(fileInDir->d_name, ".") != 0)
				{
					strcat(*dString, fileInDir->d_name);
					strcat(*dString, " ");
				}
			}
		} // end while loop
	}

	// validate string is not empty
	if (*dString[0] == '\0') 
	{
		error("error reading directory contents");
	}
	// printf("dString: %s\n", *dString);

	// send directory string to client
	sendData(dConnection, *dString, strlen(*dString), in->bufsize);

	return 0;
}


// Function: sendFile()
// Subfunction of handleCommand()
// Sends text file to client as a string
// INPUT: COMMAND object pointer, data connection file desc, pointer 
//		to a char array to hold requested data
// Pre: Client request components are all valid, connected to client's data connection, 
//		command is "-g"
// Post: text file sent via data connection
int sendFile(COMMAND * in, int ctrlSocket, int dConnection, char ** dString) {
	// file requested message
	printf("Sending \"%s\" to %s:%d\n", in->fileName, in->host, in->dataPort);

	char buffer[1024];
	size_t bytes = -1;
	int charsWritten = -1;
	int sentSoFar = 0;
	int fd,fileLength;
	char fileLenString[64];
	struct stat fileStats;

	// open file, create read/send char buffer, var for bytes sent
	FILE* fp = fopen(in->fileName, "r");
	if (fp == NULL) {
		error("File not found after validateCommand validated file");
	}
	
	// get file size without fseek/ftell
	fd = fileno(fp);
	fstat(fd, &fileStats);
	fileLength = fileStats.st_size;

	/*
	// save length of file as a string and send file length to client
	memset(fileLenString, '\0', sizeof(fileLenString));
	sprintf(fileLenString, "%d", (int)fileLength);
	charsWritten = send(dConnection, fileLenString, sizeof(fileLenString), 0);
	if (charsWritten < 0) error("File length send error");
	*/

	// read file contents to a string in in->bufsize chunks
	// send those chunks one at a time to client
	// checking bytes read from file compared to bytes sent to client
	// checking bytes sent so far compared to length of file
	// validating bytes read and chars sent are each !< 0
	sentSoFar = 0;
	charsWritten = -1;
	do {
		memset(buffer, '\0', sizeof(buffer));
		bytes = fread(buffer, sizeof(char), sizeof(buffer), fp);
		if (bytes < 0) error("fread error");
		charsWritten = send(dConnection, buffer, sizeof(buffer), 0);
		if (charsWritten < 0) error("File send error");
		sentSoFar += charsWritten;
	} while ((sentSoFar < (int)fileLength) && (bytes >= 0) && ((int)bytes == charsWritten));
	// printf("fileLength %d  sentSoFar %d  bytes %d  charsWritten %d\n", fileLength, sentSoFar, (int)bytes, charsWritten);

	// close file
	fclose(fp);

	return 0;
}


// Function: receiveData()
// Receives data from client, upto bufsize chars, from client
// Saves data received to char* parameter
// INPUT: connection file desc, char*, buffer size
// Pre: Connection established with client
// Post: Data received stored in char* data parameter
void receiveData(int connectionSock, char* data, int bufsize) {
	int charsRead = -5;

	// Read data from the socket
	charsRead = recv(connectionSock, data, bufsize, 0);
	if (charsRead < 0) { error("ERROR reading from socket"); }

	//printf("incoming message \"%s\"  charsRead %d  readSoFar %d\n", data, charsRead, readSoFar);
}


// Function: sendData()
// Sends data to client, in chunks of bufsize chars, until all data sent
// INPUT: connection file desc, char*, length of data stored in char*, buffer size
// Pre: Connection established with client
// Post: Data stored in char* data parameter sent to client
void sendData(int connectionSock, char * data, int dataLen, int bufsize) {
	int charsWritten = -1;
	int sentSoFar = 0;

	// send data to server in bufsize chunks, one at a time until all data sent
	// validating the charsWritten !< 0
	do {
		charsWritten = send(connectionSock, data + sentSoFar, bufsize, 0); 	// Write to the server 
		if (charsWritten < 0) error("ERROR writing to socket");
		sentSoFar += charsWritten;
	} while (sentSoFar < dataLen);
}