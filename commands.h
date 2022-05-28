#ifndef _COMMANDS_
#define _COMMANDS_

typedef struct book{
    char *title;
    char *author;
    char *genre;
    char *publisher;
    int page_count;
} book;

/* print books short info based on a JSON array given as a string */
void print_books_info(char *json_book_list);

/* get detailed info about a book from the JSON given as a string */
void print_book_detailed(char *book);

/* get status (Success/Error) based on server response */
char *get_response_status(char *response);

/* get authorization token from if enter_library request was successful */
char *get_token(char *response);

/* send a request of registration to the server with given username and password*/
void register_to_sv(int *sockfd, char *ip, const char *username, const char *password);

/* send a request of login to the server with given username and password and return connection cookie 
 * if successful
 */
char *login_to_sv(int *sockfd, char *ip, const char *username, const char *password);

/* send a request for accessing library data and return authorization token if successful */
char *enter_library(int *sockfd, char *ip, char *connection_cookie);

/* get all books short info from the library */
void get_books(int *sockfd, char *ip, char *auth_token);

/* get detailed info about a book from the library */
void get_book(int *sockfd, char *ip, char *auth_token, char *book_id);

/* add new book into the library*/
void add_book(int *sockfd, char *ip, char *auth_token, book *new_book);

/* remove a book from the library */
void delete_book(int *sockfd, char *ip, char *auth_token, char *book_id);

/* end current connection session and exit your account */
void logout(int *sockfd, char *ip, char *connection_cookie);

/* free all fields of a book */
void free_book(book *curr_book);


#endif
