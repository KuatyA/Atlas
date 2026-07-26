#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "lexical_analyzer.h"

//initialize the helper functions that will basically never leave this file.
bool is_valid_identifier_start(int ch);
bool is_valid_identifier_character(int ch);
int special_format_handler(const char *string_buf);

/*Ok so lets start with the main function. Its called the "lexical_parse". I want it to parse(obviously),
make up each of the strings and send them to the "generate_tokens" function. I want to avoid doing anything else 
in this function. No error handlings or logic to differentiate identifiers from other syntax just parsing and sending.*/


void parse_file(const char *filetype /*would 'filetype' be accurate? idk might change it later(probably never)*/){

}

/*lets continue with the error handler, the name speaks for itself. It will handle errors(shocking!). HJonestly,
i have NO idea how im gonna do it. It late at night and the possible errors are not coming to mind so the methods im
gonna use, i will explain(figure out lmao) them tomorrow.*/

int lex_error_handler(){

}

/*idk if returning int for the generate_token will work but i have and enum structure for the tokens table so maybe it will 
just be a little confusing at worst. IDEK if string_buf is the way to go tbh.*/


int generate_token(const char *string_buf){

}

/*Ok so a special function here. I couldn't figure out another way so i decided to split the token function to 2 and 1
identifier_handler function and call this as the default choice if the generate_token couldn't match the token with the 
table. It basically works like this: generate_token wont have the TOKEN_IDENTIFIER as a valid case so it will default to 
this function which will decide if the string it got is an identifier or unknown. God this is a long ahh comment.*/


void identifier_handler(const char *string_buf){

}

//the second generate_token function called:


int generate_identifier_token(const char *string_buf){

}

//helper functions

bool is_valid_identifier_start(int ch){

}
bool is_valid_identifier_character(int ch){

}

/*ok this one is inbetween but i think its closer to a helper function. So it will handle special formats like:
"1.5323e-2" this is a float literal but the format of it is basically:
"TOKEN_FLOAT_LITERAL + e + TOKEN_PLUS/MINUS + TOKEN_INT_LITERAL" and i dont want to dirty the base generator function
by handling special cases there.*/


int special_format_handler(const char *string_buf){

}

//wow i talk a lot