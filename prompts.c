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
#include "prompts.h"
#include <errno.h>

void invalid_command()
{
    printf("Invalid command\nHelp:\n register - register a new account\n login - enter your credentials if you are already registered");
    printf("\n enter_library - get access to library content\n add_book - add a new book in your library\n get_books - get a short info about all books");
    printf("\n get_book - get detailed info about a specific book\n delete_book - remove a book from your library\n logout - exit your account\n exit - close app\n");
}

void register_prompt(char *username, char *password)
{
    printf("username=");
    fgets(username, MAX_USERNAME, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("password=");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = 0;
}

void login_prompt(int is_logged, char *username, char *password)
{
    if(is_logged == 0) {
        printf("username=");
        fgets(username, MAX_USERNAME, stdin);
        username[strcspn(username, "\n")] = 0;

        printf("password=");
        fgets(password, MAX_PASSWORD, stdin);
        password[strcspn(password, "\n")] = 0;
    } 
}

void get_book_prompt(char *book_id)
{
    printf("id=");

    fgets(book_id, MAX_DIGITS_ID, stdin);
    book_id[strcspn(book_id, "\n")] = 0;
}

void add_book_prompt(book *new_book)
{
    printf("title=");
    new_book->title = malloc(MAX_TITLE * sizeof(char));
    fgets(new_book->title, MAX_TITLE, stdin);
    new_book->title[strcspn(new_book->title, "\n")] = '\0';
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


    char *page_count_str = malloc(MAX_PAGE_CNT_DIGITS * sizeof(char));

    /* check if user has entered a number */
    int is_number = 0;
    while(is_number == 0) {
        printf("page_count=");
        fgets(page_count_str, MAX_PAGE_CNT_DIGITS, stdin);
        page_count_str[strcspn(page_count_str, "\n")] = '\0';
        new_book->page_count = atoi(page_count_str);
        if(new_book->page_count <= 0) {
            printf("Invalid number of pages. It must be an integer. Try again.\n");
        } else {
            is_number = 1;
        }
    }

    free(page_count_str);
    
}

void delete_book_prompt(char *book_id) {
    printf("id=");
    fgets(book_id, MAX_GENRE, stdin);
    book_id[strcspn(book_id, "\n")] = '\0';
}