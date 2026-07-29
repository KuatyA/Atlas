#ifndef LEXICAL_H
#define LEXICAL_H
#define HASH_TABLE_SIZE 256
#define NUM_THREADS 4

#include <string.h>
#include <stdlib.h>
#include "structs.h"

extern FileQueue queue;
extern TokenStruct token_struct;

//initialize the functions except the helpers here.
void parse_file(const char *filetype);
int lex_error_handler(TokenType kw);
TokenStruct generate_token(const char **cursor, uint32_t *line, uint32_t *col);
void identifier_handler(const char *string_buf);
int generate_identifier_token(const char *string_buf);

//initialize hashing functions here.
static uint32_t hash_string(const char *string);
static void insert_keyword(const char *key, TokenType token);
void initialize_hash_table(void);
void initialize_char_table(void);

//multithreading for reading multiple files
void *worker_thread(void *arg);

static uint8_t char_table[256];
static KeywordEntry keyword_table[HASH_TABLE_SIZE];

#endif