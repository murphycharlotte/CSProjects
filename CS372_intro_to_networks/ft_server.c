/*
*	File:	ft_server.c
*	Name:	Charlotte Murphy
*	Course:	CS 372-400
*	Assignment:	Project #2
*	Due:	March 11, 2018
*	Description:	A server program that supports simple file transfer system on a 
*	2-connection client-server network. The server receives request from the client
*	via control connection and send the requested data to the client via a 
*	separate data connection.
*   Code based on TCP client example provided in lecture #15 and project 1
*   Additional sources listed below code
*	Usage: for use with server.c, server.h, and ftclient
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include  "server.h"
#include <stdint.h>

#define	BUFSIZE 1024


int main(int argc, char *argv[]) {

	COMMAND incoming; // struct for initial string from client

	initServer(&incoming, BUFSIZE);

	int portNumber;
	if (argc < 2) { fprintf(stderr, "SERVER: USAGE: %s port\n", argv[0]); exit(1); } //  Check usage & args
	portNumber = atoi(argv[1]); //  Get the port number, convert to an integer from a string 

	startup(portNumber, &incoming);
		
	return 0;
}


// sources
// http://man7.org/linux/man-pages/man3/getaddrinfo.3.html
// https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c
// https://www.linuxquestions.org/questions/programming-9/send-a-file-through-raw-socket-using-c-programming-821751/
// http://www.cs.put.poznan.pl/csobaniec/examples/sockets/server-tcp-file.c// 
// https://linux.die.net/man/2/stat
// http://pubs.opengroup.org/onlinepubs/009695399/functions/fstat.h
// https://stackoverflow.com/questions/11952898/c-send-and-receive-file
// https://stackoverflow.com/questions/11291154/save-file-listing-into-array-or-something-else-c
// https://stackoverflow.com/questions/41653419/c-store-a-list-of-files-in-a-directory-into-an-array
// http://people.cs.uchicago.edu/~dmfranklin/tutorials/strtok.txt
// http://users.pja.edu.pl/~jms/qnx/help/tcpip_4.25_en/prog_guide/sock_advanced_tut.html
// ftp://gaia.cs.umass.edu/pub/kurose/ftpserver.c
// https://tools.ietf.org/html/rfc959
// http://www.cs.dartmouth.edu/~campbell/cs60/socketprogramming.html
// http://slacksite.com/other/ftp.html
// https://www.codeproject.com/Articles/293391/File-Transfer-Protocol-FTP-Client
// http://www.rhyshaden.com/ftp.htm
// http://ftp.icpdas.com/pub/cd/8000cd/napdos/7186e/demo/basic/bc_tc/ethernet/tcp/ftp/
// https://www.go4expert.com/forums/simple-implementation-ftp-c-t14938/
// http://www.linuxhowtos.org/C_C++/socket.htm

/*	Additional Sources cited for CS 372 submission
*		https://www.daniweb.com/programming/software-development/code/216411/reading-a-file-line-by-line
*		http://www.sanfoundry.com/c-program-size-file-handling/
*		http://www.crasseux.com/books/ctutorial/getline.html
*		https://cboard.cprogramming.com/c-programming/134497-socket-programming-send-loop.html
*		https://stackoverflow.com/questions/32390478/what-is-the-return-value-of-strcspn-when-str1-does-not-contain-str2
*		https://stackoverflow.com/questions/2279379/how-to-convert-integer-to-char-in-c
*		http://www.c4learn.com/c-programming/c-arithmetic-operations-on-character/
*/