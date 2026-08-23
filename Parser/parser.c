#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

#define CURRENT_STATE(stack_ptr) ((stack_ptr)->items[(stack_ptr)->top].state)


ASTNode *make_ast(int rule_id, ASTNode **popped_nodes){
    switch (rule_id){
        case 0: return popped_nodes[0];
        case 1: { 
            ASTNode *prog = calloc(1, sizeof(ASTNode));
                prog->type = AST_PROGRAM;
                prog->left = popped_nodes[0];
                free(popped_nodes[1]); //free eof
                prog->next = NULL;
                return prog;
         }
         case 2: {
            popped_nodes[0]->next = NULL;
            free(popped_nodes[1]);
            return popped_nodes[0];
         }
         case 3: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_ASSIGNMENT;
                node->left = popped_nodes[0];
                node->right = popped_nodes[2];
                node->op = OP_LSHIFT_ASSIGN;
                node->next = NULL;
                free(popped_nodes[1]);
                return node;
         }
         case 4: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[2];
            new_member->next = NULL;
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            free(popped_nodes[1]);
            return head;
         }
         case 5: {
            popped_nodes[0]->next = NULL;
            return popped_nodes[0];
         }
         case 6: {
            
         }
        default: { break; }
    }
    
}
ASTNode *make_terminal_ast(TokenStruct *tokens){

}

ParserStack create_stack(int32_t initial_stack){
    ParserStack stack;
    stack.top = -1;
    stack.capacity = initial_stack;
    stack.items = malloc(sizeof(StackItem) * initial_stack);
    return stack;
}

ASTNode *fetch_tokens(TokenStream *stream){
    ParserStack stack = create_stack(1024);
    stack.top = -1;

    stack.top++;
    stack.items[stack.top].state = 0;

    TokenStruct *current_token = stream->tokens;

    for(;;){
        int current_state = stack.items[stack.top].state;
        int act = ACTION_TABLE[current_state][current_token->token];

        if(IS_SHIFT(act)){
            int next_state = GET_SHIFT_STATE(act);

            stack.top++;
            stack.items[stack.top].state = next_state;
            stack.items[stack.top].node = make_terminal_ast(current_token);
            current_token++;

        }else if(IS_REDUCE(act)){
            int rule_id = GET_REDUCE_RULE(act);
            int rhs_len = RULE_RHS_LEN[rule_id];
            int lhs_nonterm = RULE_LHS_NONTERMINALS[rule_id];

            ASTNode *popped_nodes[64];
            for (int i = rhs_len - 1; i >= 0; i--) {
                popped_nodes[i] = stack.items[stack.top].node;
                stack.top--;
            }
            ASTNode *reduced_node = make_ast(rule_id, popped_nodes);

            int latest_state = CURRENT_STATE(&stack);
            int goto_state = GOTO_TABLE[latest_state][lhs_nonterm];

            stack.top++;
            stack.items[stack.top].state = goto_state;
            stack.items[stack.top].node = reduced_node;
        }else if(act == ACTION_ACCEPT){
            printf("[+] Parsing successful!\n");

            ASTNode *final_ast_node = stack.items[stack.top].node;

            free(stack.items);

            return final_ast_node;
        }else{
           fprintf(stderr, "[!] Syntax error at line %d near token '%s'\n", 
                    current_token->line, current_token->lexeme);
                    free(stack.items);
                    return NULL;
        }
    }
    free(stream->tokens);
}