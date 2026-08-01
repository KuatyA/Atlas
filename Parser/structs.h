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
    NT_FUNC_DECLARATION,
    
    NT_TYPE,
    NT_FACTOR,

    NT_IDENTIFIER,
    
    NT_FUNC_SUFFIX,
    NT_FUNC_BODY,
    
    NT_PARAM,
    NT_PARAM_LIST,
    NT_PARAM_LIST_NONEMPTY,

    NT_BLOCK,

    NT_ARRAY_STRUCT,
    NT_ARRAY_INIT,

    NT_IF_STATEMENT,
    NT_ELSE,
    NT_LOWER_THAN_ELSE,
    NT_WHILE_STATEMENT,
    NT_FOR_STATEMENT,
    NT_SWITCH_STATEMENT,
    NT_CASE_STATEMENT,
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

    {NT_PROGRAM, 1, "program -> decl_list"},

    {NT_EXPRESSION_STATEMENT, 2, "expr_stmt -> expr TOKEN_SEMICOLON"},
    {NT_EXPRESSION_LIST, 3, "expr_list -> expr_list TOKEN_COMMA expr"},
    {NT_EXPRESSION_LIST, 1, "expr_list -> expr"},

    {NT_EXPRESSION, 1, "expr -> expr_list"},

    {NT_EXPRESSION, 3, "expr -> expr TOKEN_DOT expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_COLON expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_SCOPE_RES expr"},

    {NT_EXPRESSION, 3, "expr -> expr TOKEN_ASSIGN expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_PLUS expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_MINUS expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_STAR expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_SLASH expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_MOD expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_PLUS_EQ expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_MINUS_EQ expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_STAR_EQ expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_SLASH_EQ expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_MOD_EQ expr"},

    {NT_EXPRESSION, 3, "expr -> expr TOKEN_LT expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_GT expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_LE expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_GE expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_EQ expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_NEQ expr"},

    {NT_EXPRESSION, 3, "expr -> expr TOKEN_AND expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_OR expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_BIT_OR expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_NAND expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_XOR expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_NOT expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_BIT_NOT expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_LSHIFT expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_RSHIFT expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_LSHIFT_ASSIGN expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_RSHIFT_ASSIGN expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_QUESTION expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_AMPERSAND expr"},

    {NT_EXPRESSION, 3, "expr -> expr TOKEN_ARROW expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_FAT_ARROW expr"},
    {NT_EXPRESSION, 3, "expr -> expr TOKEN_LEFT_ARROW expr"},

    {NT_EXPRESSION, 1, "expr -> TOKEN_IDENTIFIER"},
    {NT_EXPRESSION, 3, "expr -> TOKEN_IDENTIFIER TOKEN_ASSIGN expr"},
    {NT_EXPRESSION, 4, "expr -> TOKEN_IDENTIFIER array_struct TOKEN_ASSIGN expr"},
    {NT_EXPRESSION, 2, "expr -> TOKEN_IDENTIFIER array_struct"},

    {NT_ARG_LIST, 1, "arg_list -> arg_list_nonempty"},
    {NT_ARG_LIST, 1, "arg_list -> TOKEN_KW_NULL"},
    {NT_ARG_LIST_NONEMPTY, 1, "arg_list_nonempty -> expr"},
    {NT_ARG_LIST_NONEMPTY, 2, "arg_list_nonempty -> arg_list_nonempty TOKEN_COMMA expr"},

    {NT_STATEMENT_LIST, 2, "stmt_list stmt"},
    {NT_STATEMENT, 1, "stmt -> var_decl"},
    {NT_STATEMENT, 1, "stmt -> expr_stmt"},
    {NT_STATEMENT, 1, "stmt -> if_stmt"},
    {NT_STATEMENT, 1, "stmt -> while_stmt"},
    {NT_STATEMENT, 1, "stmt -> do_stmt"},
    {NT_STATEMENT, 1, "stmt -> for_stmt"},
    {NT_STATEMENT, 1, "stmt -> switch_stmt"},
    {NT_STATEMENT, 1, "stmt -> return_stmt"},
    {NT_STATEMENT, 1, "stmt -> output_stmt"},
    {NT_STATEMENT, 1, "stmt -> input_stmt"},
    {NT_STATEMENT, 1, "stmt -> read_file_stmt"},
    {NT_STATEMENT, 1, "stmt -> write_to_file_stmt"},
    {NT_STATEMENT, 1, "stmt -> continue_stmt"},
    {NT_STATEMENT, 1, "stmt -> match_stmt"},

    {NT_STATEMENT, 1, "stmt -> arena_stmt"},
    {NT_STATEMENT, 1, "stmt -> defer_stmt"},
    {NT_STATEMENT, 1, "stmt -> reset_stmt"},
    {NT_STATEMENT, 1, "stmt -> challoc_stmt"},

    {NT_STATEMENT, 1, "stmt -> import_stmt"},
    {NT_STATEMENT, 1, "stmt -> module_stmt"},

    {NT_STATEMENT, 1, "stmt -> try_stmt"},
    {NT_STATEMENT, 1, "stmt -> catch_stmt"},
    {NT_STATEMENT, 1, "stmt -> raise_stmt"},

    {NT_STATEMENT, 1, "stmt -> thread_stmt"},
    {NT_STATEMENT, 1, "stmt -> enable_t_stmt"},
    {NT_STATEMENT, 1, "stmt -> join_stmt"},
    {NT_STATEMENT, 1, "stmt -> chan_stmt"},
    {NT_STATEMENT, 1, "stmt -> block"},

    {NT_STATEMENT_LIST, 2, "stmt_list -> stmt_list stmt"},
    {NT_STATEMENT_LIST, 1, "stmt_list -> stmt"},

    {NT_DECLARATION, 1, "decl -> var_decl"},
    {NT_DECLARATION, 1, "decl -> func_decl"},

    {NT_DECLARATION_LIST, 2, "decl_list -> decl_list decl"},
    {NT_DECLARATION_LIST, 1, "decl_list -> TOKEN_KW_NULL"},

    {NT_VAR_DECLARATION, 5, "var_decl -> type TOKEN_IDENTIFIER TOKEN_ASSIGN factor TOKEN_SEMICOLON"},
    {NT_VAR_DECLARATION, 3, "var_decl -> type TOKEN_IDENTIFIER TOKEN_SEMICOLON"},
    {NT_VAR_DECLARATION, 5, "var_decl -> type TOKEN_IDENTIFIER array_struct TOKEN_ASSIGN array_init"},
    {NT_VAR_DECLARATION, 4, "var_decl -> type TOKEN_IDENTIFIER array_struct TOKEN_SEMICOLON"},

    {NT_FUNC_DECLARATION, 7, "func_decl -> TOKEN_FUNCTION type TOKEN_IDENTIFIER TOKEN_LPAREN param_list TOKEN_RPAREN func_suffix"},

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

    {NT_FACTOR, 1, "factor -> TOKEN_INT_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_FLOAT_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_CHAR_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_STRING_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_BOOL_LITERAL"},

    {NT_IDENTIFIER, 2, "ident -> TOKEN_STAR TOKEN_IDENTIFIER"}, //for pointer and adresses if the programmer uses challoc and changes to manual memory handling.
    {NT_IDENTIFIER, 2, "ident -> TOKEN_AMPERSAND TOKEN_IDENTIFIER"},

    {NT_FUNC_SUFFIX, 1, "func_suffix -> TOKEN_SEMICOLON"},
    {NT_FUNC_SUFFIX, 1, "func_suffix -> func_body"},

    {NT_FUNC_BODY, 3, "func_body -> TOKEN_LBRACE stmt_list TOKEN_RBRACE"},
    
    {NT_PARAM, 2, "param -> type ident"},
    {NT_PARAM_LIST, 1, "param_list -> param_list_nonempty"},
    {NT_PARAM_LIST, 1, "param_list -> TOKEN_KW_NULL"},
    {NT_PARAM_LIST_NONEMPTY, 1, "parma_list_nonempty -> param"},
    {NT_PARAM_LIST_NONEMPTY, 3, "param_list_nonempty -> param_list_nonempty TOKEN_COMMA param_list"},

    {NT_BLOCK, 3, "block -> TOKEN_LBRACE stmt_list TOKEN_RBRACE"},
    
    {NT_ARRAY_STRUCT, 4, "array_struct -> array_struct TOKEN_LBRACKET expr TOKEN_RBRACKET"},
    {NT_ARRAY_STRUCT, 3, "array_struct -> TOKEN_LBRACKET expr TOKEN_RBRACKET"}, //stuff like a[2][4][7] or a[n]
    {NT_ARRAY_INIT, 3, "array_init -> TOKEN_LBRACE expr_list TOKEN_RBRACE"},

    {NT_IF_STATEMENT, 6, "if_stmt -> TOKEN_KW_IF TOKEN_LPAREN expr TOKEN_RPAREN block else"},
    {NT_ELSE, 2, "else -> TOKEN_KW_ELSE block"},
    {NT_ELSE, 1, "else -> lower_than_else"},
    {NT_LOWER_THAN_ELSE, 2, "lower_than_else -> TOKEN_KW_ELSE if_stmt"},
    {NT_LOWER_THAN_ELSE, 1, "lower_than_else -> TOKEN_KW_NULL"},
    {NT_WHILE_STATEMENT, 5, "while_stmt -> TOKEN_KW_WHILE TOKEN_LPAREN expr TOKEN_RPAREN stmt"},
    {NT_WHILE_STATEMENT, 7, "while_stmt -> TOKEN_KW_DO block TOKEN_KW_WHILE TOKEN_LPAREN expr TOKEN_RPAREN TOKEN_SEMICOLON"},
    {NT_FOR_STATEMENT, 5, "for_stmt -> TOKEN_KW_FOR TOKEN_LPAREN expr TOKEN_RPAREN stmt"},
    {NT_SWITCH_STATEMENT, 7, "switch_stmt -> TOKEN_KW_SWITCH LPAREN expr RPAREN "},







};

#endif

