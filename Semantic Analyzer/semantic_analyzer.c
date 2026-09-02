#include "semantic_analyzer.h"
#include <stdio.h>

void print_ast(ASTNode *ast, int depth){
   for (ASTNode *curr = ast; curr != NULL; curr = curr->next) {
        printf("%*sType: %d", depth * 2, "", curr->type);
        if (curr->lexeme) {
            printf(" (%s)", curr->lexeme);
        }
        printf("\n");

        print_ast(curr->left, depth + 1);
        print_ast(curr->right, depth + 1);
        print_ast(curr->middle, depth + 1);
    }
}