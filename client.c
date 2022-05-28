#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include "commands.h"
#include "prompts.h"



int main(int argc, char *argv[])
{
    char *connection_cookie = NULL;
    char *auth_token = NULL;
    int sockfd;
    int is_logged = 0;

    /* set server info */
    char *ip = malloc(MAX_IP * sizeof(char));
    strcpy(ip, "34.241.4.235");

    int port = 8080;
    sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);

    char *command = malloc(MAX_COMMAND * sizeof(char));
    char *username = malloc(MAX_USERNAME * sizeof(char));
    char *password = malloc(MAX_PASSWORD * sizeof(char));

    while(1) {
        /* get a new command from the user */
        fgets(command, MAX_COMMAND, stdin);
        command[strcspn(command, "\n")] = 0;

        /* check if it's a valid command and execute specific action */
        if(strcmp(command, "register") == 0) {
            register_prompt(username, password);
            register_to_sv(&sockfd, ip, username, password);
        } else if(strcmp(command, "login") == 0) {
            if(is_logged == 0) {
                login_prompt(is_logged, username, password);
                connection_cookie = login_to_sv(&sockfd, ip, username, password);
                if(connection_cookie != NULL) {
                    is_logged = 1;
                }
            } else {
                printf("You are already logged in! Please log out first to switch accounts\n");
            }
 
        } else if(strcmp(command, "enter_library") == 0) {
            auth_token = enter_library(&sockfd, ip, connection_cookie);
        } else if(strcmp(command, "get_books") == 0) {
            get_books(&sockfd, ip, auth_token);
        } else if(strcmp(command, "get_book") == 0) {
            char *book_id = malloc(MAX_DIGITS_ID * sizeof(char));
            get_book_prompt(book_id);
            get_book(&sockfd, ip, auth_token, book_id);
            free(book_id);
        } else if(strcmp(command, "add_book") == 0) {   
            book *new_book = malloc(sizeof(book));
            add_book_prompt(new_book);
            add_book(&sockfd, ip, auth_token, new_book);
            free_book(new_book);
        } else if(strcmp(command, "delete_book") == 0) {
            char *book_id = malloc(MAX_DIGITS_ID * sizeof(char));
            delete_book_prompt(book_id);
            delete_book(&sockfd, ip, auth_token, book_id);
            free(book_id);
        } else if(strcmp(command, "logout") == 0) {
            logout(&sockfd, ip, connection_cookie);
            is_logged = 0;

            /* reset connection cookie and authorization token */ 
            if(connection_cookie) {
                free(connection_cookie);
                connection_cookie = NULL;
            }
            if(auth_token) {
                free(auth_token);
                auth_token = NULL;
            }
        } else if(strcmp(command, "exit") == 0) {
            break;
        } else {
            invalid_command();
        }
    }

    /* free resources */
    free(ip);
    free(command);
    free(username);
    free(password);
    if(connection_cookie){
        free(connection_cookie);
    }
    if(auth_token) {
        free(auth_token);
    }
    close_connection(sockfd);

    return 0;
}
