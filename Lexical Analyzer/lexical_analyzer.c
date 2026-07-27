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
TokenType lookup_token(const char *string);

/*Ok so lets start with the main function. Its called the "parse_file". I want it to parse(obviously),
make up each of the strings and send them to the "generate_tokens" function. I want to avoid doing anything else 
in this function. No error handlings or logic to differentiate identifiers from other syntax just parsing and sending.*/


void parse_file(const char *filetype /*would 'filetype' be accurate? idk might change it later(probably never)*/){

    //open the files
    FILE *atl_file = fopen(filetype, "rb");
    //check empty
    if(atl_file == NULL){
        fprintf(stderr, "ERROR: Could not open file!");
        exit(1);
    }
    //initialize string buf and index.
    char string_buf[256];
    int buf_idx = 0;

    //get file size
    fseek(atl_file, 0, SEEK_END);
    long file_size = ftell(atl_file);
    fseek(atl_file, 0 , SEEK_SET);

    char *src_buf  = malloc(file_size + 1);
    if (!file_size) { fclose(atl_file); exit(1); }

    size_t bytes_read = fread(src_buf, 1, file_size, atl_file);
     if (ferror(atl_file)) {
        free(src_buf);
        fclose(atl_file);
        exit(1);
    }
    src_buf[bytes_read] = '\0';
    fclose(atl_file);

    const char *src = src_buf;

    while((*src != '\0')){
        unsigned char ch = *src;
       uint8_t type = char_table[ch];

       if(type & CHAR_WS){
            if(buf_idx >0){
                string_buf[buf_idx] = '\0';
                generate_token(string_buf);
                buf_idx = 0;
            }
        src++;
        continue;
       }
       if(type & CHAR_OP){
            if(buf_idx > 0){
                string_buf[buf_idx] = '\0';
                generate_token(string_buf);
                buf_idx = 0;
            }
            if (ch == '<' && src[1] == '<' && src[2] == '=') {
                src += 3; generate_token("<<="); continue;
            }
            if (ch == '>' && src[1] == '>' && src[2] == '=') {
                src += 3; generate_token(">>="); continue;
            }

            // 2-character operators
            if (ch == ':' && src[1] == ':') { src += 2; generate_token("::"); continue; }
            if (ch == '-' && src[1] == '>') { src += 2; generate_token("->"); continue; }
            if (ch == '=' && src[1] == '=') { src += 2; generate_token("=="); continue; }
            if (ch == '=' && src[1] == '>') { src += 2; generate_token("=>"); continue; }
            if (ch == '<' && src[1] == '-') { src += 2; generate_token("<-"); continue; }
            if (ch == '+' && src[1] == '=') { src += 2; generate_token("+="); continue; }
            if (ch == '-' && src[1] == '=') { src += 2; generate_token("-="); continue; }
            if (ch == '*' && src[1] == '=') { src += 2; generate_token("*="); continue; }
            if (ch == '/' && src[1] == '=') { src += 2; generate_token("/="); continue; }
            if (ch == '%' && src[1] == '=') { src += 2; generate_token("%="); continue; }
            if (ch == '<' && src[1] == '=') { src += 2; generate_token("<="); continue; }
            if (ch == '>' && src[1] == '=') { src += 2; generate_token(">="); continue; }
            if (ch == '!' && src[1] == '=') { src += 2; generate_token("!="); continue; }
            if (ch == '&' && src[1] == '&') { src += 2; generate_token("&&"); continue; }
            if (ch == '|' && src[1] == '|') { src += 2; generate_token("||"); continue; }
            if (ch == '!' && src[1] == '&') { src += 2; generate_token("!&"); continue; }
            if (ch == '<' && src[1] == '<') { src += 2; generate_token("<<"); continue; }
            if (ch == '>' && src[1] == '>') { src += 2; generate_token(">>"); continue; }

            string_buf[0] = (char)ch;
            string_buf[1] = '\0';
            generate_token(string_buf);
            src++;
            continue;
       }

       if(type & CHAR_DELIM){
            if(buf_idx > 0){
                string_buf[buf_idx] = '\0';
                generate_token(string_buf);
                buf_idx = 0;
            }
            string_buf[0] = (char)ch;
            string_buf[1] = '\0';
            generate_token(string_buf);
            src++;
            continue;
       }

       string_buf[buf_idx++] = (char)ch;
       src++;
    }
    //get the last string of the code.
    if(buf_idx > 0){
        string_buf[buf_idx] = '\0';
        generate_token(string_buf);
    }

    generate_token("EOF");
    free(src_buf);
}

/*lets continue with the error handler, the name speaks for itself. It will handle errors(shocking!). The main idea is that
when a string gets flagged as a TOKEN_UNKNOWN, i will check what error it has and print an error message. HOWEVER, the handler 
will not stop the program immediately.Instead, it will have a counter system. It will wait until it receives the TOKEN_EOF
and if the counter is bigger than 0, it will exit with 1.*/

int lex_error_handler(){

}

/*idk if returning int for the generate_token will work but i have and enum structure for the tokens table so maybe it will 
just be a little confusing at worst. IDEK if string_buf is the way to go tbh.*/


void generate_token(const char *string_buf){
    
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
TokenType lookup_token(const char *string){
    uint32_t index = hash_string(string) % HASH_TABLE_SIZE;

    while (keyword_table[index].key != NULL) {
        if (strcmp(keyword_table[index].key, string) == 0) {
            return keyword_table[index].token; 
        }
        index = (index + 1) % HASH_TABLE_SIZE;
    }

    return TOKEN_UNKNOWN;
}

/*ok this one is inbetween but i think its closer to a helper function. So it will handle special formats like:
"1.5323e-2" this is a float literal but the format of it is basically:
"TOKEN_FLOAT_LITERAL + e + TOKEN_PLUS/MINUS + TOKEN__LITERAL" and i dont want to dirty the base generator function
by handling special cases there.*/


int special_format_handler(const char *string_buf){

}

//hashing functions to convert strings to integers for assigning tokens from the enum table.
static uint32_t hash_string(const char *string){
    uint32_t hash = 2166136261u;
    while (*string) {
        hash ^= (uint8_t)*string++;
        hash *= 16777619u;
    }
    return hash;
}
static void insert_keyword(const char *key, TokenType token) {
    uint32_t index = hash_string(key) % HASH_TABLE_SIZE;

    //resolve collisions.
    while (keyword_table[index].key != NULL) {
        index = (index + 1) % HASH_TABLE_SIZE;
    }

    keyword_table[index].key = key;
    keyword_table[index].token = token;
}
void initialize_hash_table(void){

    memset(keyword_table, 0, sizeof(keyword_table));

    //types
    insert_keyword("int", TOKEN_KW_INT);
    insert_keyword("short", TOKEN_KW_SHORT);
    insert_keyword("long", TOKEN_KW_LONG);
    insert_keyword("byte", TOKEN_KW_BYTE);
    insert_keyword("float", TOKEN_KW_FLOAT);
    insert_keyword("double", TOKEN_KW_DOUBLE);
    insert_keyword("char", TOKEN_KW_CHAR);
    insert_keyword("string", TOKEN_KW_STRING);
    insert_keyword("bool", TOKEN_KW_BOOL);
    insert_keyword("struct", TOKEN_KW_STRUCT);
    insert_keyword("enum", TOKEN_KW_ENUM);
    insert_keyword("union", TOKEN_KW_UNION);

    //statements and functions
    insert_keyword("if", TOKEN_KW_IF);
    insert_keyword("else", TOKEN_KW_ELSE);
    insert_keyword("while", TOKEN_KW_WHILE);
    insert_keyword("do", TOKEN_KW_DO);
    insert_keyword("for", TOKEN_KW_FOR);
    insert_keyword("switch", TOKEN_KW_SWITCH);
    insert_keyword("return", TOKEN_KW_RETURN);
    insert_keyword("out", TOKEN_KW_OUTPUT);
    insert_keyword("in", TOKEN_KW_INPUT);
    insert_keyword("read_file", TOKEN_KW_READ_FILE);
    insert_keyword("write_file", TOKEN_KW_WRITE_TO_FILE);
    insert_keyword("break", TOKEN_KW_BREAK);
    insert_keyword("continue", TOKEN_KW_CONTINUE);
    insert_keyword("match", TOKEN_KW_MATCH);
    insert_keyword("func", TOKEN_KW_FUNCTION);

    //memory thingys
    insert_keyword("NULL", TOKEN_KW_NULL);
    insert_keyword("arena", TOKEN_KW_ARENA);
    insert_keyword("defer", TOKEN_KW_DEFER);
    insert_keyword("reset", TOKEN_KW_RESET);
    insert_keyword("temp", TOKEN_KW_TEMP);
    insert_keyword("const", TOKEN_KW_CONST);
    insert_keyword("mutable", TOKEN_KW_MUT);
    insert_keyword("static", TOKEN_KW_STATIC);
    insert_keyword("volatile", TOKEN_KW_VOLATILE);
    insert_keyword("cast", TOKEN_KW_CAST);
    insert_keyword("typealias", TOKEN_KW_TYPEALIAS);

    //modules, visibility, import
    insert_keyword("import", TOKEN_KW_IMPORT);
    insert_keyword("module", TOKEN_KW_MODULE);
    insert_keyword("public", TOKEN_KW_PUBLIC);
    insert_keyword("private", TOKEN_KW_PRIVATE);

    //error handling
    insert_keyword("try", TOKEN_KW_TRY);
    insert_keyword("catch", TOKEN_KW_CATCH);
    insert_keyword("raise", TOKEN_KW_RAISE);

    //function and multithreading thingys
    insert_keyword("async", TOKEN_KW_ASYNC);
    insert_keyword("await", TOKEN_KW_AWAIT);
    insert_keyword("spawn", TOKEN_KW_SPAWN);
    insert_keyword("thread", TOKEN_KW_THREAD);
    insert_keyword("enable_thread", TOKEN_KW_ENABLE_T);
    insert_keyword("join", TOKEN_KW_JOIN);
    insert_keyword("channel", TOKEN_KW_CHAN);
    insert_keyword("select", TOKEN_KW_SELECT);
    insert_keyword("atomic", TOKEN_KW_ATOMIC);
    insert_keyword("mutex", TOKEN_KW_MUTEX);
    insert_keyword("lock", TOKEN_KW_LOCK);
    insert_keyword("shared", TOKEN_KW_SHARED);

}
void initialize_char_table(void){
    memset(char_table, 0, sizeof(char_table));

    char_table[' '] = char_table['\t'] = char_table['\n']
  = char_table['\r'] = CHAR_WS;

  for(int i = 'a'; i <= 'z'; i++){ char_table[i] |= CHAR_ALPHA; }
  for(int i = 'A'; i <= 'Z'; i++){ char_table[i] |= CHAR_ALPHA; }
  for(int i = '0'; i <= '9'; i++){ char_table[i] |= CHAR_DIGIT; }
  char_table['_'] |= CHAR_ALPHA;

  char_table['('] = char_table[']'] = CHAR_DELIM;
  char_table['{'] = char_table['}'] = CHAR_DELIM;
  char_table['['] = char_table[')'] = CHAR_DELIM;
  char_table[','] = char_table[';'] = CHAR_DELIM;

  char_table['+'] = char_table['-'] = CHAR_OP;
  char_table['*'] = char_table['/'] = CHAR_OP;
  char_table[':'] = char_table['='] = CHAR_OP;
  char_table['<'] = char_table['>'] = CHAR_OP;
  char_table['!'] = char_table['&'] = CHAR_OP;
  char_table['%'] = char_table['|'] = CHAR_OP;


}

//wow i talk a lot