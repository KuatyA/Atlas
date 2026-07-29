#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "lexical_analyzer.h"
#include <stdatomic.h>

//initialize the helper functions that will basically never leave this file.
bool is_valid_identifier_start(int ch);
bool is_valid_identifier_character(int ch);
int special_format_handler(const char *string_buf);
TokenType lookup_token(const char *string, uint32_t len);
TokenStream create_token_stream(size_t initial_capacity);
void append_token(TokenStream *stream, TokenStruct token);

/*Ok so lets start with the main function. Its called the "parse_file". I want it to parse(obviously),
make up each of the strings and send them to the "generate_tokens" function. I want to avoid doing anything else 
in this function. No error handlings or logic to differentiate identifiers from other syntax just parsing and sending.*/
void parse_file(const char *filetype /*would 'filetype' be accurate? idk might change it later(probably never)*/){

    //open the files
    FILE *atl_file = fopen(filetype, "rb");
    //check empty
    if(atl_file == NULL){
        fprintf(stderr, "ERROR: Could not open file!");
        return;
    }

    //initialize token stream for the current file being parsed.
    TokenStream stream = create_token_stream(1024);

    //get file size
    fseek(atl_file, 0, SEEK_END);
    long file_size = ftell(atl_file);
    fseek(atl_file, 0 , SEEK_SET);

    char *src_buf  = malloc(file_size + 1);
    if (!file_size) { fclose(atl_file); return; }

    size_t bytes_read = fread(src_buf, 1, file_size, atl_file);
     if (ferror(atl_file)) {
        free(src_buf);
        fclose(atl_file);
        return;
    }
    src_buf[bytes_read] = '\0';
    fclose(atl_file);

    const char *src = src_buf;
    uint32_t line = 1, col = 1;

    while((*src != '\0')){

       if(char_table[(unsigned char)*src] & CHAR_WS){
            if(src == '\n'){
                line++;
                col = 1;
            }else{
                col++;
            }
        src++;
        continue;
       }
       TokenStruct tok = generate_token(&src, &line, &col);
        append_token(&stream, tok);
    }
    //generate eof token.
    TokenStruct eof_tok = { .token = TOKEN_EOF, .lexeme = src, .length = 0, .line = line, .column = col };
    append_token(&stream, eof_tok); 

    //free memory(for now).
    free(stream.tokens);
    free(src_buf);
}

/*lets continue with the error handler, the name speaks for itself. It will handle errors(shocking!). The main idea is that
when a string gets flagged as a TOKEN_UNKNOWN, i will check what error it has and print an error message. HOWEVER, the handler 
will not stop the program immediately.Instead, it will have a counter system. It will wait until it receives the TOKEN_EOF
and if the counter is bigger than 0, it will exit with 1.*/
int lex_error_handler(TokenType kw){

}

/*idk if returning int for the generate_token will work but i have and enum structure for the tokens table so maybe it will 
just be a little confusing at worst. IDEK if string_buf is the way to go tbh.(update, string_buf was NOT the way to go)*/
TokenStruct generate_token(const char **cursor, uint32_t *line, uint32_t *col){
        TokenStruct tok;
        const char *start = *cursor;
        char c = *start;

        tok.lexeme = start;
        tok.line = *line;
        tok.column = *col;
        // for the single charcter tokens
        
            switch(c){
                case '{':
                    tok.token = TOKEN_LBRACE;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '}':
                    tok.token = TOKEN_RBRACE;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '[':
                    tok.token = TOKEN_LBRACKET;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case ']':
                    tok.token = TOKEN_RBRACKET;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '(':
                    tok.token = TOKEN_LPAREN;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case ')':
                    tok.token = TOKEN_RPAREN;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '.':
                    tok.token = TOKEN_DOT;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case ',':
                    tok.token = TOKEN_COMMA;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case ':':
                    if(start[1] == ':'){
                        tok.token = TOKEN_SCOPE_RES;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                        }
                        tok.token = TOKEN_COLON;
                        tok.length = 1;
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case ';':
                    tok.token = TOKEN_SEMICOLON;
                    tok.length = 1;
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '=':
                    if(start[1] == '='){
                        tok.token = TOKEN_EQ;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }else if(start[1] == '>'){
                        tok.token = TOKEN_FAT_ARROW;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_ASSIGN;
                        tok.length = 1;
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '+':
                    if(start[1] == '='){
                        tok.token = TOKEN_PLUS_EQ;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_PLUS;
                        tok.length = 1;
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '-':
                    if(start[1] == '='){
                        tok.token = TOKEN_MINUS_EQ;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }else if(start[1] == '>'){
                        tok.token = TOKEN_ARROW;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_MINUS;
                        tok.length = 1;
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '*':
                    if(start[1] == '='){
                        tok.token = TOKEN_STAR_EQ;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_STAR;
                        tok.length = 1;        
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '/':
                    if(start[1] == '='){
                        tok.token = TOKEN_SLASH_EQ;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_SLASH;
                        tok.length = 1;
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '%':
                    if(start[1] == '='){
                        tok.token = TOKEN_MOD_EQ;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_MOD;
                        tok.length = 1;
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '<':
                    if(start[1] == '<' && start[2] == '='){
                        tok.token = TOKEN_LSHIFT_ASSIGN;
                        tok.length = 3;
                        *cursor += 3;
                        *col += 3;
                        return tok;
                    }else if(start[1] == '<'){
                        tok.token = TOKEN_LSHIFT;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }else if(start[1] == '='){
                        tok.token = TOKEN_LE;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }else if(start[1] == '-'){
                        tok.token = TOKEN_LEFT_ARROW;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_LT;
                        tok.length = 1;
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '>':
                    if(start[1] == '>' && start[2] == '='){
                        tok.token = TOKEN_RSHIFT_ASSIGN;
                        tok.length = 3;
                        *cursor += 3;
                        *col += 3;
                        return tok;
                    }else if(start[1] == '>'){
                        tok.token = TOKEN_RSHIFT;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }else if(start[1] == '='){
                        tok.token = TOKEN_GE;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_GT;
                        tok.length = 1;
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '^':
                    tok.token = TOKEN_XOR;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '!':
                    if(start[1] == '&'){
                        tok.token = TOKEN_NAND;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }else if(start[1] == '='){
                        tok.token = TOKEN_NEQ;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_NOT;
                        tok.length = 1;
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '~':
                    tok.token = TOKEN_BIT_NOT;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '?':
                    tok.token = TOKEN_QUESTION;
                    tok.length = 1;
                    
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '&':
                    if(start[1] == '&'){
                        tok.token = TOKEN_AND;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_AMPERSAND;
                        tok.length = 1;    
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '|':
                    if(start[1] == '|'){
                        tok.token = TOKEN_OR;
                        tok.length = 2;
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_BIT_OR;
                        tok.length = 1;    
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '\n':
                (*line)++;
                *col = 1;
                (*cursor)++;
                break;
                default:
                    if(char_table[(unsigned char)c] & CHAR_ALPHA){
                        const char *p = start;
                        while(char_table[(unsigned char)*p] & (CHAR_ALPHA | CHAR_DELIM)){
                            p++;
                        }
                        uint32_t len = (uint32_t)(p - start);
                        TokenType kw = lookup_token(start, len);
                        tok.token = (kw != TOKEN_UNKNOWN) ? kw : TOKEN_IDENTIFIER;
                        tok.length = len;

                        *cursor += len;
                        *col += len;
                        return tok;
                    }
                    if(char_table[(unsigned char)c] & CHAR_DELIM){
                        const char *p = start;
                        while(char_table[(unsigned char)*p] & CHAR_DELIM){
                            p++;
                        }
                        uint32_t len = (uint32_t)(p - start);
                        TokenType kw = lookup_token(start, len);
                        tok.token = (kw != TOKEN_UNKNOWN) ? kw : TOKEN_INT_LITERAL;
                        tok.length = len;

                        *cursor += len;
                        *col += len;
                        return tok;
                    }

                    tok.token = TOKEN_UNKNOWN;
                    tok.length = 1;
                    (*cursor)++;
                    (*col)++;
                    
                break;

            }
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
TokenStream create_token_stream(size_t initial_capacity){
    TokenStream stream;
    stream.capacity = initial_capacity;
    stream.count = 0;
    stream.tokens = malloc(sizeof(TokenStruct) * initial_capacity);
    return stream;
}
void append_token(TokenStream *stream, TokenStruct token) {
    if (stream->count >= stream->capacity) {
        stream->capacity *= 2;
        stream->tokens = realloc(stream->tokens, sizeof(TokenStruct) * stream->capacity);
    }
    stream->tokens[stream->count++] = token;
}
TokenType lookup_token(const char *string, uint32_t len){
    uint32_t idx = hash_string(string) % HASH_TABLE_SIZE;

    while (keyword_table[idx].key != NULL) {
        if (strcmp(keyword_table[idx].key, string) == 0) {
            return keyword_table[idx].token; 
        }
        idx = (idx + 1) % HASH_TABLE_SIZE;
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
    uint32_t idx = hash_string(key) % HASH_TABLE_SIZE;

    //resolve collisions.
    while (keyword_table[idx].key != NULL) {
        idx = (idx + 1) % HASH_TABLE_SIZE;
    }

    keyword_table[idx].key = key;
    keyword_table[idx].token = token;
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
    insert_keyword("challoc", TOKEN_KW_CHALLOC);
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

    //special insert to enforce a "main"
    insert_keyword("main", TOKEN_MAIN);

}
void initialize_char_table(void){
    memset(char_table, 0, sizeof(char_table));

    char_table[' '] = char_table['\t'] = char_table['\n']
  = char_table['\r'] = CHAR_WS;

  for(int i = 'a'; i <= 'z'; i++){ char_table[i] |= CHAR_ALPHA; }
  for(int i = 'A'; i <= 'Z'; i++){ char_table[i] |= CHAR_ALPHA; }
  for(int i = '0'; i <= '9'; i++){ char_table[i] |= CHAR_DIGIT; }
  char_table['_'] |= CHAR_ALPHA;

  char_table['('] = char_table[')'] = CHAR_DELIM;
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

/*the multithreading function to parse multiple files at the same time, i dont know how im gonna do the tokens maybe
put them in seperate token files and th parser can read them just like the lexical analyzer and combine them at the end of
the compilers life like the LLVM part maybe idk*/
void *worker_thread(void *arg){

    int thread_id = *(int *)arg;
    int token_struct_id = *(int *)arg;

    while(1){
        char *filename = NULL;
        
        pthread_mutex_lock(&queue.lock);
        if(queue.next_file_index < queue.total_files){
            filename = queue.files[queue.next_file_index];
            queue.next_file_index++;
        }
        pthread_mutex_unlock(&queue.lock);

        if(filename == NULL) break;

        parse_file(filename);
        
    }
    return NULL;
}
//wow i talk a lot