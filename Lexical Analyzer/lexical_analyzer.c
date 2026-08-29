#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lexical_analyzer.h"
#include <stdatomic.h>
#include <threads.h>

//initialize the helper functions that will basically never leave this file.
int special_format_handler(const char *start, uint32_t *out_len);
TokenType lookup_token(const char *string, uint32_t len);
TokenStream create_token_stream(size_t initial_capacity);
void append_token(TokenStream *stream, TokenStruct token);

static thread_local uint32_t lex_error_count = 0;

/*Ok so lets start with the main function. Its called the "parse_file". I want it to parse(obviously),
make up each of the strings and send them to the "generate_tokens" function. I want to avoid doing anything else 
in this function. No error handlings or logic to differentiate identifiers from other syntax just parsing and sending.*/
void lexer(const char *filename /*would 'filetype' be accurate? idk might change it later(probably never)*/){
    lex_error_count = 0;
    LexerContext ctx = {0};
    //open the files
    FILE *atl_file = fopen(filename, "rb");
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
    if (!src_buf) { fclose(atl_file); return; }

    size_t bytes_read = fread(src_buf, 1, file_size, atl_file);
    src_buf[bytes_read] = '\0';
    if (ferror(atl_file)) {
        free(src_buf);
        fclose(atl_file);
        return;
    }
    fclose(atl_file);

    const char *src = src_buf;
    uint32_t line = 1, col = 1;

    while((*src != '\0')){

       if(char_table[(unsigned char)*src] & CHAR_WS){
            if(*src == '\n'){
                line++;
                col = 1;
            }else{
                col++;
            }
        src++;
        continue;
       }
       TokenStruct tok = generate_token(&src, &line, &col);

       if (tok.token == TOKEN_COMMENT) continue;
       if(tok.token == TOKEN_UNKNOWN){
            lex_error_handler(&ctx, tok);
       }

        append_token(&stream, tok);
    }
    //generate eof token.
    TokenStruct eof_tok = { .token = TOKEN_EOF, .lexeme = src, .length = 0, .line = line, .column = col };
    append_token(&stream, eof_tok); 

    int status = lex_error_handler(&ctx, eof_tok);
    

    if (status != 0) {
        free(stream.tokens);
        free(src_buf);
        return; 
    }
    stream.read_idx = 0;

    if (stream.count == 0) {
        fprintf(stderr, "[!] Error: File '%s' is empty or could not be read.\n", filename);
        free(stream.tokens);
        return;
    }
    
    //free memory(for now).
    ASTNode *ast = fetch_tokens(&stream);
    free(src_buf);
}

/*lets continue with the error handler, the name speaks for itself. It will handle errors(shocking!). The main idea is that
when a string gets flagged as a TOKEN_UNKNOWN, i will check what error it has and print an error message. HOWEVER, the handler 
will not stop the program immediately.Instead, it will have a counter system. It will wait until it receives the TOKEN_EOF
and if the counter is bigger than 0, it will exit with 1.*/
int lex_error_handler(LexerContext *ctx,TokenStruct tok){
    ctx->error_count++;
    if (tok.token == TOKEN_UNKNOWN) {
        lex_error_count++;
        fprintf(stderr, "Lexical Error [%u:%u]: Unrecognized character '%.*s' (0x%02X)\n", 
                tok.line, tok.column, tok.length, tok.lexeme, (unsigned char)*tok.lexeme);
        return 0;
    }

    if (tok.token == TOKEN_EOF) {
        if (lex_error_count > 0) {
            fprintf(stderr, "Lexing failed with %u errors.\n", lex_error_count);
            return 1; // Signal failure to the caller
        }
        return 0; // Clean exit
    }

    return 0;
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
                    tok.lexeme = strndup(start, 1);
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '}':
                    tok.token = TOKEN_RBRACE;
                    tok.length = 1;
                    tok.lexeme = strndup(start, 1);
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '[':
                    tok.token = TOKEN_LBRACKET;
                    tok.length = 1;
                    tok.lexeme = strndup(start, 1);
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case ']':
                    tok.token = TOKEN_RBRACKET;
                    tok.length = 1;
                    tok.lexeme = strndup(start, 1);
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '(':
                    tok.token = TOKEN_LPAREN;
                    tok.length = 1;
                    tok.lexeme = strndup(start, 1);
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case ')':
                    tok.token = TOKEN_RPAREN;
                    tok.length = 1;
                    tok.lexeme = strndup(start, 1);
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '.':
                    tok.token = TOKEN_DOT;
                    tok.length = 1;
                    tok.lexeme = strndup(start, 1);
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case ',':
                    tok.token = TOKEN_COMMA;
                    tok.length = 1;
                    tok.lexeme = strndup(start, 1);
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case ':':
                    if(start[1] == ':'){
                        tok.token = TOKEN_SCOPE_RES;
                        tok.length = 2;
                        tok.lexeme = strndup(start, 2);
                        *cursor += 2;
                        *col += 2;
                        return tok;
                        }
                        tok.token = TOKEN_COLON;
                        tok.length = 1;
                        tok.lexeme = strndup(start, 1);
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case ';':
                    tok.token = TOKEN_SEMICOLON;
                    tok.length = 1;
                    tok.lexeme = strndup(start, 1);
                    (*cursor)++;
                    (*col)++;
                    return tok;
                break;
                case '"':{
                   const char *quote_start = *cursor; // Save exact start position
                    (*cursor)++;
                    (*col)++;
                    const char *start_str = *cursor;

                    while (**cursor != '"' && **cursor != '\0' && **cursor != '\n') {
                        (*cursor)++;
                        (*col)++;
                    }

                    if (**cursor == '"') {
                        tok.token = TOKEN_STRING_LITERAL;
                        tok.lexeme = start_str;
                        tok.length = (uint32_t)(*cursor - start_str);
                        (*cursor)++;
                        (*col)++;
                    } else {
                        tok.token = TOKEN_UNKNOWN;
                        tok.lexeme = quote_start; // Safe pointer
                        tok.length = (uint32_t)(*cursor - quote_start);
                    }
                    return tok;
                }
                break;
                case '#':
                    tok.token = TOKEN_HASH;
                    tok.length = 1;
                    tok.lexeme = strndup(start, 1);
                    (*cursor)++;
                    (*col)++;
                    return tok;
                case '=':
                    if(start[1] == '='){
                        tok.token = TOKEN_EQ;
                        tok.length = 2;
                        tok.lexeme = strndup(start, 2);
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }else if(start[1] == '>'){
                        tok.token = TOKEN_FAT_ARROW;
                        tok.length = 2;
                        tok.lexeme = strndup(start, 2);
                        *cursor += 2;
                        *col += 2;
                        return tok;
                    }
                        tok.token = TOKEN_ASSIGN;
                        tok.length = 1;
                        tok.lexeme = strndup(start, 1);
                        (*cursor)++;
                        (*col)++;
                        return tok;
                break;
                case '+':
                    if(start[1] == '='){
                        tok.token = TOKEN_PLUS_EQ;
                        tok.length = 2;
                        tok.lexeme = strndup(start, 2);
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
                    if (start[1] == '=') {
                    tok.token = TOKEN_SLASH_EQ;
                    tok.length = 2;
                    *cursor += 2;
                    *col += 2;
                    return tok;
                } else if (start[1] == '/') {
                    while (**cursor != '\n' && **cursor != '\0') {
                        (*cursor)++;
                        (*col)++;
                    }
                    tok.token = TOKEN_COMMENT; 
                    tok.length = (uint32_t)(*cursor - start);
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
                case '\'':;
                    const char *char_start = *cursor;
                    (*cursor)++;
                    (*col)++;
                    while (**cursor != '\'' && **cursor != '\n' && **cursor != '\0') {
                        if (**cursor == '\\' && (*cursor)[1] != '\0') {
                            (*cursor) += 2;
                            (*col) += 2;
                        } else {
                            (*cursor)++;
                            (*col)++;
                        }
                    }
                    if (**cursor == '\'') {
                        (*cursor)++;
                        (*col)++;
                        tok.token = TOKEN_CHAR_LITERAL;
                        tok.lexeme = char_start;
                        tok.length = (size_t)(*cursor - char_start);
                    } else {
                        tok.token = TOKEN_UNKNOWN;
                        tok.lexeme = char_start;
                        tok.length = 1;
                    }
                    return tok;
                break;
                default:
                    if(char_table[(unsigned char)c] & CHAR_ALPHA){
                        const char *p = start;
                        while(char_table[(unsigned char)*p] & (CHAR_ALPHA | CHAR_DIGIT)){
                            p++;
                        }
                        uint32_t len = (uint32_t)(p - start);
                        TokenType kw = lookup_token(start, len);
                        tok.token = (kw != TOKEN_UNKNOWN) ? kw : TOKEN_IDENTIFIER;
                        tok.length = len;
                        tok.lexeme = strndup(start, len);
                        *cursor += len;
                        *col += len;
                        return tok;
                    }
                    if(char_table[(unsigned char)c] & CHAR_DIGIT){
                        const char *p = start;
                        while(char_table[(unsigned char)*p] & CHAR_DIGIT){
                            p++;
                        }
                        if(*p == '.' || *p == 'e' || *p == 'E'){
                            uint32_t len = 0;
                            tok.token = special_format_handler(start, &len);
                            tok.length = len;
                            tok.lexeme = strndup(start, len);
                            *cursor += len;
                            *col += len;
                            return tok;
                        }
                        uint32_t len = (uint32_t)(p - start);
                        tok.token = TOKEN_INT_LITERAL;
                        tok.length = len;
                        tok.lexeme = strndup(start, len);

                        *cursor += len;
                        *col += len;
                        return tok;
                    }
                    unsigned char uc = (unsigned char)(**cursor);
                    uint32_t char_len = 1;

                    if ((uc & 0xE0) == 0xC0)      char_len = 2;
                    else if ((uc & 0xF0) == 0xE0) char_len = 3; 
                    else if ((uc & 0xF8) == 0xF0) char_len = 4; 

                    for (uint32_t i = 0; i < char_len; i++) {
                        if ((*cursor)[i] == '\0') {
                            char_len = i > 0 ? i : 1;
                            break;
                        }
                    }

                        tok.token = TOKEN_UNKNOWN;
                        tok.lexeme = *cursor;
                        tok.length = char_len;
                    
                        *cursor += char_len;
                        *col += 1;
                        return tok;
                break;

            }
}

//helper functions
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
    uint32_t idx = hash_string(string, len) % HASH_TABLE_SIZE;
    uint32_t start_idx = idx;

    while (keyword_table[idx].key != NULL) {
        if (keyword_table[idx].len == len && memcmp(keyword_table[idx].key, string, len) == 0) {
            return keyword_table[idx].token;
        }
        idx = (idx + 1) % HASH_TABLE_SIZE;
        if(idx == start_idx) break;
    }

    return TOKEN_UNKNOWN;
}

/*ok this one is inbetween but i think its closer to a helper function. So it will handle special formats like:
"1.5323e-2" this is a float literal but the format of it is basically:
"TOKEN_FLOAT_LITERAL + e + TOKEN_PLUS/MINUS + TOKEN__LITERAL" and i dont want to dirty the base generator function
by handling special cases there.*/
int special_format_handler(const char *start, uint32_t *out_len){
    const char *p = start;

    while (*p != '\0' && (char_table[(unsigned char)*p] & CHAR_DIGIT)) {
        p++;
    }
    if (*p == '.') {
        p++;
        while (*p != '\0' && (char_table[(unsigned char)*p] & CHAR_DIGIT)) {
            p++;
        }
    }
    if (*p == 'e' || *p == 'E') {
        const char *e_start = p;
        p++;
        if (*p == '+' || *p == '-') {
            p++;
        }
        if (*p != '\0' && (char_table[(unsigned char)*p] & CHAR_DIGIT)) {
            while (*p != '\0' && (char_table[(unsigned char)*p] & CHAR_DIGIT)) {
                p++;
            }
        } else {
            p = e_start;
        }
    }
    *out_len = (uint32_t)(p - start);
    return TOKEN_FLOAT_LITERAL;
}

//hashing functions to convert strings to integers for assigning tokens from the enum table.
static uint32_t hash_string(const char *string, uint32_t len){
    uint32_t hash = 2166136261u;
    for (uint32_t i = 0; i < len; i++) {
        hash ^= (uint8_t)string[i];
        hash *= 16777619u;
    }
    return hash;
}
static void insert_keyword(const char *key, TokenType token, uint32_t len) {
    uint32_t idx = hash_string(key, len) % HASH_TABLE_SIZE;

    //resolve collisions.
    while (keyword_table[idx].key != NULL) {
        idx = (idx + 1) % HASH_TABLE_SIZE;
    }

    keyword_table[idx].key = key;
    keyword_table[idx].token = token;
    keyword_table[idx].len = len;
}
void initialize_hash_table(void){

    memset(keyword_table, 0, sizeof(keyword_table));

    //types
    insert_keyword("int", TOKEN_KW_INT, 3);
    insert_keyword("short", TOKEN_KW_SHORT, 5);
    insert_keyword("long", TOKEN_KW_LONG, 4);
    insert_keyword("byte", TOKEN_KW_BYTE, 4);
    insert_keyword("float", TOKEN_KW_FLOAT, 5);
    insert_keyword("double", TOKEN_KW_DOUBLE, 6);
    insert_keyword("char", TOKEN_KW_CHAR, 4);
    insert_keyword("string", TOKEN_KW_STRING, 6);
    insert_keyword("bool", TOKEN_KW_BOOL, 4);
    insert_keyword("struct", TOKEN_KW_STRUCT, 6);
    insert_keyword("enum", TOKEN_KW_ENUM, 4);
    insert_keyword("union", TOKEN_KW_UNION, 5);
    insert_keyword("void", TOKEN_KW_VOID, 4);

    //statements and functions
    insert_keyword("if", TOKEN_KW_IF, 2);
    insert_keyword("else", TOKEN_KW_ELSE, 4);
    insert_keyword("while", TOKEN_KW_WHILE, 5);
    insert_keyword("do", TOKEN_KW_DO, 2);
    insert_keyword("for", TOKEN_KW_FOR, 3);
    insert_keyword("switch", TOKEN_KW_SWITCH, 6);
    insert_keyword("case", TOKEN_KW_CASE, 4);
    insert_keyword("default", TOKEN_KW_DEFAULT, 7);
    insert_keyword("return", TOKEN_KW_RETURN, 6);
    insert_keyword("break", TOKEN_KW_BREAK, 5);
    insert_keyword("continue", TOKEN_KW_CONTINUE, 8);
    insert_keyword("match", TOKEN_KW_MATCH, 5);
    insert_keyword("func", TOKEN_KW_FUNCTION, 4);

    //memory thingys
    insert_keyword("NULL", TOKEN_KW_NULL, 4);
    insert_keyword("defer", TOKEN_KW_DEFER, 5);
    insert_keyword("temp", TOKEN_KW_TEMP, 4);
    insert_keyword("const", TOKEN_KW_CONST, 5);
    insert_keyword("mutable", TOKEN_KW_MUT, 7);
    insert_keyword("static", TOKEN_KW_STATIC, 6);
    insert_keyword("volatile", TOKEN_KW_VOLATILE, 8);
    insert_keyword("cast", TOKEN_KW_CAST, 4);
    insert_keyword("typealias", TOKEN_KW_TYPEALIAS, 9);

    //modules, visibility, import
    insert_keyword("import", TOKEN_KW_IMPORT, 6);
    insert_keyword("module", TOKEN_KW_MODULE, 6);
    insert_keyword("public", TOKEN_KW_PUBLIC, 6);
    insert_keyword("private", TOKEN_KW_PRIVATE, 7);

    //error handling
    insert_keyword("try", TOKEN_KW_TRY, 3);
    insert_keyword("catch", TOKEN_KW_CATCH, 5);
    insert_keyword("raise", TOKEN_KW_RAISE, 5);

    //function and multithreading thingys
    insert_keyword("async", TOKEN_KW_ASYNC, 5);
    insert_keyword("inline", TOKEN_KW_INLINE, 6);
    insert_keyword("await", TOKEN_KW_AWAIT, 5);
    insert_keyword("spawn", TOKEN_KW_SPAWN, 5);
    insert_keyword("channel", TOKEN_KW_CHAN, 7);
    insert_keyword("select", TOKEN_KW_SELECT, 6);
    insert_keyword("atomic", TOKEN_KW_ATOMIC, 6);
    insert_keyword("mutex", TOKEN_KW_MUTEX, 5);
    insert_keyword("lock", TOKEN_KW_LOCK, 4);
    insert_keyword("shared", TOKEN_KW_SHARED, 6);

    //boolean literals
    insert_keyword("true", TOKEN_BOOL_LITERAL, 4);
    insert_keyword("false", TOKEN_BOOL_LITERAL, 5);

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
  char_table['['] = char_table[']'] = CHAR_DELIM;
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

        lexer(filename);
        
    }
    return NULL;
}
//wow i talk a lot