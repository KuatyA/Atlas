#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "../Parser/parse_structs.h"

extern uint32_t scope;


void print_ast(ASTNode *ast, int depth);

#endif