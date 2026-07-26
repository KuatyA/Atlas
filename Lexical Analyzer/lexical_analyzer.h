#ifndef LEXICAL_H
#define LEXICAL_H

//const char check_illegal_characters(const char *string_buf);
void return_token(const char *string_buf);
int parse_file(const char *filepath);
bool is_valid_char(int ch);
bool is_valid_identifier_start(int ch);

typedef enum {
    TOKEN_EOF,
    TOKEN_UNKNOWN,

    TOKEN_KW_INT,
    TOKEN_KW_SHORT,
    TOKEN_KW_LONG,
    TOKEN_KW_BYTE,
    TOKEN_KW_FLOAT,
    TOKEN_KW_DOUBLE,
    TOKEN_KW_CHAR,
    TOKEN_KW_BOOL,
    TOKEN_KW_STRUCT,
    TOKEN_KW_ENUM,

    TOKEN_INT_LITERAL,     
    TOKEN_FLOAT_LITERAL,   
    TOKEN_CHAR_LITERAL,   
    TOKEN_STRING_LITERAL,
    TOKEN_BOOL_LITERAL,

    TOKEN_KW_IF,
    TOKEN_KW_ELSE,
    TOKEN_KW_WHILE,
    TOKEN_KW_DO,
    TOKEN_KW_FOR,
    TOKEN_KW_SWITCH,
    TOKEN_KW_RETURN,

    TOKEN_PRINT,
    TOKEN_INPUT,

    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LE,
    TOKEN_GE,
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_QUESTION_CMP, //  ex: (var1 ? var2)
    TOKEN_ASSIGN,
    
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_MOD, // "%"

    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NAND,
    TOKEN_XOR,
    TOKEN_NOT,

    TOKEN_IDENTIFIER,

    TOKEN_AMPERSAND, // "&"
    TOKEN_ARROW,
    TOKEN_KW_ARENA,
    TOKEN_KW_DEFER,
    TOKEN_KW_RESET,
    TOKEN_KW_TEMP,

    TOKEN_CONST,
    TOKEN_MUT,
    TOKEN_STATIC,
    TOKEN_VOLATILE,

    TOKEN_KW_CAST

}TokenType;

#endif