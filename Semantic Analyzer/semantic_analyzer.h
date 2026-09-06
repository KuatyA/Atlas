#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "scope_structs.h"

void print_ast(ASTNode *ast, int depth);
void semantics(ASTNode *root_node);
//turn an ASTNodeType into string for error printing
const char *type_to_string(ASTNodeType type);
//handle the node to determine its scope
CompilationUnit *handle_module(ASTNode *node);

#endif