#include "commands.h"

#ifndef _PROMPTS_
#define _PROMPTS_

/* show a list of commands to help the user */
void invalid_command();

/* display prompt for username and password (registration) */
void register_prompt(char *username, char *password);

/* display prompt for username and password (login) */
void login_prompt(int is_logged, char *username, char *password);

/* display prompt for searched book's id */
void get_book_prompt(char *book_id);

/* display prompt for all fields of a book (id is assigned automatically) */
void add_book_prompt(book *new_book);

/* display prompt for the id of the book which is going to be removed */
void delete_book_prompt(char *book_id);

#endif