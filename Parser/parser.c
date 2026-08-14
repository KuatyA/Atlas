#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

#define CURRENT_STATE(stack_ptr) ((stack_ptr)->items[(stack_ptr)->top].state)

//operation fuctions
ASTNode *shift(){

}
ASTNode *reduce(){

}
ASTNode *accept(){

}
ASTNode *error(){
    
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
    int token_id = stream->tokens->token;
    int state = CURRENT_STATE(&stack);
    int32_t act = ACTION_TABLE[state][token_id];
    if(act == ACTION_ACCEPT){

    }else if(act > 0){
        shift();
    }else if(act < 0){
        reduce();
    }else if(act == ACTION_ERROR){
        error();
    }
    free(stream);
}