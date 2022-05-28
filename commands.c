#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <errno.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"


void print_books_info(char *json_book_list)
{
    /* check if library is empty */
    if(json_book_list == NULL) {
        printf("There are no books in the library. Use \"add_book\" command to add a new book!\n");
        return;
    }
    
    /* get each book */
    char *curr_book = malloc(MAX_BOOK * sizeof(char));
    while(*json_book_list == '{') {
        /* get next book from the list */
        char *end_string = strchr(json_book_list, '}');
        int end_pos = (int)(end_string - json_book_list);
        
        strncpy(curr_book, json_book_list, end_pos + 1);
        curr_book[end_pos + 1] = '\0'; 

        /* parse string into a JSON */
        JSON_Value *val = json_parse_string(curr_book);
        JSON_Object *obj = json_value_get_object(val);
        
        printf("ID: %d, ", (int)json_object_get_number(obj, "id"));
        printf("Title: %s\n", json_object_get_string(obj, "title"));
        json_book_list += (strlen(curr_book) + 1);

        json_value_free(val);
        
    }
    free(curr_book);
}

void print_book_detailed(char *book)
{
    /* get book from JSON string (given as a parameter)*/
    char *curr_book = malloc(MAX_BOOK_DETAILED * sizeof(char));
    char *end_string = strchr(book, '}');
    int end_pos = (int)(end_string - book);
    
    strncpy(curr_book, book, end_pos + 1);
    curr_book[end_pos + 1] = '\0'; 

    /* parse string into a JSON */
    JSON_Value *val = json_parse_string(curr_book);
    JSON_Object *obj = json_value_get_object(val);
    
    printf("\tTitle: %s\n", json_object_get_string(obj, "title"));
    printf("\tAuthor: %s\n", json_object_get_string(obj, "author"));
    printf("\tPublisher: %s\n", json_object_get_string(obj, "publisher"));
    printf("\tGenre: %s\n", json_object_get_string(obj, "genre"));
    printf("\tPage count: %d\n", (int)json_object_get_number(obj, "page_count"));

    json_value_free(val);
        
    free(curr_book);
}

char *get_response_status(char *response) 
{
    /* check if it is succes/error */
    char *success = "HTTP/1.1 2";
    char *error = "HTTP/1.1 4";
    printf("resp:%s\n", response);
    if(strstr(response, success) != NULL) {
        return "Success";
    } else if(strstr(response, error) != NULL) {
        char *get_err = strstr(response, "\"error\":\"");
        get_err += strlen("\"error\":\"");
        char *err_msg = strtok(get_err, "\"");
        char *final_msg = malloc(MAX_MSG * sizeof(char));
        sprintf(final_msg, "Error: %s\n", err_msg);
        return final_msg;
    } else {
        return "Something";
    }
}

void free_book(book *curr_book)
{
    free(curr_book->title);
    free(curr_book->author);
    free(curr_book->genre);
    free(curr_book->publisher);
    free(curr_book);
}


char *get_token(char *response) 
{
    /* extract auth token from response */
    char *needle = malloc(20 * sizeof(char));
    strcpy(needle, "\"token\":\"");
    char *tkn = strstr(response, needle) + strlen(needle);

    char *p = strchr(tkn, '}');
    *(p - 1) = '\0';

    free(needle);

    char *auth_token = malloc(MAX_TOKEN * sizeof(char));
    strncpy(auth_token, tkn, strlen(tkn));
    auth_token[strlen(tkn)] = '\0';

    return auth_token;
}

void register_to_sv(int *sockfd, char *ip, const char *username, const char *password) {

    *sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

    char *url = malloc(URL_SIZE * sizeof(char));
    strcpy(url, "/api/v1/tema/auth/register");

    char *content_type = malloc(CONT_TYPE_SIZE * sizeof(char));
    strcpy(content_type, "application/json");

    char **body_data = malloc(MAX_BODY_FIELDS * sizeof(char *));
    body_data[0] = malloc(MAX_FIELD * sizeof(char));

    /* create new JSON with username and password to complete the body of a further request */
    JSON_Value *new_val =  json_value_init_object();
    JSON_Object *new_obj = json_value_get_object(new_val);

    json_object_set_string(new_obj, "username", username);
    json_object_set_string(new_obj, "password", password);

    char *serialized_string = json_serialize_to_string(new_val);
    strcpy(body_data[0], serialized_string);

    char *msg = compute_post_request(ip, url, content_type,  body_data, 1, NULL, 0, NULL);

    send_to_server(*sockfd, msg);

    char *response = receive_from_server((*sockfd));
    char *status = get_response_status(response);

    if(strcmp(status, "Success") == 0) {
        printf("Register was successful. New user created!\n");
    } else if(strncmp(status, "Error", strlen("Error")) == 0) {
        printf("%s", status);
        free(status);
    } else {
        printf("Status unknown, response from sv: %s\n", response);
    }

    free(url);
    free(content_type);
    free(body_data[0]);
    free(body_data);
    free(msg);
    free(response);
    json_value_free(new_val);
    json_free_serialized_string(serialized_string);
}

char *login_to_sv(int *sockfd, char *ip, const char *username, const char *password)
{

    *sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);
    char *url = malloc(URL_SIZE * sizeof(char));
    strcpy(url, "/api/v1/tema/auth/login");

    char *content_type = malloc(CONT_TYPE_SIZE * sizeof(char));
    strcpy(content_type, "application/json");

    char **body_data = malloc(MAX_BODY_FIELDS * sizeof(char *));
    body_data[0] = malloc(MAX_FIELD * sizeof(char));
    /* create new JSON with username and password to complete the body of a further request */
    JSON_Value *new_val =  json_value_init_object();
    JSON_Object *new_obj = json_value_get_object(new_val);

    json_object_set_string(new_obj, "username", username);
    json_object_set_string(new_obj, "password", password);
    char *serialized_string = json_serialize_to_string(new_val);
    strcpy(body_data[0], serialized_string);

    char *msg = compute_post_request(ip, url, content_type,  body_data, 1, NULL, 0, NULL);
    send_to_server((*sockfd), msg);
    char *response = receive_from_server((*sockfd));
    char *status = get_response_status(response);
    char *connection_cookie = NULL;

    if(strcmp(status, "Success") == 0) {
        printf("Login was successful. You are now logged in!\nWelcome %s!\n", username);
        char *set_cookie = strstr(response, "Set-Cookie:");

        char *token = strtok(set_cookie, " ;");
        token = strtok(NULL, " ;");

        /* set connection cookie */
        connection_cookie = malloc(MAX_COOKIE_SIZE * sizeof(char));
        strcpy(connection_cookie, token);

    } else if(strncmp(status, "Error", strlen("Error")) == 0) {
        printf("%s", status);
        free(status);
    } else {
        printf("Status unknown, response from sv: %s\n", response);
    }

    free(msg);
    free(response);
    free(url);
    free(content_type);
    free(body_data[0]);
    free(body_data);
    json_value_free(new_val);
    json_free_serialized_string(serialized_string);

    return connection_cookie;
}

char *enter_library(int *sockfd, char *ip, char *connection_cookie)
{

    *sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);
    char *url = malloc(URL_SIZE * sizeof(char));
    strcpy(url, "/api/v1/tema/library/access");

    char **cookies = malloc(MAX_COOKIES * sizeof(char *));
    cookies[0] = malloc(MAX_COOKIE_SIZE * sizeof(char));

    int no_cookies = 0;
    if(connection_cookie != NULL) {
        strcpy(cookies[0], connection_cookie);
        no_cookies = 1;
    }
    
    char *msg = compute_get_request(ip, url, NULL, cookies, no_cookies, NULL);

    send_to_server(*sockfd, msg);
    

    char *response = receive_from_server(*sockfd);
    char *status = get_response_status(response);
    
    char *auth_token = NULL;
    
    if(strcmp(status, "Success") == 0) {
        printf("You have entered the library!\n");

        /* set authorization token */
        auth_token = get_token(response);
    } else if(strncmp(status, "Error", strlen("Error")) == 0){
        printf("%s", status);
        free(status);
    } else {
        printf("Status unknown, response from sv: %s\n", response);
    }

    free(msg);
    free(response);
    free(url);
    free(cookies[0]);
    free(cookies);
    return auth_token;
}

void get_books(int *sockfd, char *ip, char *auth_token)
{
    *sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

    char *url = malloc(URL_SIZE * sizeof(char));
    strcpy(url, "/api/v1/tema/library/books");

    char *msg = compute_get_request(ip, url, NULL, NULL, 0, auth_token);

    send_to_server(*sockfd, msg);

    char *response = receive_from_server(*sockfd);
    /* get list of books in JSON format from the response*/
    char *list = basic_extract_json_response(response);
    char *status = get_response_status(response);

    if(strcmp(status, "Success") == 0) {
        printf("Book list:\n");
        print_books_info(list);
    } else if(strncmp(status, "Error", strlen("Error")) == 0) {
        printf("%s", status);
        free(status);
    } else {
        printf("Status unknown, response from sv: %s\n", response);
    }

    free(url);
    free(msg);
    free(response); 
}

void get_book(int *sockfd, char *ip, char *auth_token, char *book_id)
{

    *sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

    char *url = malloc(URL_SIZE * sizeof(char));
    strcpy(url, "/api/v1/tema/library/books/");
    strcat(url, book_id);

    char *msg = compute_get_request(ip, url, NULL, NULL, 0, auth_token);
    
    send_to_server(*sockfd, msg);

    char *response = receive_from_server(*sockfd);

    char *status = get_response_status(response);
    
    if(strcmp(status, "Success") == 0) {
        printf("Book with id = %s\n", book_id);
        /* get the book in JSON format from the response */
        char *book = basic_extract_json_response(response);
        print_book_detailed(book);

    }
    else if(strncmp(status, "Error", strlen("Error")) == 0) {
        printf("%s", status);
        free(status);
    } else {
        printf("Status unknown, response from sv: %s\n", response);
    }

    free(response);
    free(url);
    free(msg);
}

void add_book(int *sockfd, char *ip, char *auth_token, book *new_book)
{

    *sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

    char *url = malloc(URL_SIZE * sizeof(char));
    strcpy(url, "/api/v1/tema/library/books");

    char *content_type = malloc(CONT_TYPE_SIZE * sizeof(char));
    strcpy(content_type, "application/json");

    /* create a JSON containing the new book */
    JSON_Value *new_val =  json_value_init_object();
    JSON_Object *new_obj = json_value_get_object(new_val);

    json_object_set_string(new_obj, "title", new_book->title);
    json_object_set_string(new_obj, "author", new_book->author);
    json_object_set_string(new_obj, "publisher", new_book->publisher);
    json_object_set_string(new_obj, "genre", new_book->genre);
    json_object_set_number(new_obj, "page_count", new_book->page_count);

    char **body_data = malloc(MAX_BODY_FIELDS * sizeof(char *));
    body_data[0] = malloc(MAX_FIELD * sizeof(char));

    char *serialized_string = json_serialize_to_string(new_val);
    strcpy(body_data[0], serialized_string);

    char *msg = compute_post_request(ip, url, content_type, body_data, 1, NULL, 0, auth_token);

    send_to_server(*sockfd, msg);

    char *response = receive_from_server(*sockfd);
    char *status = get_response_status(response);
    
    if(strcmp(status, "Success") == 0) {
        printf("Book added into the library\n");
    }
    else if(strncmp(status, "Error", strlen("Error")) == 0) {
        printf("%s", status);
        free(status);
    } else {
        printf("Status unknown, response from sv: %s\n", response);
    }

    free(url);
    free(content_type);
    free(body_data[0]);
    free(body_data);
    free(msg);
    free(response);
    json_value_free(new_val);
    json_free_serialized_string(serialized_string);
}

void delete_book(int *sockfd, char *ip, char *auth_token, char *book_id)
{
    *sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

    char *url = malloc(URL_SIZE * sizeof(char));
    strcpy(url, "/api/v1/tema/library/books/");
    strcat(url, book_id);

    char *msg = compute_delete_request(ip, url, NULL, 0, auth_token);

    send_to_server(*sockfd, msg);

    char *response = receive_from_server(*sockfd);
    char *status = get_response_status(response);

    if(strcmp(status, "Success") == 0) {
        printf("Book deleted successfully\n");
    }   else if(strncmp(status, "Error", strlen("Error")) == 0) {
        printf("%s", status);
        free(status);
    } else {
        printf("Status unknown, response from sv: %s\n", response);
    }

    free(url);
    free(msg);
    free(response);
}

void logout(int *sockfd, char *ip, char *connection_cookie)
{
    *sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

    char **cookies = malloc(MAX_COOKIES * sizeof(char *));
    cookies[0] = malloc(MAX_COOKIE_SIZE * sizeof(char));
    strcpy(cookies[0], connection_cookie);

    char *url = malloc(URL_SIZE * sizeof(char));
    strcpy(url, "/api/v1/tema/auth/logout");

    char *msg = compute_get_request(ip, url, NULL, cookies, 1, NULL);

    send_to_server(*sockfd, msg);

    char *response = receive_from_server(*sockfd);
    char *status = get_response_status(response);

    if(strcmp(status, "Success") == 0) {
        printf("You have logged out successfully\n");
    }
    else if(strncmp(status, "Error", strlen("Error")) == 0) {
        printf("%s", status);
    }
        
    free(url);
    free(cookies[0]);
    free(cookies);
    free(response);
    free(msg);
}

