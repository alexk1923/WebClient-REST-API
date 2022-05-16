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

typedef struct book{
    char *title;
    char *author;
    char *genre;
    char *publisher;
    int page_count;
} book;


void check_connection(char *ip, int *sockfd)
{
    
    write(*sockfd, "a", 1);

    char buff_test[3];
    int r = read(*sockfd, buff_test, 1);

    int port = 8080;

    if(r == 0) {
        *sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);
    }
}


void register_to_sv(int sockfd, char *ip, const char *username, const char *password) {
    char *url = malloc(100 * sizeof(char));
    strcpy(url, "/api/v1/tema/auth/register");

    char *content_type = malloc(60 * sizeof(char));
    strcpy(content_type, "application/json");

    char **body_data = malloc(MAX_BODY_FIELDS * sizeof(char *));
    body_data[0] = malloc(MAX_FIELD * sizeof(char));
    // printf("{\n\"username\":%s,\n\"password\":%s\n}", username, password);
    sprintf(body_data[0], "{\n\t\"username\":\"%s\",\n\t\"password\":\"%s\"\n}", username, password);

    // strcpy(body_data, "{\"username\": \"");
    // strcat(body_data, username);
    // strcat("\"password\": \"")
    // strcat(body_data, password);
    // strcat("}");
    // printf("Message JSON:\n%s\n", body_data[0]);

    char *msg = compute_post_request(ip, url, content_type,  body_data, 1, NULL, 0, NULL);
    printf("Message:\n%s\n", msg);

    send_to_server(sockfd, msg);
    printf("Am primit de la server: %s\n", receive_from_server(sockfd));
}


char *login_to_sv(int sockfd, char *ip, const char *username, const char *password)
{

    char *url = malloc(100 * sizeof(char));
    strcpy(url, "/api/v1/tema/auth/login");

    char *content_type = malloc(60 * sizeof(char));
    strcpy(content_type, "application/json");

    char **body_data = malloc(MAX_BODY_FIELDS * sizeof(char *));
    body_data[0] = malloc(MAX_FIELD * sizeof(char));
    sprintf(body_data[0], "{\n\t\"username\":\"%s\",\n\t\"password\":\"%s\"\n}", username, password);

    char *msg = compute_post_request(ip, url, content_type,  body_data, 1, NULL, 0, NULL);
    printf("Message:\n%s\n", msg);

    send_to_server(sockfd, msg);

    free(url);
    free(content_type);
    free(body_data[0]);
    free(body_data);

    char *response = receive_from_server(sockfd);
    printf("Am primit de la server: \n%s", response);

    char *set_cookie = strstr(response, "Set-Cookie:");

    char *token = strtok(set_cookie, " ;");
    token = strtok(NULL, " ;");

    char *connection_cookie = malloc(MAX_COOKIE_SIZE * sizeof(char));
    strcpy(connection_cookie, token);

    // connection_cookie[strlen(token)] = '\0';


    return connection_cookie;
}


char * get_token(char *response) 
{
    char *needle = malloc(20 * sizeof(char));
    strcpy(needle, "\"token\":\"");
    char *tkn = strstr(response, needle) + strlen(needle);

    char *p = strchr(tkn, '}');
    *(p - 1) = '\0';

    free(needle);
    return tkn;
}

char *enter_library(int sockfd, char *ip, char *connection_cookie)
{
    char *url = malloc(100 * sizeof(char));
    strcpy(url, "/api/v1/tema/library/access");

    char **cookies = malloc(MAX_COOKIES * sizeof(char *));
    cookies[0] = malloc(MAX_COOKIE_SIZE * sizeof(char));
    strcpy(cookies[0], connection_cookie);


    int no_cookies = 1;

    char *msg = compute_get_request(ip, url, NULL, cookies, no_cookies, NULL);


    printf("Enter library Message:\n%s", msg);
    send_to_server(sockfd, msg);

    free(url);
    free(cookies[0]);
    free(cookies);

    char *response = receive_from_server(sockfd);

    printf("Am primit de la server: %s\n", response);
    if(response) {
        return get_token(response);
    }
    return NULL;
}

void get_books(int *sockfd, char *ip, char *auth_token)
{
    char *url = malloc(100 * sizeof(char));
    strcpy(url, "/api/v1/tema/library/books");

    check_connection(ip, sockfd);


    char *msg = compute_get_request(ip, url, NULL, NULL, 0, auth_token);
    printf("Message:\n%s\n", msg);

    send_to_server(*sockfd, msg);

    free(url);

    char *response = receive_from_server(*sockfd);

    printf("Am primit de la server: %s\n", response);

    char *list = strstr(response, "[{");


    JSON_Value *value = json_parse_string(list);
    // JSON_Object *new_obj = json_value_get_object(value);

    printf("JSON SERIALIZE:%s", json_serialize_to_string_pretty(value));
    json_value_free(value);
}

void get_book(int *sockfd, char *ip, char *auth_token, char *book_id)
{
    char *url = malloc(100 * sizeof(char));
    strcpy(url, "/api/v1/tema/library/books/");
    strcat(url, book_id);

    check_connection(ip, sockfd);
    // char **cookies = malloc(MAX_COOKIES * sizeof(char *));
    // cookies[0] = malloc(MAX_COOKIE_SIZE * sizeof(char));
    // strcpy(cookies[0], connection_cookie);

    char *msg = compute_get_request(ip, url, NULL, NULL, 0, auth_token);
    printf("Message:\n%s\n", msg);

    free(url);

    send_to_server(*sockfd, msg);

    char *response = receive_from_server(*sockfd);

    printf("Am primit de la server: %s\n", response);
}



void add_book(int *sockfd, char *ip, char *auth_token, book *new_book)
{

    char *url = malloc(100 * sizeof(char));
    strcpy(url, "/api/v1/tema/library/books");

    char *content_type = malloc(60 * sizeof(char));
    strcpy(content_type, "application/json");

    JSON_Value *new_val =  json_value_init_object();
    JSON_Object *new_obj = json_value_get_object(new_val);

    json_object_set_string(new_obj, "title", new_book->title);
    json_object_set_string(new_obj, "author", new_book->author);
    json_object_set_string(new_obj, "publisher", new_book->publisher);
    json_object_set_string(new_obj, "genre", new_book->genre);

    json_object_set_number(new_obj, "page_count", new_book->page_count);
    
    printf("%s", json_serialize_to_string_pretty(new_val));

    char **body_data = malloc(MAX_BODY_FIELDS * sizeof(char *));
    body_data[0] = malloc(MAX_FIELD * sizeof(char));
    // printf("{\n\"username\":%s,\n\"password\":%s\n}", username, password);
    // strcpy(body_data[0], json_serialize_to_string(new_val));
    strcpy(body_data[0], json_serialize_to_string(new_val));

    // compute_post_request(ip, url, content_type, body_data, 1, NULL, 0, NULL);

    char *msg = compute_post_request(ip, url, content_type, body_data, 1, NULL, 0, auth_token);
    printf("Message:\n%s", msg);
    send_to_server(*sockfd, msg);

    char *response = receive_from_server(*sockfd);

    printf("Am primit de la server: %s\n", response);

    free(url);
    free(content_type);
    free(body_data[0]);
    free(body_data);
    json_value_free(new_val);
}

void delete_book(int sockfd, char *ip, char *auth_token, char *book_id)
{
    char *url = malloc(100 * sizeof(char));
    strcpy(url, "/api/v1/tema/library/books/");
    strcat(url, book_id);

    check_connection(ip, &sockfd);
    // char **cookies = malloc(MAX_COOKIES * sizeof(char *));
    // cookies[0] = malloc(MAX_COOKIE_SIZE * sizeof(char));
    // strcpy(cookies[0], connection_cookie);

    char *msg = compute_delete_request(ip, url, NULL, 0, auth_token);
    printf("Message:\n%s\n", msg);

    send_to_server(sockfd, msg);

    char *response = receive_from_server(sockfd);

    printf("Am primit de la server: %s\n", response);
}

void logout(int sockfd, char *ip, char *connection_cookie)
{
    char **cookies = malloc(MAX_COOKIES * sizeof(char *));
    cookies[0] = malloc(MAX_COOKIE_SIZE * sizeof(char));
    strcpy(cookies[0], connection_cookie);

    char *url = malloc(100 * sizeof(char));
    strcpy(url, "/api/v1/tema/auth/logout");

    char *msg = compute_get_request(ip, url, NULL, cookies, 1, NULL);
    printf("Message:\n%s\n", msg);

    send_to_server(sockfd, msg);

    char *response = receive_from_server(sockfd);
    printf("Am primit de la server: \n%s", response);

    free(url);
    free(cookies[0]);
    free(cookies);
}

void free_book(book *curr_book)
{
    free(curr_book->title);
    free(curr_book->author);
    free(curr_book->genre);
    free(curr_book->publisher);
    free(curr_book);
}



int main(int argc, char *argv[])
{
    char *message;
    char *response;
    char *connection_cookie;
    char *auth_token;
    int sockfd, sockfd_GET, sockfd_POST;

    setvbuf(stdout, NULL, _IONBF, BUFSIZ);   
    // Ex 1.1: GET dummy from main server
    char *ip = malloc(100 * sizeof(char));
    strcpy(ip, "34.241.4.235");

    int port = 8080;
    // sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd = open_connection(ip, port, AF_INET, SOCK_STREAM, 0);


    char *command = malloc(MAX_COMMAND * sizeof(char));
    char *username = malloc(MAX_USERNAME * sizeof(char));
    char *password = malloc(MAX_PASSWORD * sizeof(char));

    while(1) {
        fgets(command, MAX_COMMAND, stdin);
        command[strcspn(command, "\n")] = 0;

        if(strcmp(command, "register") == 0) {
            printf("username=");
            fgets(username, MAX_USERNAME, stdin);
            username[strcspn(username, "\n")] = 0;

            printf("password=");
            fgets(password, MAX_PASSWORD, stdin);
            password[strcspn(password, "\n")] = 0;
            register_to_sv(sockfd, ip, username, password);
        } else if(strcmp(command, "login") == 0) {
            printf("username=");
            fgets(username, MAX_USERNAME, stdin);
            username[strcspn(username, "\n")] = 0;

            printf("password=");
            fgets(password, MAX_PASSWORD, stdin);
            password[strcspn(password, "\n")] = 0;
            connection_cookie = login_to_sv(sockfd, ip, username, password);
            // printf("strlen connection:%ld\n", strlen(connection_cookie));
        } else if(strcmp(command, "enter_library") == 0) {
            auth_token = enter_library(sockfd, ip, connection_cookie);
        } else if(strcmp(command, "get_books") == 0) {
            get_books(&sockfd, ip, auth_token);
        } else if(strcmp(command, "get_book") == 0) {
            printf("id=");
            char *book_id = malloc(MAX_DIGITS_ID * sizeof(char));

            fgets(book_id, MAX_DIGITS_ID, stdin);
            book_id[strcspn(book_id, "\n")] = 0;
            
            get_book(&sockfd, ip, auth_token, book_id);
        } else if(strcmp(command, "add_book") == 0) {
            printf("title=");
            book *new_book = malloc(sizeof(book));
            new_book->title = malloc(MAX_TITLE * sizeof(char));
            fgets(new_book->title, MAX_TITLE, stdin);
            new_book->title[strcspn(new_book->title, "\n")] = '\0';
            // printf("title:%s", new_book->title);

            printf("author=");
            new_book->author = malloc(MAX_AUTHOR * sizeof(char));
            fgets(new_book->author, MAX_AUTHOR, stdin);
            new_book->author[strcspn(new_book->author, "\n")] = '\0';

            printf("genre=");
            new_book->genre = malloc(MAX_GENRE * sizeof(char));
            fgets(new_book->genre, MAX_GENRE, stdin);
            new_book->genre[strcspn(new_book->genre, "\n")] = '\0';

            printf("publisher=");
            new_book->publisher = malloc(MAX_PUBLISHER * sizeof(char));
            fgets(new_book->publisher, MAX_GENRE, stdin);
            new_book->publisher[strcspn(new_book->publisher, "\n")] = '\0';

            printf("page_count=");
            scanf("%d", &new_book->page_count);
            add_book(&sockfd, ip, auth_token, new_book);
            free_book(new_book);
        } else if(strcmp(command, "delete_book") == 0) {
            char *book_id = malloc(MAX_DIGITS_ID * sizeof(char));
            printf("id=");
            fgets(book_id, MAX_GENRE, stdin);
            book_id[strcspn(book_id, "\n")] = '\0';
            delete_book(sockfd, ip, auth_token, book_id);
        } else if(strcmp(command, "logout") == 0) {
            logout(sockfd, ip, connection_cookie);
        } else if(strcmp(command, "exit") == 0) {
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
            
            return 0;
        }
    }


    
    char **cookies = malloc(10 * sizeof(char *));
    cookies[0] = malloc(100 * sizeof(char));
    strcpy(cookies[0], "c_user=XXXXXXXXXX");

    cookies[1] = malloc(100 * sizeof(char));
    strcpy(cookies[1], "presence=XXXXXXX");

    char *url = malloc(100 * sizeof(char));
    strcpy(url, "/api/v1/dummy");

    char *msg = malloc(1000 * sizeof(char));
    strcpy(msg, compute_get_request(ip,  url, NULL, cookies, 2, NULL));

    printf("Messsage: %s", compute_get_request(ip,  "/api/v1/dummy", NULL, cookies, 2, NULL));
    send_to_server(sockfd, msg);

    printf("Am primit de la server: %s\n", receive_from_server(sockfd));
    // Ex 1.2: POST dummy and print response from main server
    char *content_type = malloc(60 * sizeof(char));
    strcpy(content_type, "application/x-www-form-urlencoded");

    char **body_data = malloc(5 * sizeof(char *));
    body_data[0] = malloc(50 * sizeof(char));
    strcpy(body_data[0], "field1=value1");

    body_data[1] = malloc(50 * sizeof(char));
    strcpy(body_data[1], "field2=value2");
    // printf("%s", compute_post_request(ip, url, content_type,  body_data, 2, NULL, 0));

    strcpy(msg, compute_post_request(ip, url, content_type,  body_data, 2, NULL, 0, NULL));
    printf("%s", msg);

    send_to_server(sockfd, msg);
    printf("Am primit de la server POST:\n %s", receive_from_server(sockfd));


    // Ex 2: Login into main server

    strcpy(url, "/api/v1/auth/login");
    strcpy(body_data[0], "username=student");
    strcpy(body_data[1], "password=student");

    strcpy(msg, compute_post_request(ip, url, content_type,  body_data, 2, NULL, 0, NULL));
    printf("Msg de POST pt login:\n%s", msg);

    send_to_server(sockfd, msg);
    response = receive_from_server(sockfd);
    printf("Am primit de la server POST LOGIN:\n %s", response);

    // char *set_cookie = strstr(response, "Set-Cookie: ");
    // char *token = strtok(set_cookie, " ;");
    // token = strtok(NULL, " ;");

    // printf("Token:%s\n", token);
    // Ex 3: GET weather key from main server
    strcpy(url, "/api/v1/weather/key");
    strcpy(cookies[0], "connect.sid=s%3Ab5MLOiW4XrybIYR3hie4pL6-hlygX8yQ.Vj27QWdCZoPK9HgTrqUdef3XeoEaFXUyx9w%2BOkMQBQg");
    // cookies[0][strlen(token)] = '\0';
    // strcpy(cookies[1], "Path=/");
    // cookies[2] = malloc(100 * sizeof(char));
    // strcpy(cookies[2], "HttpOnly");


    
    strcpy(msg, compute_get_request(ip, url, NULL, cookies, 1, NULL));
    printf("Messsage: %s", msg);
    send_to_server(sockfd, msg);
    

    response = receive_from_server(sockfd);
    printf("Am primit de la server: %s\n", response);
    
    // Ex 4: GET weather data from OpenWeather API
    strcpy(url, "/data/2.5/weather");
    strcpy(ip, "https://api.openweathermap.org/");
    char **querry_params = malloc(10 * sizeof(char *));
    querry_params[0] = malloc(100 * sizeof(char));
    querry_params[1] = malloc(100 * sizeof(char));
    querry_params[2] = malloc(100 * sizeof(char));
    strcpy(querry_params[0], "lat=45.15");
    strcpy(querry_params[1], "lon=26.83");
    strcpy(querry_params[2], "lat=45.15&lon=26.83&appid=b912dd495585fbf756dc6d8f415a7649");

    sockfd = open_connection("37.139.20.5", 80, AF_INET, SOCK_STREAM, 0);
    msg = compute_get_request("37.139.20.5", url, querry_params[2], NULL, 0, NULL);
    printf("Voi trimite mesajul:\n%s\n", msg);

    send_to_server(sockfd, msg);
    response = receive_from_server(sockfd);
    printf("Am primit de la server: \n%s\n", response);

    char *decoded_resp = basic_extract_json_response(response);
    printf("Decoded reply:\n%s\n", decoded_resp);
    // Ex 5: POST weather data for verification to main server
    // Ex 6: Logout from main server
    sockfd = open_connection("34.241.4.235", port, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("34.241.4.235", "/api/v1/auth/logout", NULL, NULL, 0, NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("raspuns logout\n%s\n", response);
    // close_connection()
    // BONUS: make the main server return "Already logged in!"

    // free the allocated data at the end!

    return 0;
}
