#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

#define CURRENT_STATE(stack_ptr) ((stack_ptr)->items[(stack_ptr)->top].state)


ASTNode *make_ast(int rule_id, ASTNode *popped_nodes){

}

ParserStack create_stack(int32_t initial_stack){
    ParserStack stack;
    stack.capacity = initial_stack;
    stack.count = 0;
    stack.items = malloc(sizeof(StackItem) * initial_stack);
    return stack;
}

void fetch_tokens(TokenStream *stream){
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
           free(stream);
        }else{
           fprintf(stderr, "[!] Syntax error at line %d near token '%s'\n", 
                    current_token->line, current_token->lexeme);
            free(stream);
            return -1;
        }
    }
    free(stream);
}