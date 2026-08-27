#include "parser.h"
#include <stdio.h>
#include <string.h>

#define CURRENT_STATE(stack_ptr) ((stack_ptr)->items[(stack_ptr)->top].state)


ASTNode *make_ast(int rule_id, ASTNode **popped_nodes){

    //SWITCH STATEMENT OF DOOM AND DESPAIR
    switch (rule_id){
        case 0: PASS(popped_nodes);
        case 1: { 
            ASTNode *prog = calloc(1, sizeof(ASTNode)); 
            prog->type = AST_PROGRAM;
            prog->left = popped_nodes[0];
            free(popped_nodes[1]); //free eof
            prog->next = NULL;
            return prog;
         }
        case 2: return PASS_FREE_R(popped_nodes);
        case 3: return ASSIGN(popped_nodes, OP_LSHIFT_ASSIGN);
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
        case 11: return NULL;
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
        case 14: return ASSIGN(popped_nodes, OP_RSHIFT_ASSIGN);
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
        case 21: return MODIFIER_Q(popped_nodes, TQ_CONST);// 9 + 10   
        case 22: return MODIFIER_Q(popped_nodes, TQ_MUT);
        case 23: return MODIFIER_SC(popped_nodes, SC_STATIC);
        case 24: return MODIFIER_Q(popped_nodes, TQ_VOLATILE);
        case 25: PASS_CLEAR_NEXT(popped_nodes);
        case 26: return MODIFIER_SC(popped_nodes, SC_ATOMIC);
        case 27: return MODIFIER_VS(popped_nodes, VS_SHARED);
        case 28: return MODIFIER_VS(popped_nodes, VS_PRIVATE);
        case 29: return MODIFIER_VS(popped_nodes, VS_PUBLIC);
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
        case 31: PASS_CLEAR_NEXT(popped_nodes);
        case 32: PASS_CLEAR_NEXT(popped_nodes);
        case 33: PASS_CLEAR_NEXT(popped_nodes);
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
        case 37: PASS_CLEAR_NEXT(popped_nodes);
        case 38: {
           ASTNode *node = popped_nodes[1];
           node->type_info.flag |= F_CHANNEL;
           free(popped_nodes[0]);
           return node;
        }
        case 39: return SET_TYPE(popped_nodes, PT_INT);
        case 40: return SET_TYPE(popped_nodes, PT_SHORT);
        case 41: return SET_TYPE(popped_nodes, PT_LONG);
        case 42: return SET_TYPE(popped_nodes, PT_BYTE);
        case 43: return SET_TYPE(popped_nodes, PT_FLOAT);
        case 44: return SET_TYPE(popped_nodes, PT_DOUBLE);
        case 45: return SET_TYPE(popped_nodes, PT_CHAR);
        case 46: return SET_TYPE(popped_nodes, PT_STRING);
        case 47: return PASS_CLEAR_NEXT(popped_nodes);
        case 48: return SET_TYPE(popped_nodes, PT_BOOL);
        case 49: return SET_TYPE(popped_nodes, PT_STRUCT);
        case 50: return SET_TYPE(popped_nodes, PT_ENUM);
        case 51: return SET_TYPE(popped_nodes, PT_UNION);
        case 52: return SET_TYPE(popped_nodes, PT_VOID);
        case 53: return SET_SIMPLE_TYPE(popped_nodes, PT_MUTEX);
        case 54: {
            ASTNode *mod_node = popped_nodes[0];
                ASTNode *node = calloc(1, sizeof(ASTNode));
                node->type = AST_TYPE;
                node->type_info.p_type = PT_CUSTOM;
                if(popped_nodes[1] && popped_nodes[1]->name){
                    node->name = strdup(popped_nodes[1]->name);
                }
            if(mod_node != NULL){
                node->type_info.modifier |= mod_node->type_info.modifier;
                node->type_info.qualifiers |= mod_node->type_info.qualifiers;
                free(mod_node);
            }
            free(popped_nodes[1]);
            return node;
        }
        case 55: return MUTATE_AST_TERMINAL(popped_nodes, AST_INT_LITERAL);
        case 56: return MUTATE_AST_TERMINAL(popped_nodes, AST_FLOAT_LITERAL);
        case 57: return MUTATE_AST_TERMINAL(popped_nodes, AST_CHAR_LITERAL);
        case 58: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_OR);
        case 59: return MUTATE_AST_TERMINAL(popped_nodes, AST_STRING_LITERAL);
        case 60: return MUTATE_AST_TERMINAL(popped_nodes, AST_BOOL_LITERAL);
        case 61: return FREE_NULL(popped_nodes);
        case 62: return PASS(popped_nodes);
        case 63: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_FUNC_BODY;
            node->left = popped_nodes[1];
            free(popped_nodes[0]);
            free(popped_nodes[2]);
            return node;
        }
        case 64: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_PARAM;
            node->left = popped_nodes[0];
            if(popped_nodes[1] && popped_nodes[1]->name){
                    node->name = strdup(popped_nodes[1]->name);
                }
            free(popped_nodes[1]);
            return node;
        }
        case 65: PASS(popped_nodes);
        case 66: return NULL;
        case 67: PASS(popped_nodes);
        case 68: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_param = popped_nodes[2];
            new_param->next = NULL;
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_param;
            free(popped_nodes[1]);
            return head;
        }
        case 69: PASS(popped_nodes);
        case 70: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_BLOCK;
            node->left = popped_nodes[1];
            free(popped_nodes[0]);
            free(popped_nodes[2]);
            return node;
        }
        case 71: return (free(popped_nodes[0]), free(popped_nodes[1]), (ASTNode *)NULL);
        default: { break; }
    }
    
}
ASTNode *make_terminal_ast(TokenStruct *tokens){
    ASTNode *node = calloc(1, sizeof(ASTNode));
    node->type = AST_TERMINAL;
    node->op = tokens->token;
    node->line = tokens->line;
    node->col = tokens->column;
    switch (tokens->token){
        case TOKEN_IDENTIFIER: { if(tokens->lexeme) node->name = strdup(tokens->lexeme); break; }
        case TOKEN_STRING_LITERAL: { if(tokens->lexeme) node->name = strdup(tokens->lexeme); break; }
        case TOKEN_CHAR_LITERAL: { if(tokens->lexeme) node->name = strdup(tokens->lexeme); break; }
        case TOKEN_INT_LITERAL: { node->int_val = strtoll(tokens->lexeme, NULL, 0); break;}
        case TOKEN_BOOL_LITERAL:{ node->int_val = strtoll(tokens->lexeme, NULL, 0); break; }
        case TOKEN_FLOAT_LITERAL:{node->float_val = strtod(tokens->lexeme, 0); break; }
        default: break;
    }
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
            free(stream->tokens);

            return final_ast_node;
        }else{
           fprintf(stderr, "[!] Syntax error at line %d near token '%s'\n", 
                    current_token->line, current_token->lexeme);
                    free(stack.items);
                    free(stream->tokens);
                    return NULL;
        }
    }
    
}