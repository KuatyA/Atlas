#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

#define CURRENT_STATE(stack_ptr) ((stack_ptr)->items[(stack_ptr)->top].state)


ASTNode *make_ast(int rule_id, ASTNode **popped_nodes){

    //SWITCH STATEMENT OF DOOM AND DESPAIR
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
         case 4: { //enum member list(long)
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[2];
            new_member->next = NULL;
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            free(popped_nodes[1]);
            return head;
         }
         case 5: { //enum member list(base)
            popped_nodes[0]->next = NULL;
            return popped_nodes[0];
         }
         case 6: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_UNION_DECL;
            node->type_info.p_type = PT_UNION;
            node->left = popped_nodes[1];
            node->right = popped_nodes[3];
            node->next = NULL;
            free(popped_nodes[0]);
            free(popped_nodes[2]);
            free(popped_nodes[4]);
            free(popped_nodes[5]);
            return node;
         }
         case 7: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_UNION_DECL;
            node->type_info.p_type = PT_UNION;
            node->left = popped_nodes[1];
            node->right = popped_nodes[2];
            node->next = NULL;
            free(popped_nodes[0]);
            free(popped_nodes[3]);
            return node;
         }
         case 8: {//union member list(long)
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[1];
            new_member->next = NULL;
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            return head;
         }
         case 9: {//union member list(base)
            popped_nodes[0]->next = NULL;
            return popped_nodes[0];
            }
        case 10: {
            ASTNode *list = popped_nodes[0];
            ASTNode *new_decl = popped_nodes[1];
            if(new_decl == NULL){ return list; }
            new_decl->next = NULL;
            if(list == NULL){ return new_decl; }
            ASTNode *curr = list;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_decl;
            return list;
        }
        case 11: { return NULL; }
        case 12: {
            ASTNode *var_decl = calloc(1, sizeof(ASTNode));
            var_decl->type = AST_VAR_DECL;
                ASTNode *assignment = calloc(1, sizeof(ASTNode));
                assignment->type = AST_ASSIGNMENT;
                assignment->left = popped_nodes[1];
                assignment->right = popped_nodes[3];
                assignment->op = OP_ASSIGN;
            var_decl->left = popped_nodes[0];
            var_decl->right = assignment;
            free(popped_nodes[2]);
            free(popped_nodes[4]);
            return var_decl;
        }
        case 13: {
            ASTNode *var_decl = calloc(1, sizeof(ASTNode));
            var_decl->type = AST_VAR_DECL;
            var_decl->left = popped_nodes[0];
            var_decl->right = popped_nodes[1];
            free(popped_nodes[2]);
            return var_decl;
        }
        case 14: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_ASSIGNMENT;
            node->left = popped_nodes[0];
            node->right = popped_nodes[2];
            node->op = OP_RSHIFT_ASSIGN;
            node->next = NULL;
            free(popped_nodes[1]);
            return node;
        }
        case 15: {
            ASTNode *var_decl = calloc(1, sizeof(ASTNode));
            var_decl->type = AST_VAR_DECL;
                ASTNode *assignment = calloc(1, sizeof(ASTNode));
                assignment->type = AST_ASSIGNMENT;
                    ASTNode *array_assign = calloc(1, sizeof(ASTNode));
                    array_assign->type = AST_ARRAY_ASSIGN;
                    array_assign->left = popped_nodes[2];
                    array_assign->right = popped_nodes[4];
                assignment->left = popped_nodes[1];
                assignment->right = array_assign;
                assignment->op = OP_ASSIGN;
            var_decl->left = popped_nodes[0];
            var_decl->right = assignment;
            free(popped_nodes[3]);
            free(popped_nodes[5]);
            return var_decl;
        }
        case 16: {
            ASTNode *var_decl = calloc(1, sizeof(ASTNode));
            var_decl->type = AST_VAR_DECL;
                ASTNode *assignment = calloc(1, sizeof(ASTNode));
                assignment->type = AST_ASSIGNMENT;
                assignment->left = popped_nodes[1];
                assignment->right = popped_nodes[2];
                assignment->op = OP_ASSIGN;
            var_decl->left = popped_nodes[0];
            var_decl->right = assignment;
            free(popped_nodes[3]);
            return var_decl;
        }
        case 17: {
            ASTNode *func_decl = calloc(1, sizeof(ASTNode));
            func_decl->type = AST_FUNC_DECL;
                ASTNode *details = calloc(1, sizeof(ASTNode));
                details->type = AST_FUNC_DETAILS;
                details->left = popped_nodes[2];
                details->right = popped_nodes[4];
                details->middle = popped_nodes[6];
            func_decl->left = popped_nodes[1];
            func_decl->right = details;
            free(popped_nodes[0]);
            free(popped_nodes[3]);
            free;(popped_nodes[5]);
            return func_decl;
        }
        case 18: {
            ASTNode *func_decl = calloc(1, sizeof(ASTNode));
            func_decl->type = AST_FUNC_DECL;
                ASTNode *details = calloc(1, sizeof(ASTNode));
                details->type = AST_FUNC_DETAILS;
                details->left = popped_nodes[3];
                details->right = popped_nodes[5];
                details->middle = popped_nodes[7];
            func_decl->left = popped_nodes[2];
            func_decl->right = details;
            func_decl->type_info.flag |= F_ASYNC;
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[4]);
            free(popped_nodes[6]);
            return func_decl;
        }
        case 19: {
            ASTNode *func_decl = calloc(1, sizeof(ASTNode));
            func_decl->type = AST_FUNC_DECL;
                ASTNode *details = calloc(1, sizeof(ASTNode));
                details->type = AST_FUNC_DETAILS;
                details->left = popped_nodes[3];
                details->right = popped_nodes[5];
                details->middle = popped_nodes[7];
            func_decl->left = popped_nodes[2];
            func_decl->right = details;
            func_decl->type_info.flag |= F_INLINE;
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[4]);
            free(popped_nodes[6]);
            return func_decl;
        }
        case 20: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_TYPEALIAS_DECL;
            node->left = popped_nodes[1];
            node->right = popped_nodes[3];
            node->op = OP_ASSIGN;
            free(popped_nodes[0]);
            free(popped_nodes[2]);
            free(popped_nodes[4]);
            return node;
        }
        case 21: { // 9 + 10
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODIFIER;
            node->type_info.qualifiers = TQ_CONST;
            free(popped_nodes[0]);
            return node;
        }
        case 22: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODIFIER;
            node->type_info.qualifiers = TQ_MUT;
            free(popped_nodes[0]);
            return node;
        }
        case 23: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODIFIER;
            node->type_info.storage_class = SC_STATIC;
            free(popped_nodes[0]);
            return node;
        }
        case 24: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODIFIER;
            node->type_info.qualifiers = TQ_VOLATILE;
            free(popped_nodes[0]);
            return node;
        }
        case 25: {
            popped_nodes[0]->next = NULL;
            return popped_nodes[0];
        }
        case 26: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODIFIER;
            node->type_info.storage_class = SC_ATOMIC;
            free(popped_nodes[0]);
            return node;
        }
        case 27: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODIFIER;
            node->type_info.visibility = VS_SHARED;
            free(popped_nodes[0]);
            return node;
        }
        case 28: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODIFIER;
            node->type_info.visibility = VS_PRIVATE;
            free(popped_nodes[0]);
            return node;
        }
        case 29: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODIFIER;
            node->type_info.visibility = VS_PUBLIC;
            free(popped_nodes[0]);
            return node;
        }
        case 30: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[1];
            new_member->next = NULL;
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            free(popped_nodes[1]);
            return head;
        }
        case 31: {
            popped_nodes[0]->next = NULL;
            return popped_nodes[0];
        }
        case 32: {
            popped_nodes[0]->next = NULL;
            return popped_nodes[0];
        }
        case 33: {
            popped_nodes[0]->next = NULL;
            return popped_nodes[0];
        }
        case 34: {  //parser rule 34 ( ͡° ͜ʖ ͡°)
            ASTNode *node = popped_nodes[0];
            node->type_info.pointer_level++;
            free(popped_nodes[1]);
            return node;
        }
        case 35: {
            ASTNode *node = popped_nodes[0];
            node->type_info.flag |= F_REFERENCE;
            free(popped_nodes[1]);
            return node;
        }
        case 36: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_TERNARY_EXPR;
                ASTNode *ternary_body = calloc(1, sizeof(ASTNode));
                ternary_body->type = AST_TERNARY_BODY;
                ternary_body->left = popped_nodes[2];
                ternary_body->right = popped_nodes[4];
            node->left = popped_nodes[0];
            node->right=ternary_body;
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }
        case 37: {
            popped_nodes[0]->next = NULL;
            return popped_nodes[0];
        }
        case 38: {
           ASTNode *node = popped_nodes[1];
           node->type_info.flag |= F_CHANNEL;
           free(popped_nodes[0]);
           return node;
        }
        case 39: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_INT;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 40: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_SHORT;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 41: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_LONG;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 42: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_BYTE;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 43: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_FLOAT;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 44: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_DOUBLE;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 45: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_CHAR;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 46: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_STRING;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 47: {
            popped_nodes[0]->next = NULL;
            return popped_nodes[0];
        }
        case 48: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_BOOL;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 49: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_STRUCT;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 50: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_ENUM;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 51: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_UNION;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 52: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_VOID;
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
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
            int rhs_len = RULE_RHS_LENGTHS[rule_id];
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