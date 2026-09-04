#ifndef SCOPE_STRUCTS
#define SCOPE_STRUCTS

#include "../Parser/parse_structs.h"
#include <regex>

typedef enum{
    SYM_VAR,
    SYM_FUNC,
    SYM_STRUCT,
    SYM_ENUM,
    SYM_UNION,
    SYM_TYPEALIAS,
    SYM_IMPORT
}SymbolKind;

typedef struct{
    char *lexeme;
    ASTNode *definition_node;
}SymbolTable;

typedef struct Scope{
    SymbolTable *symbols;
    struct Scope *parent;
    enum { SCOPE_GLOBAL, SCOPE_LOCAL, SCOPE_FUNCTION, SCOPE_BLOCK }ScopeLevel;
}Scope;

typedef struct{
    char *filename;
    ASTNode *ast_root;
    Scope *local_scope;
}Module;

typedef struct{
    Module *modules;
    uint32_t module_count;
    Scope *global_sym_table;
}CompilationUnit;


#endif