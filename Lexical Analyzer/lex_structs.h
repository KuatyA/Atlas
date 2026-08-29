#ifndef LEX_TABLES_H
#define LEX_TABLES_H

#include <stdint.h>
#include <pthread.h>

typedef enum Tokens{
    TOKEN_EOF, //end of file
    TOKEN_UNKNOWN, //unknown string / character
    TOKEN_IDENTIFIER, //identifiers ex: function names, variable names

    TOKEN_KW_INT, //int keyword
    TOKEN_KW_SHORT, //short keyword
    TOKEN_KW_LONG, //long keyword
    TOKEN_KW_BYTE, //byte keyword
    TOKEN_KW_FLOAT, //float keyword
    TOKEN_KW_DOUBLE, //double keyword
    TOKEN_KW_CHAR, //char keyword
    TOKEN_KW_STRING,
    TOKEN_KW_BOOL, //bool keyword
    TOKEN_KW_STRUCT, //struct keyword
    TOKEN_KW_ENUM, //enum keyword
    TOKEN_KW_UNION, //union keyword
    TOKEN_KW_VOID,

    TOKEN_INT_LITERAL, //int literal ex: 1, 45, 8779     
    TOKEN_FLOAT_LITERAL, //float literal ex: 43.63, 12e-5
    TOKEN_CHAR_LITERAL, //char literal ex: a, b, c  
    TOKEN_STRING_LITERAL, //string literal ex: "wassup", "my G"
    TOKEN_BOOL_LITERAL, //bool literal ex: true, false

    TOKEN_KW_IF, //if(potential man) statement block
    TOKEN_KW_ELSE, //else statement block
    TOKEN_KW_WHILE, //while statement block
    TOKEN_KW_DO, //do statement block
    TOKEN_KW_FOR, //for statement block
    TOKEN_KW_SWITCH,
    TOKEN_KW_CASE, //switch statement block
    TOKEN_KW_DEFAULT,
    TOKEN_KW_RETURN, //return statement block
    TOKEN_KW_BREAK, //break a block of code from executing further
    TOKEN_KW_CONTINUE, //skip that specific iteration
    TOKEN_KW_MATCH, //to match the structure of data
    TOKEN_KW_FUNCTION, //function keyword

    TOKEN_KW_NULL, //null pointer
    TOKEN_KW_DEFER, //defer / procrastinate an arenas destruction
    TOKEN_KW_TEMP, //a temporary scratch arena
    TOKEN_KW_CONST, //make constant variable types
    TOKEN_KW_MUT, //make mutable variable types
    TOKEN_KW_STATIC, //make static var,able types
    TOKEN_KW_VOLATILE, //prevent compiler modifications in harware mem registers
    TOKEN_KW_CAST, //convert types to eachother ex: char to int
    TOKEN_KW_TYPEALIAS, //custom type names

    TOKEN_KW_IMPORT, //import libraries
    TOKEN_KW_MODULE, //declare current files namespace
    TOKEN_KW_PRIVATE,
    TOKEN_KW_PUBLIC,

    TOKEN_KW_TRY, //try/propogate an error
    TOKEN_KW_CATCH, //handle the caught error
    TOKEN_KW_RAISE, //throw / emit error

    TOKEN_KW_ASYNC, // async declaeration
    TOKEN_KW_INLINE,
    TOKEN_KW_AWAIT, //wait async result
    TOKEN_KW_SPAWN, //spin up worker thread
    TOKEN_KW_CHAN, //declare channel type
    TOKEN_KW_SELECT, //multiplex channel operations
    TOKEN_KW_ATOMIC, //edgelord ahh token / atomic memory operations SON...
    TOKEN_KW_MUTEX, //exclusion lock
    TOKEN_KW_LOCK, //safely aquire mutex or atomic locks and release them at critical parts of the code
    TOKEN_KW_SHARED, //cross arena shared data specifier

    TOKEN_LBRACE, // "{"
    TOKEN_RBRACE, // "}"
    TOKEN_LBRACKET, // "["
    TOKEN_RBRACKET, // "]"
    TOKEN_LPAREN, // "("
    TOKEN_RPAREN, // ")"
    TOKEN_DOT, // "."
    TOKEN_COMMA, // ","
    TOKEN_COLON, // ":"
    TOKEN_SEMICOLON, // ";"
    TOKEN_SCOPE_RES, // "::"
    TOKEN_QUOTATION, // ' " '
    TOKEN_HASH, // "#"
    TOKEN_BACKSLASH, // "\"

    TOKEN_ASSIGN, // "="
    TOKEN_PLUS, // "+"
    TOKEN_MINUS, // "-"
    TOKEN_STAR, // "*"
    TOKEN_SLASH, // "/"
    TOKEN_MOD, // "%"
    TOKEN_PLUS_EQ, // "+="         
    TOKEN_MINUS_EQ, // "-="
    TOKEN_STAR_EQ, // "*="
    TOKEN_SLASH_EQ, // "/="
    TOKEN_MOD_EQ, // "%="
    
    TOKEN_LT, // "<"
    TOKEN_GT, // ">"
    TOKEN_LE, // "<="
    TOKEN_GE, // ">="
    TOKEN_EQ, // "=="
    TOKEN_NEQ, // "!="
    
    TOKEN_AND, // "&&"
    TOKEN_OR, // "||"
    TOKEN_BIT_OR, // "|"
    TOKEN_NAND, // "!&"
    TOKEN_XOR, // "^"
    TOKEN_NOT, // "!"
    TOKEN_BIT_NOT, // "~"     
    TOKEN_LSHIFT, // "<<"
    TOKEN_RSHIFT, // ">>"
    TOKEN_LSHIFT_ASSIGN, // "<<="   
    TOKEN_RSHIFT_ASSIGN, //">>="
    TOKEN_QUESTION, // "?"
    TOKEN_AMPERSAND, // "&"
    

    TOKEN_ARROW, // "->"
    TOKEN_FAT_ARROW, // "=>" used in match arms ex: 5 => return
    TOKEN_LEFT_ARROW, // use to send or receive on a channel "<-"
    
    TOKEN_COMMENT
}TokenType;

//mapping characters to a bitmask array
typedef enum Character{
    CHAR_WS = (1<<0), //whitespace
    CHAR_ALPHA = (1<<1), //alphanumeric
    CHAR_DIGIT = (1<<2), //digits
    CHAR_DELIM = (1<<3), //delimiters
    CHAR_OP = (1<<4) //operations
}CharacterClass;

typedef struct Keyword{
    const char *key;
    TokenType token;
    uint32_t len;
}KeywordEntry;

//struct for file queue
typedef struct {
    char **files;
    int total_files;
    int next_file_index;
    pthread_mutex_t lock;
}FileQueue;

//structure of a single token
typedef struct{
  TokenType token;
  const char *lexeme;
  uint32_t length;
  uint32_t line;
  uint32_t column;
}TokenStruct;

// struct that will hold the tokens in RAM.
typedef struct{
    TokenStruct *tokens;
    size_t capacity;
    size_t count;
    size_t read_idx;
}TokenStream;

typedef struct {
    const char *cursor;
    uint32_t line;
    uint32_t col;
    uint32_t error_count;
} LexerContext;

#endif