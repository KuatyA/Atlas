#include "semantic_analyzer.h"
#include <stdio.h>

void print_ast(ASTNode *ast, int depth){
   for (ASTNode *curr = ast; curr != NULL; curr = curr->next) {
        printf("%*sType: %d", depth * 2, "", curr->type);
        if (curr->lexeme) {
            printf(" (%s)", curr->lexeme);
        }
        if(curr->type_info.flag || curr->type_info.qualifiers ||
           curr->type_info.visibility || curr->type_info.storage_class){
            printf(" Flag:(%d) Qualifier:(%d) Visibility:(%d) Storage Class:(%d)", 
                curr->type_info.flag,
                curr->type_info.qualifiers,
                curr->type_info.visibility,
                curr->type_info.storage_class);
        }
        printf("\n");

        print_ast(curr->left, depth + 1);
        print_ast(curr->right, depth + 1);
        print_ast(curr->middle, depth + 1);
    }

}
const char *primitive_type_to_string(PrimitiveType pt){
    switch(pt){
        case PT_INT: return "'int'";
        case PT_SHORT: return "'short'";
        case PT_LONG: return "'long'";
        case PT_BYTE: return "'byte'";
        case PT_FLOAT: return "'float'";
        case PT_DOUBLE: return "'double'";
        case PT_CHAR: return "'char'";
        case PT_STRING: return "'string'";
        case PT_BOOL: return "'bool'";
        case PT_STRUCT: return "'struct'";
        case PT_UNION: return "'union'";
        case PT_ENUM: return "'enum'";
        case PT_VOID: return "'void'";
        case PT_MUTEX: return "'mutex'";
        case PT_CUSTOM: return "'custom'";
    }
}
const char *flag_to_string(Flags f){
    switch(f){
        case F_NONE: return NULL;
        case F_ASYNC: return "'async'";
        case F_CHANNEL: return "'channel'";
        case F_INLINE: return "'inline'";
        case F_REFERENCE: return "'reference'";
    }
}
const char *storage_class_to_string(StorageClass sc){
    switch(sc){
        case SC_NONE: return NULL;
        case SC_ATOMIC: return "'atomic'";
        case SC_STATIC: return "'static'";
    }
}
const char *visibility_to_string(VisibilitySpecifiers vs){
    switch(vs){
        case VS_NONE: return NULL;
        case VS_PRIVATE: return "'private'";
        case VS_PUBLIC: return "'public'";
        case VS_SHARED: return "'shared'";
    }
}
const char *type_to_string(ASTNodeType type){
    switch(type){
        case AST_PROGRAM: return "program";
        case AST_TERMINAL: return "a terminal";
        case AST_ARG_LIST: return "the arguments list";
        case AST_INIT_LIST: return "the initializations list";
        case AST_PARAM_LIST: return "the parameters list";
        case AST_DECL_LIST: return "the declarations list";
        case AST_EXPR_LIST: return "the expressions list";
        case AST_STMT_LIST: return "the statements list";
        case AST_ENUM_MEMBER_LIST: return "the enums members list";
        case AST_STRUCT_MEMBER_LIST: return "the struct members list";
        case AST_UNION_MEMBER_LIST: return "the union members list";
        case AST_CASE_LIST: return "the case list";
        case AST_MATCH_LIST: return "the match list";
        case AST_MODULE_LIST: return "the module list";
        case AST_STRUCT_BODY: return "the struct body";
        case AST_ASSIGNMENT: return "the assignment";
        case AST_ARRAY_ASSIGN: return "the array assignment";
        case AST_FUNC_BODY: return "the function body";
        case AST_PARAM: return "the parameter";
        case AST_TYPE: return "the type";
        case AST_MODIFIER: return "the modifier";
        case AST_IDENTIFIER: return "the identifier";
        case AST_TYPE_IDENTIFIER: return "the type identifier";
        case AST_BLOCK: return "the block";
        case AST_CASE_BLOCK: return "the case block";
        case AST_MATCH_BLOCK: return "the match block";
        case AST_INT_LITERAL: return "the integer:";
        case AST_FLOAT_LITERAL: return "the float value:";
        case AST_CHAR_LITERAL: return "the character:";
        case AST_STRING_LITERAL: return "the string:";
        case AST_BOOL_LITERAL: return "the boolean:";
        case AST_NULL_LITERAL: return "the null value";
        case AST_VAR_DECL: return "the variable declaration";
        case AST_FUNC_DECL: return "the function declaration";
        case AST_STRUCT_DECL: return "the struct declaration";
        case AST_ENUM_DECL: return "the enum decalaration";
        case AST_UNION_DECL: return "the union declaration";
        case AST_TYPEALIAS_DECL: return "the custom type declaration";
        case AST_BINARY_EXPR: return "the binary expression";
        case AST_UNARY_EXPR: return "the unary expression";
        case AST_CALL_EXPR: return "the call expression";
        case AST_TERNARY_EXPR: return "the ternary expression";
        case AST_CAST_EXPR: return "the cast expression";
        case AST_ARRAY_INIT: return "the array initialization";
        case AST_ARRAY_STRUCT: return "the array structure";
        case AST_FOR_INIT: return "the \"for\" initialization";
        case AST_ENUM_MEMBER: return "the enum member";
        case AST_EXPR_STMT: return "the expression statement";
        case AST_IF_STMT: return "the \"if\" statement";
        case AST_WHILE_STMT: return "the \"while\" statement";
        case AST_DO_STMT: return "the \"do\" statement";
        case AST_FOR_STMT: return "the \"for\" statement";
        case AST_SWITCH_STMT: return "the \"switch\" statement";
        case AST_CASE_STMT: return "the \"case\" statement";
        case AST_RETURN_STMT: return "the \"return\" statement";
        case AST_BREAK_STMT: return "the \"break\" statement";
        case AST_DEFAULT_STMT: return "the \"default\" statement";
        case AST_DEFAULT_MATCH_STMT: return "the \"default\" statement";
        case AST_CONTINUE_STMT: return "the \"continue\" statement";
        case AST_MATCH_STMT: return "the \"match\" statement";
        case AST_MATCH_ARM: return "the match arm";
        case AST_IMPORT_STMT: return "the \"import\" statement";
        case AST_MODULE_STMT: return "the \"module\" statement";
        case AST_TRY_STMT: return "the \"try\" statement";
        case AST_CATCH_STMT: return "the \"catch\" statement";
        case AST_RAISE_STMT: return "the \"raise\" statement";
        case AST_SPAWN_STMT: return "the \"spawn\" statement";
        case AST_SELECT_STMT: return "the \"select\" statement";
        case AST_LOCK_STMT: return "the \"lock\" statement";
        case AST_DEFER_STMT: return "the \"defer\" statement";
        default: return "error on default states, fix it!\n(YES IM TALKING TO YOU, FUTURE ME, ADD ANOTHER CASE TO\n type_to_string at semantic_analyzer.c)";
          break;
        }
}
const char *type_qualifier_to_string(TypeQualfiers q){
    switch(q){
        case TQ_CONST: return "'const'";
        case TQ_MUT: return "'mut'";
        case TQ_VOLATILE: return "'voaltile'";
        case TQ_NONE: return NULL;
    }
}
CompilationUnit *handle_module(ASTNode *node){
   VisibilitySpecifiers level = node->type_info.visibility;
   switch(level){
    case VS_PRIVATE: {
        
    }
   }
}
void semantics(ASTNode *root_ast){

}
