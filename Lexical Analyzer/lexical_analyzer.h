#ifndef LEXICAL_H
#define LEXICAL_H

//initialize the functions except the helpers here.
void parse_file(const char *filetype);
int lex_error_handler();
int generate_token(const char *string_buf);
void identifier_handler(const char *string_buf);
int generate_identifier_token(const char *string_buf);

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
    TOKEN_KW_BOOL, //bool keyword
    TOKEN_KW_STRUCT, //struct keyword
    TOKEN_KW_ENUM, //enum keyword
    TOKEN_KW_UNION, //union keyword

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
    TOKEN_KW_SWITCH, //switch statement block
    TOKEN_KW_RETURN, //return statement block
    TOKEN_KW_PRINT, //print to terminal
    TOKEN_KW_INPUT, //get input from terminal
    TOKEN_KW_READ_FILE, //read from file
    TOKEN_KW_WRITE_TO_FILE, //write to file
    TOKEN_KW_BREAK, //break a block of code from executing further
    TOKEN_KW_CONTINUE, //skip that specific iteration
    TOKEN_KW_MATCH, //to match the structure of data

    TOKEN_KW_NULL, //null pointer
    TOKEN_KW_ARENA, //alternative to heap memory, an area of data with a bump pointer
    TOKEN_KW_DEFER, //defer / procrastinate an arenas destruction
    TOKEN_KW_RESET, //destroy arena
    TOKEN_KW_TEMP, //a temporary scratch arena
    TOKEN_KW_CONST, //make constant variable types
    TOKEN_KW_MUT, //make mutable variable types
    TOKEN_KW_STATIC, //make static var,able types
    TOKEN_KW_VOLATILE, //prevent compiler modifications in harware mem registers
    TOKEN_KW_CAST, //convert types to eachother ex: char to int
    TOKEN_KW_TYPEALIAS, //custom type names

    TOKEN_KW_IMPORT, //import libraries
    TOKEN_KW_MODULE, //declare current files namespace
    TOKEN_KW_VIS, //default: private

    TOKEN_KW_TRY, //try/propogate an error
    TOKEN_KW_CATCH, //handle the caught error
    TOKEN_KW_RAISE, //throw / emit error

    TOKEN_KW_ASYNC, // async declaeration
    TOKEN_KW_AWAIT, //wait async result
    TOKEN_KW_SPAWN, //spin up worker thread
    TOKEN_KW_THREAD, //assign which thread the program/block will run on
    TOKEN_KW_ENABLE_T, //decide how many threads the program will use
    TOKEN_KW_JOIN, //wait thread termination
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
    TOKEN_NAND, // "!&"
    TOKEN_XOR, // "^"
    TOKEN_NOT, // "!"
    TOKEN_BIT_NOT, // "~"     
    TOKEN_LSHIFT, // "<<"
    TOKEN_RSHIFT, // ">>"
    TOKEN_LSHIFT_EQ, // "<<="   
    TOKEN_RSHIFT_EQ, //">>="
    TOKEN_QUESTION, // "?"
    TOKEN_AMPERSAND, // "&"
    

    TOKEN_ARROW, // "->"
    TOKEN_FAT_ARROW, // "=>" used in match arms ex: 5 => return
    TOKEN_LEFT_ARROW, //use to send or receive on a channel "<-"
    
}TokenType;

#endif