
const char * usage =
"                                                               \n"
"IRCServer:                                                   \n"
"                                                               \n"
"Simple server program used to communicate multiple users       \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   IRCServer <port>                                          \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"                                                               \n"
"In another window type:                                        \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where talk-server      \n"
"is running. <port> is the port number you used when you run    \n"
"daytime-server.                                                \n"
"                                                               \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "IRCServer.h"
#include "UserList.h"
#include "HashTableVoid.h"
int QueueLength = 5;

        LinkedList userlist;
        HashTableVoid a;
        HashTableVoid b;


int
IRCServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress; 
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);
  
	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
		perror("socket");
		exit( -1 );
	}

	// Set socket options to reuse port. Otherwise we will
	// have to wait about 2 minutes before reusing the sae port number
	int optval = 1; 
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
			     (char *) &optval, sizeof( int ) );
	
	// Bind the socket to the IP address and port
	int error = bind( masterSocket,
			  (struct sockaddr *)&serverIPAddress,
			  sizeof(serverIPAddress) );
	if ( error ) {
		perror("bind");
		exit( -1 );
	}
	
	// Put socket in listening mode and set the 
	// size of the queue of unprocessed connections
	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	return masterSocket;
}

void
IRCServer::runServer(int port)
{
	int masterSocket = open_server_socket(port);
	        //IRCServer ircServer;
//        LinkedList userlist;
  //      HashTableVoid a;
    //    HashTableVoid b;
	initialize();
	      
	while ( 1 ) {
		
		// Accept incoming connections
		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket,
					  (struct sockaddr *)&clientIPAddress,
					  (socklen_t*)&alen);
		
		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}
		
		// Process request.
		processRequest( slaveSocket );		
	}
}

int
main( int argc, char ** argv )
{
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}
	
	// Get the port from the arguments
	int port = atoi( argv[1] );

	IRCServer ircServer;
//	LinkedList userlist;
//	HashTableVoid a;
//	HashTableVoid b;
	// It will never return
	ircServer.runServer(port);
	
}

//
// Commands:
//   Commands are started y the client.
//
//   Request: ADD-USER <USER> <PASSWD>\r\n
//   Answer: OK\r\n or DENIED\r\n
//
//   REQUEST: GET-ALL-USERS <USER> <PASSWD>\r\n
//   Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//
//   REQUEST: CREATE-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LIST-ROOMS <USER> <PASSWD>\r\n
//   Answer: room1\r\n
//           room2\r\n
//           ...
//           \r\n
//
//   Request: ENTER-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LEAVE-ROOM <USER> <PASSWD>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: SEND-MESSAGE <USER> <PASSWD> <MESSAGE> <ROOM>\n
//   Answer: OK\n or DENIED\n
//
//   Request: GET-MESSAGES <USER> <PASSWD> <LAST-MESSAGE-NUM> <ROOM>\r\n
//   Answer: MSGNUM1 USER1 MESSAGE1\r\n
//           MSGNUM2 USER2 MESSAGE2\r\n
//           MSGNUM3 USER2 MESSAGE2\r\n
//           ...\r\n
//           \r\n
//
//    REQUEST: GET-USERS-IN-ROOM <USER> <PASSWD> <ROOM>\r\n
//    Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//

void
IRCServer::processRequest( int fd )
{
	// Buffer used to store the comand received from the client
	const int MaxCommandLine = 1024;
	char commandLine[ MaxCommandLine + 1 ];
	int commandLineLength = 0;
	int n;
	
	// Currently character read
	unsigned char prevChar = 0;
	unsigned char newChar = 0;
	
	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( commandLineLength < MaxCommandLine &&	read( fd, &newChar, 1) > 0 ) {

		if (newChar == '\n' && prevChar == '\r') {
			break;
		}
		
		commandLine[ commandLineLength ] = newChar;
		commandLineLength++;

		prevChar = newChar;
	}
	
	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
        commandLine[ commandLineLength ] = 0;

	printf("RECEIVED: %s\n", commandLine);

	printf("The commandLine has the following format:\n");
	printf("COMMAND <user> <password> <arguments>. See below.\n");
	printf("You need to separate the commandLine into those components\n");
	printf("For now, command, user, and password are hardwired.\n");

	//const char * command = "ADD-USER";
	//const char * user = "peter";
	//const char * password = "spider";
	//const char * args = "";

	char * command = (char*)malloc(sizeof(commandLine));
	char * user = (char*)malloc(sizeof(commandLine));
	char * password = (char*)malloc(sizeof(commandLine));
	char * args = (char*)malloc(sizeof(commandLine));
	char * commandLine1 = &commandLine[0];
	char * a = command;

	while(*commandLine1 != ' ') {
	
	*a = *commandLine1;
	a++;
	commandLine1 ++;
	
 	}
	
	//*a = '\n';
	commandLine1++;
	 
	char * b = user;
	while(*commandLine1 != ' ') {
        *b = * commandLine1;
        b++;
        commandLine1++;
         
      	}
	
	//*b = '\n';
	commandLine1++; 
	
	char * c = password;
        while(*commandLine1 != ' ') {
        *c = * commandLine1;
        c++;
        commandLine1 ++;
        
        }
         
        //*c = '\n';
        commandLine1++;

	char * d = args;
        while(*commandLine1!= ' ') {
        *d = * commandLine1;
        d++;
        commandLine1++;
        
        }
         
        //*d = '\n';
        commandLine1++;

	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("args=%s\n", args);

	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
 	else if (!strcmp(command, "CREATE-ROOM")) {
                createRoom(fd, user, password, args);
        }
	else if (!strcmp(command, "LIST-ROOMS")) {  
                listRooms(fd, user, password, args);
        }
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password);
	}
	else {
		const char * msg =  "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	// Send OK answer
	//const char * msg =  "OK\n";
	//write(fd, msg, strlen(msg));

	close(fd);	
}

void
IRCServer::initialize()
{
	// Open password file
	//Initialise users
	
	llist_init(&userlist);
	llist_read(&userlist, "pwd.ll");
	
	// Initalize message list
	
	
	
}

bool
IRCServer::checkPassword(int fd, const char * user, const char * password) {
	// Here check the password
	
	return llist_checkpwd(&userlist, user, password);
}

void
IRCServer::addUser(int fd, const char * user, const char * password, const char * args)
{
	// Here add a new user. For now always return OK.
	
//	if(!llist_exists(&userlist, user)) {
//	
//	llist_add(&userlist, user, password);
//	        const char * msg =  "OK\r\n";
  //      write(fd, msg, strlen(msg));
//	}
//	else {
	const char * msg =  "DENIED\r\n";
	write(fd, msg, strlen(msg));
//	}
	return;		
}

void
IRCServer::enterRoom(int fd, const char * user, const char * password, const char * args)
{
void *rand;
LinkedList * list1;
if(a.find(args, &rand) && llist_checkpwd(&userlist, user, password)) {
list1 = (LinkedList*) rand;
llist_add(list1, user, NULL);
const char * msg =  "OK\r\n";
        write(fd, msg, strlen(msg));
}

else {
const char * msg =  "DENIED\r\n";
        write(fd, msg, strlen(msg));
}

}

void
IRCServer::createRoom(int fd, const char * user, const char * password, const char * args) {

if(llist_checkpwd(&userlist, user, password)) {

LinkedList * roomlist;
LinkedList  * msglist;
llist_init(roomlist);
llist_init(msglist);
a.insertItem(args,roomlist);
b.insertItem(args, msglist);
const char * msg =  "OK\r\n";
        write(fd, msg, strlen(msg));
}

else {
const char * msg =  "DENIED\r\n";
        write(fd, msg, strlen(msg));
}
}
void
IRCServer::leaveRoom(int fd, const char * user, const char * password, const char * args)
{

if(llist_checkpwd(&userlist, user, password)) {

LinkedList * roomlist;
void *rand;
a.find(args, &rand);
roomlist = (LinkedList*)rand;
llist_remove(roomlist, user, password);
const char * msg =  "OK\r\n";
        write(fd, msg, strlen(msg));
}

else {
const char * msg =  "DENIED\r\n";
        write(fd, msg, strlen(msg));
}
}


void
IRCServer::listRooms(int fd, const char * user, const char * password, const char * args) {

if(llist_checkpwd(&userlist, user, password)) {
HashTableVoidIterator iterator(&a);
int sum=0;
  const char * key;
  void * rand;
  while (iterator.next(key, rand)) {
    printf("%s\n", key);
  }
}

else {
const char * msg =  "DENIED\r\n";
        write(fd, msg, strlen(msg));
}

}

void
IRCServer::sendMessage(int fd, const char * user, const char * password, const char * args)
{
char *room = (char*)malloc(sizeof(args));
char * msg = (char*)malloc(sizeof(args));

char * c = room;
char * d = msg;

while(*args != ' ') {
        
        *c = *args;
        c++;
        args++;
        
        }   
       
        *c = '\n';
        args++;
                
while(*args != '\n') {

        *d = *args;        
        d++;
        args++;

        }

        *d = '\n';

void *rand;
LinkedList * list1;
b.find(room,&rand);

list1 = (LinkedList*) rand;
llist_add(list1, user, msg);

}

void
IRCServer::getMessages(int fd, const char * user, const char * password, const char * args)
{
if(llist_checkpwd(&userlist, user, password)) {
int lmno = atoi(args);
while(*args != ' ') {
args++;
}

args++;

char *room = (char*)malloc(sizeof(args));

char * c = room;

while(*args != ' ') {

        *c = *args;
        c++;
        args++;

        }
  
        *c = '\n';

void *rand;
LinkedList * list1;
b.find(room,&rand);

list1 = (LinkedList*) rand;

llist_printmsg(list1);
}

else {
const char * msg =  "DENIED\r\n";
      write(fd, msg, strlen(msg));
}
}

void
IRCServer::getUsersInRoom(int fd, const char * user, const char * password, const char * args)
{
if(llist_checkpwd(&userlist, user, password)) {
void *rand;
LinkedList * list1;
a.find(args, &rand);
 
list1 = (LinkedList*) rand;
llist_print(list1);
}

else {
const char * msg =  "DENIED\r\n";
      write(fd, msg, strlen(msg));
}


}

void
IRCServer::getAllUsers(int fd, const char * user, const char * password)
{
if(llist_checkpwd(&userlist, user, password)) {

llist_print(&userlist);

}

else {
const char * msg =  "DENIED\r\n";
      write(fd, msg, strlen(msg));
}

}

