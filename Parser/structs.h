#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include "../AST/ast.h"

#define SYMBOL_TO_INDEX(sym) ((sym) >= NT_PROGRAM ? (TOKEN_COMMENT + 1 + ((sym) - NT_PROGRAM)) : (sym))

typedef enum{
    NT_PROGRAM  = 1000,

    NT_EXPRESSION,
    NT_EXPRESSION_STATEMENT,
    NT_EXPRESSION_LIST,

    NT_ARG_LIST,
    NT_ARG_LIST_NONEMPTY,
    
    NT_STATEMENT,
    NT_STATEMENT_LIST,

    NT_DECLARATION,
    NT_DECLARATION_LIST,
    NT_VAR_DECLARATION,
    
    NT_TYPE,
    NT_FACTOR,
    
    NT_FUNC_SUFFIX,
    NT_FUNC_BODY,
    NT_FUNC_DECLARATION,

    NT_PARAM,
    NT_PARAM_LIST,
    NT_PARAM_LIST_NONEMPTY,

    NT_BLOCK,

    NT_ARRAY_INIT,

    NT_IF_STATEMENT,
    NT_WHILE_STATEMENT,
    NT_FOR_STATEMENT,
    NT_SWITCH_STATEMENT,
    NT_RETURN_STATEMENT,
    NT_OUTPUT_STATEMENT,
    NT_INPUT_STATEMENT,
    NT_READ_FILE_STATEMENT,
    NT_WRITE_TO_FILE_STATEMENT,
    NT_CONTINUE_STATEMENT,
    NT_MATCH_STATEMENT,

    NT_ARENA_STATEMENT,
    NT_DEFER_STATEMENT,
    NT_RESET_STATEMENT,
    NT_CHALLOC_STATEMENT,
    
    NT_IMPORT_STATEMENT,
    NT_MODULE_STATEMENT,
    
    NT_TRY_STATEMENT,
    NT_CATCH_STATEMENT,
    NT_RAISE_STATEMENT,

    NT_THREAD_STATEMENT,
    NT_ENABLE_T_STATEMENT,
    NT_JOIN_STATEMENT,
    NT_CHAN_STATEMENT,



}NonTerminal;

typedef struct{
    uint16_t lhs;
    uint8_t rhs_len;
    const char *name;
}GrammarRule;

static GrammarRule GRAMMAR_RULES[] = {
    {0, 0, "INVALID_RULE"},

    {NT_PROGRAM, 1, "stmt -> stmt_list"},

    {NT_TYPE, 1, "type -> TOKEN_KW_INT"},
    {NT_TYPE, 1, "type -> TOKEN_KW_SHORT"},
    {NT_TYPE, 1, "type -> TOKEN_KW_LONG"},
    {NT_TYPE, 1, "type -> TOKEN_KW_BYTE"},
    {NT_TYPE, 1, "type -> TOKEN_KW_FLOAT"},
    {NT_TYPE, 1, "type -> TOKEN_KW_DOUBLE"},
    {NT_TYPE, 1, "type -> TOKEN_KW_CHAR"},
    {NT_TYPE, 1, "type -> TOKEN_KW_STRING"},
    {NT_TYPE, 1, "type -> TOKEN_KW_BOOL"},
    {NT_TYPE, 1, "type -> TOKEN_KW_STRUCT"},
    {NT_TYPE, 1, "type -> TOKEN_KW_ENUM"},
    {NT_TYPE, 1, "type -> TOKEN_KW_UNION"},

    {NT_VAR_DECLARATION, 5, "var_decl -> type IDENTIFIER ASSIGN factor SEMICOLON"},
    {NT_FACTOR, 1, "factor -> TOKEN_INT_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_FLOAT_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_CHAR_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_STRING_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_BOOL_LITERAL"},

    {NT_STATEMENT, 1, "stmt -> var_decl"},
    {NT_STATEMENT_LIST, 2, "stmt_list -> stmt_list stmt"},
    {NT_STATEMENT_LIST, 1, "stmt_list -> stmt"}
};
#endif

