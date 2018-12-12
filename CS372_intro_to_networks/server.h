/*
* File: server.h
* Author: Charlotte Murphy
* Course: CS 372 - 400
* Due:	March 11, 2018
* Description: Project 2 - TCP file transfer
*	Header file for server.c
*   TCP Server - functions for server code. Server client requests for directory contents
*	or text files over a control connection.
*   Send requested data over a separate data connection.
*   Code based on TCP client example provided in lecture #15 and project 1
* Usage: For use with ft_server.c, ftclient, and server.c
*/

#ifndef SERVER
#define SERVER


#pragma once
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdint.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <arpa/inet.h>


typedef struct  command {
	size_t	bufsize;

	char	host[NI_MAXHOST];
	char	service[NI_MAXSERV];

	char	request[2048];

	char	data[1024];
	int		dataPort;

	char	command[1024];

	char	fileName[1024];

	char	ack[1024];

	char	errInvalidPort[1024];
	char	errInvalidCommand[1024];
	char	errIncorrectUsage[1024];
	char	errFileNotFound[1024];
	char	errNoFileNameSent[1024];
	char	errInvalidFileType[1024];

	char*	dataReply;
} COMMAND;


void	error(const char *msg);
void	initServer(COMMAND* in, int bufsize);

void	startup(int portNum, COMMAND* in);
int		getCommand(int ctrlSock, COMMAND* in);
void	parseCommand(COMMAND* in);
int		validateCommand(int ctrlSock, COMMAND* in);
void	handleCommand(COMMAND* in, int ctrlSocket, int dConnection, char* path, char** dString);

int		sendDirectoryContents(COMMAND* in, int ctrlSocket, int dConnection, char** dString); 
int		sendFile(COMMAND* in, int ctrlSocket, int dConnection, char** dString); 
void	receiveData(int connectionSock, char* data, int bufsize);
void	sendData(int connectionSock, char* data, int dataLen, int bufsize);

#endif // !SERVER
