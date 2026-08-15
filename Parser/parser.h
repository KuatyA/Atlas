#ifndef PARSER_H
#define PARSER_H

#include "../Lexical Analyzer/lex_structs.h"
#include "parse_structs.h"
#include "parser_tables.h"

/*The parser is gonna be an LALR parser with a matrix table*/
//fetch the token streams
void fetch_tokens(TokenStream *stream);
ASTNode *make_terminal_ast(TokenStruct *current_token);

#endif