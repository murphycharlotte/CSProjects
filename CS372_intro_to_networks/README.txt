/*
* File: README.txt
* Author: Charlotte Murphy
* Course: CS 372 - 400
* Due:	March 11, 2018
* Description: Project 2 - TCP file transfer
*	Header file for server.c
*   TCP Server - functions for server code. Server client requests
*	for directory contents or text files over a control connection.
*   Send requested data over a separate data connection.
*   Code based on TCP client example provided in lecture #15 and project 1
* Usage: For use with ft_server.c, ftclient, server.c, server.h
*/


Server instructions:
	Command to compile:
	gcc -o ft_server ft_server.c server.c

	Command to start server:
	./ft_server <SERVER_PORT>


Client instructions:
	Command to authorize execution:
	chmod u+x ftclient

	Command to start client:
		Command to request a TEXT file:
		./ftclient <SERVER_HOST> <SERVER_PORT> -g <FILE_NAME> <DATA_PORT>

		Command to request directory contents:
		./ftclient <SERVER_HOST> <SERVER_PORT> -l <DATA_PORT>

