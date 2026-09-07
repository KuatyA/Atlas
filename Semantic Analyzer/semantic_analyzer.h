#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "scope_structs.h"

void print_ast(ASTNode *ast, int depth);
void semantics(ASTNode *root_node);
//turn flag to string for error printing
const char *flag_to_string(Flags f);
//turn storage classes to string for error printing
const char *storage_class_to_string(StorageClass sc);
//turn visibility specifiers to string for error printing
const char *visibility_to_string(VisibilitySpecifiers vs);
//turn qualifiers to string for error printing
const char *type_qualifier_to_string(TypeQualfiers q);
//turn an ASTNodeType into string for error printing
const char *type_to_string(ASTNodeType type);
//turn a primitive type to string for error printing
const char *primitive_type_to_string(PrimitiveType pt);
//handle the node to determine its scope
CompilationUnit *handle_module(ASTNode *node);

#endif