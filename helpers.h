#ifndef _HELPERS_
#define _HELPERS_

#include "commands.h"

#define BUFLEN 4096
#define LINELEN 1000
#define MAX_IP 17
#define MAX_COMMAND 50
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_BODY_FIELDS 10
#define MAX_FIELD 300
#define MAX_COOKIES 20
#define MAX_COOKIE_SIZE 200
#define MAX_DIGITS_ID 7
#define MAX_TITLE 50
#define MAX_AUTHOR 30
#define MAX_GENRE 15
#define MAX_PUBLISHER 30
#define MAX_MSG 200
#define MAX_TOKEN 500
#define MAX_BOOK 100
#define MAX_BOOK_DETAILED 300
#define URL_SIZE 50
#define CONT_TYPE_SIZE 30
#define MAX_PAGE_CNT_DIGITS 5

// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

#endif
