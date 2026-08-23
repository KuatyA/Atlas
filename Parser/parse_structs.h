#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>

#define SYMBOL_TO_INDEX(sym) ((sym) >= NT_PROGRAM ? (TOKEN_COMMENT + 1 + ((sym) - NT_PROGRAM)) : (sym))

typedef enum{
    NT_PROGRAM  = 1000,

    NT_EXPRESSION,
    NT_EXPRESSION_STATEMENT,
    NT_EXPRESSION_LIST,

    NT_ASSIGNMENT,
    NT_TERNARY,
    NT_LOGICAL_OR,
    NT_LOGICAL_AND,
    NT_BIT_OR,
    NT_XOR,
    NT_BIT_AND,
    NT_EQUALITY,
    NT_RELATIONAL,
    NT_ADDITIVE,
    NT_MULTIPLICATIVE,
    NT_UNARY,
    NT_POSTFIX,
    NT_PRIMARY,

    NT_ARG_LIST,
    NT_ARG_LIST_NONEMPTY,
    
    NT_STATEMENT,
    NT_STATEMENT_LIST,

    NT_DECLARATION,
    NT_DECLARATION_LIST,
    NT_VAR_DECLARATION,
    NT_FUNC_DECLARATION,
    NT_STRUCT_DECLARATION,
    NT_STRUCT_BODY,
    NT_STRUCT_MEMBER_LIST,
    NT_ENUM_DECLARATION,
    NT_ENUM_MEMBER_LIST,
    NT_UNION_DECLARATION,
    NT_UNION_MEMBER_LIST,
    NT_TYPEALIAS_DECLARATION,
    
    NT_MODIFIER,
    NT_MODIFIER_LIST,
    NT_TYPE,
    NT_POINTER_TYPE,
    NT_BASE_TYPE,
    NT_CHANNEL_TYPE,
    NT_FACTOR,
    
    NT_FUNC_SUFFIX,
    NT_FUNC_BODY,
    
    NT_PARAM,
    NT_PARAM_LIST,
    NT_PARAM_LIST_NONEMPTY,

    NT_BLOCK,
    NT_CASE_BLOCK,
    NT_MATCH_BLOCK,

    NT_MATCH_ARM,

    NT_ARRAY_STRUCT,
    NT_ARRAY_INIT,
    NT_INIT_LIST,
    NT_INITIALIZER,

    NT_IF_STATEMENT,
    NT_ELSE,
    NT_LOWER_THAN_ELSE,
    NT_WHILE_STATEMENT,
    NT_DO_STATEMENT,
    NT_FOR_STATEMENT,
    NT_FOR_INIT,
    NT_SWITCH_STATEMENT,
    NT_DEFAULT_STMT,
    NT_CONTINUE_STATEMENT,
    NT_BREAK_STATEMENT,
    NT_CASE_STATEMENT,
    NT_CASE_LIST,
    NT_RETURN_STATEMENT,
    NT_MATCH_STATEMENT,
    NT_MATCH_LIST,
    NT_DEFAULT_MATCH_STATEMENT,

    NT_DEFER_STATEMENT,
    
    NT_IMPORT_STATEMENT,
    NT_MODULE_STATEMENT,
    NT_MODULE_LIST,
    
    NT_TRY_STATEMENT,
    NT_CATCH_STATEMENT,
    NT_RAISE_STATEMENT,

    NT_SPAWN_STATEMENT,
    NT_SELECT_STATEMENT,
    NT_LOCK_STATEMENT,



}NonTerminal;

typedef struct{
    uint16_t lhs;
    uint8_t rhs_len;
    const char *name;
}GrammarRule;

static GrammarRule GRAMMAR_RULES[] = {
    {0, 0, "INVALID_RULE"},

    {NT_PROGRAM, 2, "program -> decl_list TOKEN_EOF"},

    {NT_EXPRESSION_STATEMENT, 2, "expr_stmt -> expr TOKEN_SEMICOLON"},

    {NT_EXPRESSION, 1, "expr -> assignment"},

    {NT_ASSIGNMENT, 1, "assignment -> ternary"},
    {NT_ASSIGNMENT, 3, "assignment -> unary TOKEN_ASSIGN assignment"},
    {NT_ASSIGNMENT, 3, "assignment -> unary TOKEN_PLUS_EQ assignment"},
    {NT_ASSIGNMENT, 3, "assignment -> unary TOKEN_MINUS_EQ assignment"},
    {NT_ASSIGNMENT, 3, "assignment -> unary TOKEN_STAR_EQ assignment"},
    {NT_ASSIGNMENT, 3, "assignment -> unary TOKEN_SLASH_EQ assignment"},
    {NT_ASSIGNMENT, 3, "assignment -> unary TOKEN_MOD_EQ assignment"},
    {NT_ASSIGNMENT, 3, "assignment -> unary TOKEN_LSHIFT_ASSIGN assignment"},
    {NT_ASSIGNMENT, 3, "assignment -> unary TOKEN_RSHIFT_ASSIGN assignment"},

    {NT_TERNARY, 1, "ternary -> logical_or"},
    {NT_TERNARY, 5, "ternary -> logical_or TOKEN_QUESTION expr TOKEN_COLON ternary"},

    {NT_LOGICAL_OR, 1, "logical_or -> logical_and"},
    {NT_LOGICAL_OR, 3, "logical_or -> logical_or TOKEN_OR logical_and"},

    {NT_LOGICAL_AND, 1, "logical_and -> bit_or"},
    {NT_LOGICAL_AND, 3, "logical_and -> logical_and TOKEN_AND bit_or"},

    {NT_BIT_OR, 1, "bit_or -> xor"},
    {NT_BIT_OR, 3, "bit_or -> bit_or TOKEN_BIT_OR xor"},

    {NT_XOR, 1, "xor -> bit_and"},
    {NT_XOR, 3, "xor -> xor TOKEN_XOR bit_and"},

    {NT_BIT_AND, 1, "bit_and -> equality"},
    {NT_BIT_AND, 3, "bit_and -> bit_and TOKEN_AMPERSAND equality"},
    {NT_BIT_AND, 3, "bit_and -> bit_and TOKEN_NAND equality"},

    {NT_EQUALITY, 1, "equality -> relational"},
    {NT_EQUALITY, 3, "equality -> equality TOKEN_EQ relational"},
    {NT_EQUALITY, 3, "equality -> equality TOKEN_NEQ relational"},

    {NT_RELATIONAL, 1, "relational -> additive"},
    {NT_RELATIONAL, 3, "relational -> relational TOKEN_LT additive"},
    {NT_RELATIONAL, 3, "relational -> relational TOKEN_GT additive"},
    {NT_RELATIONAL, 3, "relational -> relational TOKEN_LE additive"},
    {NT_RELATIONAL, 3, "relational -> relational TOKEN_GE additive"},

    {NT_ADDITIVE, 1, "additive -> multiplicative"},
    {NT_ADDITIVE, 3, "additive -> additive TOKEN_PLUS multiplicative"},
    {NT_ADDITIVE, 3, "additive -> additive TOKEN_MINUS multiplicative"},

    {NT_MULTIPLICATIVE, 1, "multiplicative -> unary"},
    {NT_MULTIPLICATIVE, 3, "multiplicative -> multiplicative TOKEN_STAR unary"},
    {NT_MULTIPLICATIVE, 3, "multiplicative -> multiplicative TOKEN_SLASH unary"},
    {NT_MULTIPLICATIVE, 3, "multiplicative -> multiplicative TOKEN_MOD unary"},

    {NT_UNARY, 2, "unary -> TOKEN_PLUS unary"},
    {NT_UNARY, 2, "unary -> TOKEN_MINUS unary"},
    {NT_UNARY, 2, "unary -> TOKEN_NOT unary"},
    {NT_UNARY, 2, "unary -> TOKEN_BIT_NOT unary"},
    {NT_UNARY, 2, "unary -> TOKEN_AMPERSAND unary"},
    {NT_UNARY, 2, "unary -> TOKEN_STAR unary"},
    {NT_UNARY, 2, "unary -> TOKEN_KW_AWAIT unary"},
    {NT_UNARY, 1, "unary -> postfix"},

    {NT_POSTFIX, 1, "postfix -> primary"},
    {NT_POSTFIX, 4, "postfix -> postfix TOKEN_LPAREN arg_list TOKEN_RPAREN"},
    {NT_POSTFIX, 4, "postfix -> postfix TOKEN_LBRACKET expr TOKEN_RBRACKET"},
    {NT_POSTFIX, 3, "postfix -> postfix TOKEN_DOT TOKEN_IDENTIFIER"},
    {NT_POSTFIX, 3, "postfix -> postfix TOKEN_SCOPE_RES TOKEN_IDENTIFIER"},
    {NT_POSTFIX, 3, "postfix -> postfix TOKEN_ARROW TOKEN_IDENTIFIER"},
    {NT_POSTFIX, 3, "postfix -> postfix TOKEN_LEFT_ARROW TOKEN_IDENTIFIER"},

    {NT_EXPRESSION, 4, "expr -> TOKEN_KW_CAST type TOKEN_ARROW expr"},

    {NT_PRIMARY, 1, "primary -> TOKEN_IDENTIFIER"},
    {NT_PRIMARY, 1, "primary -> factor"},
    {NT_PRIMARY, 3, "primary -> TOKEN_LPAREN expr TOKEN_RPAREN"},
    {NT_PRIMARY, 1, "primary -> TOKEN_KW_NULL"},

    {NT_ARG_LIST, 1, "arg_list -> arg_list_nonempty"},
    {NT_ARG_LIST, 0, "arg_list -> "}, /*empty*/
    {NT_ARG_LIST_NONEMPTY, 1, "arg_list_nonempty -> expr"},
    {NT_ARG_LIST_NONEMPTY, 3, "arg_list_nonempty -> arg_list_nonempty TOKEN_COMMA expr"},

    {NT_STATEMENT, 1, "stmt -> var_decl"},
    {NT_STATEMENT, 1, "stmt -> expr_stmt"},
    {NT_STATEMENT, 1, "stmt -> if_stmt"},
    {NT_STATEMENT, 1, "stmt -> while_stmt"},
    {NT_STATEMENT, 1, "stmt -> do_stmt"},
    {NT_STATEMENT, 1, "stmt -> for_stmt"},
    {NT_STATEMENT, 1, "stmt -> switch_stmt"},
    {NT_STATEMENT, 1, "stmt -> return_stmt"},
    {NT_STATEMENT, 1, "stmt -> break_stmt"},
    {NT_STATEMENT, 1, "stmt -> continue_stmt"},
    {NT_STATEMENT, 1, "stmt -> match_stmt"},

    {NT_STATEMENT, 1, "stmt -> defer_stmt"},

    {NT_STATEMENT, 1, "stmt -> import_stmt"},
    {NT_STATEMENT, 1, "stmt -> module_stmt"},

    {NT_STATEMENT, 1, "stmt -> try_stmt"},
    {NT_STATEMENT, 1, "stmt -> catch_stmt"},
    {NT_STATEMENT, 1, "stmt -> raise_stmt"},

    {NT_STATEMENT, 1, "stmt -> spawn_stmt"},
    {NT_STATEMENT, 1, "stmt -> select_stmt"},
    {NT_STATEMENT, 1, "stmt -> lock_stmt"},
    {NT_STATEMENT, 1, "stmt -> block"},

    {NT_STATEMENT_LIST, 2, "stmt_list -> stmt_list stmt"},
    {NT_STATEMENT_LIST, 1, "stmt_list -> stmt"},

    {NT_DECLARATION, 1, "decl -> var_decl"},
    {NT_DECLARATION, 1, "decl -> func_decl"},
    {NT_DECLARATION, 1, "decl -> struct_decl"},
    {NT_DECLARATION, 1, "decl -> typealias_decl"},
    {NT_DECLARATION, 1, "decl -> enum_decl"},
    {NT_DECLARATION, 1, "decl -> union_decl"},

    {NT_STRUCT_DECLARATION, 3, "struct_decl -> TOKEN_KW_STRUCT TOKEN_IDENTIFIER struct_body"},
    {NT_STRUCT_BODY, 4, "struct_body -> TOKEN_LBRACE struct_member_list TOKEN_RBRACE TOKEN_SEMICOLON"},
    {NT_STRUCT_BODY, 2, "struct_body -> TOKEN_IDENTIFIER TOKEN_SEMICOLON"},
    {NT_STRUCT_MEMBER_LIST, 2, "struct_member_list -> struct_member_list var_decl"},
    {NT_STRUCT_MEMBER_LIST, 1, "struct_member_list -> var_decl"},

    {NT_ENUM_DECLARATION, 6, "enum_decl -> TOKEN_KW_ENUM TOKEN_IDENTIFIER TOKEN_LBRACE enum_member_list TOKEN_RBRACE TOKEN_SEMICOLON"},
    {NT_ENUM_DECLARATION, 4, "enum_decl -> TOKEN_KW_ENUM TOKEN_IDENTIFIER TOKEN_IDENTIFIER TOKEN_SEMICOLON"},
    {NT_ENUM_MEMBER_LIST, 3, "enum_member_list -> enum_member_list TOKEN_COMMA TOKEN_IDENTIFIER"},
    {NT_ENUM_MEMBER_LIST, 1, "enum_member_list -> TOKEN_IDENTIFIER"},

    {NT_UNION_DECLARATION, 6, "union_decl -> TOKEN_KW_UNION TOKEN_IDENTIFIER TOKEN_LBRACE union_member_list TOKEN_RBRACE TOKEN_SEMICOLON"},
    {NT_UNION_DECLARATION, 4, "union_decl -> TOKEN_KW_UNION TOKEN_IDENTIFIER TOKEN_IDENTIFIER TOKEN_SEMICOLON"},
    {NT_UNION_MEMBER_LIST, 2, "union_member_list -> union_member_list var_decl"},
    {NT_UNION_MEMBER_LIST, 1, "union_member_list -> var_decl"},

    {NT_DECLARATION_LIST, 2, "decl_list -> decl_list decl"},
    {NT_DECLARATION_LIST, 0, "decl_list -> "}, /*empty*/

    {NT_VAR_DECLARATION, 5, "var_decl -> type TOKEN_IDENTIFIER TOKEN_ASSIGN expr TOKEN_SEMICOLON"},
    {NT_VAR_DECLARATION, 3, "var_decl -> type TOKEN_IDENTIFIER TOKEN_SEMICOLON"},
    {NT_VAR_DECLARATION, 6, "var_decl -> type TOKEN_IDENTIFIER array_struct TOKEN_ASSIGN array_init TOKEN_SEMICOLON"},
    {NT_VAR_DECLARATION, 4, "var_decl -> type TOKEN_IDENTIFIER array_struct TOKEN_SEMICOLON"},

    {NT_FUNC_DECLARATION, 7, "func_decl -> TOKEN_KW_FUNCTION type TOKEN_IDENTIFIER TOKEN_LPAREN param_list TOKEN_RPAREN func_suffix"},
    {NT_FUNC_DECLARATION, 8, "func_decl -> TOKEN_KW_ASYNC TOKEN_KW_FUNCTION type TOKEN_IDENTIFIER TOKEN_LPAREN param_list TOKEN_RPAREN func_suffix"},

    {NT_TYPEALIAS_DECLARATION, 5, "typealias_decl -> TOKEN_KW_TYPEALIAS TOKEN_IDENTIFIER TOKEN_ASSIGN type TOKEN_SEMICOLON"},

    {NT_MODIFIER, 1, "modifier -> TOKEN_KW_CONST"},
    {NT_MODIFIER, 1, "modifier -> TOKEN_KW_MUT"},
    {NT_MODIFIER, 1, "modifier -> TOKEN_KW_STATIC"},
    {NT_MODIFIER, 1, "modifier -> TOKEN_KW_VOLATILE"},
    {NT_MODIFIER, 1, "modifier -> TOKEN_KW_ATOMIC"},
    {NT_MODIFIER, 1, "modifier -> TOKEN_KW_SHARED"},
    {NT_MODIFIER, 1, "modifier -> TOKEN_KW_PRIVATE"},
    {NT_MODIFIER, 1, "modifier -> TOKEN_KW_PUBLIC"},
    {NT_MODIFIER_LIST, 2, "modifier_list -> modifier_list modifier"},
    {NT_MODIFIER_LIST, 1, "modifier_list -> modifier"}, /*empty*/

    {NT_TYPE, 1, "type -> channel_type"},
    {NT_TYPE, 1, "type -> pointer_type"},

    {NT_POINTER_TYPE, 2, "pointer_type -> pointer_type TOKEN_STAR"},
    {NT_POINTER_TYPE, 2, "pointer_type -> pointer_type TOKEN_AMPERSAND"},
    {NT_POINTER_TYPE, 1, "pointer_type -> base_type"}, 

    {NT_CHANNEL_TYPE, 2, "channel_type -> TOKEN_KW_CHAN type"},

    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_INT"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_SHORT"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_LONG"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_BYTE"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_FLOAT"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_DOUBLE"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_CHAR"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_STRING"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_BOOL"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_STRUCT"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_ENUM"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_UNION"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_KW_VOID"},
    {NT_BASE_TYPE, 1, "base_type -> TOKEN_KW_MUTEX"},
    {NT_BASE_TYPE, 2, "base_type -> modifier_list TOKEN_IDENTIFIER"},

    {NT_FACTOR, 1, "factor -> TOKEN_INT_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_FLOAT_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_CHAR_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_STRING_LITERAL"},
    {NT_FACTOR, 1, "factor -> TOKEN_BOOL_LITERAL"},

    {NT_FUNC_SUFFIX, 1, "func_suffix -> TOKEN_SEMICOLON"},
    {NT_FUNC_SUFFIX, 1, "func_suffix -> func_body"},

    {NT_FUNC_BODY, 3, "func_body -> TOKEN_LBRACE stmt_list TOKEN_RBRACE"},
    
    {NT_PARAM, 2, "param -> type TOKEN_IDENTIFIER"},
    {NT_PARAM_LIST, 1, "param_list -> param_list_nonempty"},
    {NT_PARAM_LIST, 0, "param_list -> "}, /*empty*/
    {NT_PARAM_LIST_NONEMPTY, 1, "param_list_nonempty -> param"},
    {NT_PARAM_LIST_NONEMPTY, 3, "param_list_nonempty -> param_list_nonempty TOKEN_COMMA param"},

    {NT_BLOCK, 3, "block -> TOKEN_LBRACE stmt_list TOKEN_RBRACE"},
    {NT_BLOCK, 2, "block -> TOKEN_LBRACE TOKEN_RBRACE"},
    {NT_CASE_BLOCK, 4, "case_block -> TOKEN_LBRACE case_list default_stmt TOKEN_RBRACE"},
    {NT_CASE_BLOCK, 3, "case_block -> TOKEN_LBRACE case_list TOKEN_RBRACE"},
    {NT_MATCH_BLOCK, 4, "match_block -> TOKEN_LBRACE match_list default_match_stmt TOKEN_RBRACE"},
    {NT_MATCH_BLOCK, 3, "match_block -> TOKEN_LBRACE match_list TOKEN_RBRACE"},
    {NT_MATCH_LIST, 2, "match_list -> match_list match_arm"},
    {NT_MATCH_LIST, 1, "match_list -> match_arm"},
    {NT_MATCH_ARM, 4, "match_arm -> expr TOKEN_FAT_ARROW block TOKEN_SEMICOLON"},
    
    {NT_ARRAY_STRUCT, 4, "array_struct -> array_struct TOKEN_LBRACKET expr TOKEN_RBRACKET"},
    {NT_ARRAY_STRUCT, 3, "array_struct -> TOKEN_LBRACKET expr TOKEN_RBRACKET"}, //stuff like a[2][4][7] or a[n]
    {NT_ARRAY_STRUCT, 2, "array_struct -> TOKEN_LBRACKET TOKEN_RBRACKET"},
    {NT_ARRAY_INIT, 3, "array_init -> TOKEN_LBRACE init_list TOKEN_RBRACE"},
    {NT_INIT_LIST, 3, "init_list -> init_list TOKEN_COMMA initializer"},
    {NT_INIT_LIST, 1, "init_list -> initializer"},
    {NT_INITIALIZER ,1 ,"initializer -> expr"},
    {NT_INITIALIZER ,1 ,"initializer -> array_init"},

    {NT_IF_STATEMENT, 6, "if_stmt -> TOKEN_KW_IF TOKEN_LPAREN expr TOKEN_RPAREN block else"},
    {NT_ELSE, 2, "else -> TOKEN_KW_ELSE block"},
    {NT_ELSE, 1, "else -> lower_than_else"},
    {NT_LOWER_THAN_ELSE, 2, "lower_than_else -> TOKEN_KW_ELSE if_stmt"},
    {NT_LOWER_THAN_ELSE, 0, "lower_than_else -> "}, /*empty*/
    {NT_WHILE_STATEMENT, 5, "while_stmt -> TOKEN_KW_WHILE TOKEN_LPAREN expr TOKEN_RPAREN stmt"},
    {NT_DO_STATEMENT, 7, "do_stmt -> TOKEN_KW_DO block TOKEN_KW_WHILE TOKEN_LPAREN expr TOKEN_RPAREN TOKEN_SEMICOLON"},
    {NT_FOR_STATEMENT, 5, "for_stmt -> TOKEN_KW_FOR TOKEN_LPAREN for_init TOKEN_RPAREN stmt"},
    {NT_FOR_INIT, 8, "for_init -> type TOKEN_IDENTIFIER TOKEN_ASSIGN expr TOKEN_COMMA expr TOKEN_COMMA expr"},
    {NT_SWITCH_STATEMENT, 5, "switch_stmt -> TOKEN_KW_SWITCH TOKEN_LPAREN expr TOKEN_RPAREN case_block"},
    {NT_CASE_STATEMENT, 6, "case_stmt -> TOKEN_KW_CASE expr TOKEN_COLON block TOKEN_KW_BREAK TOKEN_SEMICOLON"},
    {NT_DEFAULT_STMT, 5, "default_stmt -> TOKEN_KW_DEFAULT TOKEN_COLON block TOKEN_KW_BREAK TOKEN_SEMICOLON"},
    {NT_CONTINUE_STATEMENT, 2, "continue_stmt -> TOKEN_KW_CONTINUE TOKEN_SEMICOLON"},
    {NT_BREAK_STATEMENT, 2, "break_stmt -> TOKEN_KW_BREAK TOKEN_SEMICOLON"},
    {NT_CASE_LIST, 2, "case_list -> case_list case_stmt"},
    {NT_CASE_LIST, 1, "case_list -> case_stmt"},
    {NT_RETURN_STATEMENT, 3, "return_stmt -> TOKEN_KW_RETURN expr TOKEN_SEMICOLON"},
    {NT_RETURN_STATEMENT, 2, "return_stmt -> TOKEN_KW_RETURN TOKEN_SEMICOLON"},
    {NT_MATCH_STATEMENT, 6, "match_stmt -> TOKEN_KW_MATCH TOKEN_LPAREN expr TOKEN_RPAREN TOKEN_COLON match_block"},
    {NT_DEFAULT_MATCH_STATEMENT, 4, "default_match_stmt -> TOKEN_KW_DEFAULT TOKEN_FAT_ARROW block TOKEN_SEMICOLON"},

    {NT_DEFER_STATEMENT, 5, "defer_stmt -> TOKEN_KW_DEFER TOKEN_LPAREN expr TOKEN_RPAREN TOKEN_SEMICOLON"},
   
    {NT_IMPORT_STATEMENT, 3, "import_stmt -> TOKEN_GT TOKEN_KW_IMPORT TOKEN_IDENTIFIER"},
    {NT_MODULE_STATEMENT, 3, "module_stmt -> TOKEN_KW_MODULE module_list TOKEN_SEMICOLON"},
    {NT_MODULE_LIST, 3, "module_list -> module_list TOKEN_SCOPE_RES TOKEN_IDENTIFIER"},
    {NT_MODULE_LIST, 1, "module_list -> TOKEN_IDENTIFIER"},

    {NT_TRY_STATEMENT, 4, "try_stmt -> TOKEN_KW_TRY TOKEN_COLON TOKEN_LBRACE stmt_list TOKEN_RBRACE catch_stmt"},
    {NT_CATCH_STATEMENT, 8, "catch_stmt -> TOKEN_KW_CATCH TOKEN_LPAREN TOKEN_IDENTIFIER TOKEN_RPAREN TOKEN_COLON TOKEN_LBRACE stmt_list TOKEN_RBRACE"},
    {NT_RAISE_STATEMENT, 3, "raise_stmt -> TOKEN_KW_RAISE expr TOKEN_SEMICOLON"},

    {NT_SPAWN_STATEMENT, 3, "spawn_stmt -> TOKEN_KW_SPAWN expr TOKEN_SEMICOLON"},
    {NT_SELECT_STATEMENT, 2, "select_stmt -> TOKEN_KW_SELECT case_block"},
    {NT_LOCK_STATEMENT, 6, "lock_stmt -> TOKEN_KW_LOCK TOKEN_LPAREN expr TOKEN_RPAREN block"},

};

typedef enum{
    AST_PROGRAM,

    AST_ARG_LIST,
    AST_INIT_LIST,
    AST_PARAM_LIST,
    AST_DECL_LIST,
    AST_EXPR_LIST,
    AST_STMT_LIST,
    AST_STRUCT_MEMBER_LIST,
    AST_ENUM_MEMBER_LIST,
    AST_UNION_MEMBER_LIST,
    AST_CASE_LIST,
    AST_MATCH_LIST,
    AST_MODULE_LIST,

    AST_ASSIGNMENT,

    AST_PARAM,
    AST_TYPE,
    AST_MODIFIER,
    AST_IDENTIFIER,
    AST_INITIALIZER,
    AST_ELSE,
    AST_BLOCK,

    AST_INT_LITERAL,
    AST_FLOAT_LITERAL,
    AST_CHAR_LITERAL,
    AST_STRING_LITERAL,
    AST_BOOL_LITERAL,
    AST_NULL_LITERAL,

    AST_VAR_DECL, 
    AST_FUNC_DECL,
    AST_STRUCT_DECL,
    AST_ENUM_DECL,
    AST_UNION_DECL,
    AST_TYPEALIAS_DECL,

    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_CALL_EXPR,
    AST_TERNARY_EXPR,
    AST_INDEX_EXPR,
    AST_MEMBER_ACCESS_EXPR,
    AST_CAST_EXPR,

    AST_ARRAY_INIT,
    AST_ARRAY_STRUCT,
    AST_FOR_INIT,
    AST_ENUM_MEMBER,
    
    AST_EXPR_STMT,
    AST_IF_STMT,
    AST_WHILE_STMT,
    AST_DO_STMT,
    AST_FOR_STMT,
    AST_SWITCH_STMT,
    AST_CASE_STMT,
    AST_RETURN_STMT,
    AST_BREAK_STMT,
    AST_DEFAULT_STMT,
    AST_DEFAULT_MATCH_STMT,
    AST_CONTINUE_STMT,
    AST_MATCH_STMT,
    AST_MATCH_ARM,

    AST_DEFER_STMT,

    AST_IMPORT_STMT,
    AST_MODULE_STMT,

    AST_TRY_STMT,
    AST_CATCH_STMT,
    AST_RAISE_STMT,

    AST_SPAWN_STMT,
    AST_SELECT_STMT,
    AST_LOCK_STMT,
}ASTNodeType;

typedef enum{
    NO_OP,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_LSHIFT_ASSIGN
}Operations;

typedef struct{
    ASTNodeType type;
    Operations op;

    struct ASTNode *left;
    struct ASTNode *middle;
    struct ASTNode *right;
    struct ASTNode *next;

    union{
        char *name; //for identifiers
        int int_val; // for integer and boolean literals 
        float float_val; //for float literals
        char *string_val; //for string literals
    }; 
    
}ASTNode;

typedef struct{
    int state;
    ASTNode *node;
}StackItem;

typedef struct{
    StackItem *items;
    int top;
    size_t capacity;
}ParserStack;

#endif