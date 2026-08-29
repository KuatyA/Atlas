#ifndef PARSER_H
#define PARSER_H

#include "../Lexical Analyzer/lex_structs.h"
#include "parse_structs.h"
#include "parser_tables.h"

/*The parser is gonna be an LALR parser with a matrix table*/
//fetch the token streams
ASTNode *fetch_tokens(TokenStream *stream);
ASTNode *make_ast(int rule_id, ASTNode **opped_nodes);
ASTNode *make_terminal_ast(TokenStruct *tokens);
TokenStruct *get_current_token(TokenStream *stream);

static TokenStruct EOF_TOKEN = {
    .token = TOKEN_EOF,
    .lexeme = "EOF",
    .length = 3,
    .line = 1,
    .column = 1,
};

#endif