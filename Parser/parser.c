#define _POSIX_C_SOURCE 200809L

#include "parser.h"
#include "parse_structs.h"
#include <bits/pthreadtypes.h>
#include <stdio.h>

#include <stdlib.h>

#define CURRENT_STATE(stack_ptr) ((stack_ptr)->items[(stack_ptr)->top].state)


ASTNode *make_ast(int rule_id, ASTNode **popped_nodes){

    //SWITCH STATEMENT OF DOOM AND DESPAIR
    switch (rule_id){
        case 0: { 
            ASTNode *prog = calloc(1, sizeof(ASTNode)); 
            prog->type = AST_PROGRAM;
            prog->left = popped_nodes[0];
            free(popped_nodes[1]); //free eof
            prog->next = NULL;
            return prog; 
        }
        case 1: return PASS_FREE_R(popped_nodes);
        case 2: return PASS(popped_nodes);
        case 3: return PASS(popped_nodes);
        case 4: return ASSIGN(popped_nodes, OP_ASSIGN);
        case 5: return ASSIGN(popped_nodes, OP_PLUS_EQ);
        case 6: return ASSIGN(popped_nodes, OP_MINUS_EQ);
        case 7: return ASSIGN(popped_nodes, OP_STAR_EQ);
        case 8: return ASSIGN(popped_nodes, OP_SLASH_EQ);
        case 9: return ASSIGN(popped_nodes, OP_MOD_EQ);
        case 10: return ASSIGN(popped_nodes, OP_LSHIFT_ASSIGN);
        case 11: return ASSIGN(popped_nodes, OP_RSHIFT_ASSIGN);
        case 12: return PASS(popped_nodes);
        case 13: {
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
        case 14: return PASS(popped_nodes);
        case 15: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_OR);
        case 16: return PASS(popped_nodes);
        case 17: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_AND);
        case 18: return PASS(popped_nodes);
        case 19: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_BIT_OR);
        case 20: return PASS(popped_nodes);
        case 21: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_XOR);
        case 22: return PASS(popped_nodes);
        case 23: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_BIT_AND);
        case 24: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_NAND);
        case 25: return PASS(popped_nodes);
        case 26: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_EQ);
        case 27: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_NEQ);
        case 28: return PASS(popped_nodes);
        case 29: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_LT);
        case 30: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_GT);
        case 31: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_LE);
        case 32: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_GE);
        case 33: return PASS(popped_nodes);
        case 34: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_PLUS);
        case 35: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_MINUS);
        case 36: return PASS(popped_nodes);
        case 37: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_STAR);
        case 38: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_SLASH);
        case 39: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_MOD);
        case 40: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_PLUS);
        case 41: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_MINUS);
        case 42: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_NOT);
        case 43: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_BIT_NOT);
        case 44: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_ADDR_OF);
        case 45: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_DEREF);
        case 46: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_AWAIT);
        case 47: return PASS(popped_nodes);
        case 48: return PASS(popped_nodes);
        case 49: return use_2_trash_2(popped_nodes[0], popped_nodes[2], popped_nodes[1], popped_nodes[3], AST_POSTFIX);
        case 50: return use_2_trash_2(popped_nodes[0], popped_nodes[2], popped_nodes[1], popped_nodes[3], AST_POSTFIX);
        case 51: return postfix_node(popped_nodes[0], popped_nodes[2], popped_nodes[1], AST_POSTFIX, OP_DOT);
        case 52: return postfix_node(popped_nodes[0], popped_nodes[2], popped_nodes[1], AST_POSTFIX, OP_SCOPE_RES);
        case 53: return postfix_node(popped_nodes[0], popped_nodes[2], popped_nodes[1], AST_POSTFIX, OP_ARROW);
        case 54: return postfix_node(popped_nodes[0], popped_nodes[2], popped_nodes[1], AST_POSTFIX, OP_LEFT_ARROW);
        case 55: return use_2_trash_2(popped_nodes[1], popped_nodes[3], popped_nodes[0], popped_nodes[2], AST_CAST_EXPR);
        case 56: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_PRIMARY;
            node->lexeme = strdup(popped_nodes[0]->lexeme);
            return node;
        }
        case 57: return PASS(popped_nodes);
        case 58: return use_2_trash_2(popped_nodes[1],NULL, popped_nodes[0], popped_nodes[2], AST_PRIMARY);
        case 59: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_NULL_LITERAL;
            node->op = OP_NULL;
            node->lexeme = strdup("NULL");
            node->left = NULL;
            node->right = NULL;
            free(popped_nodes[0]);
            return node;
        }
        case 60: return PASS(popped_nodes);
        case 61: return NULL;
        case 62: return PASS_CLEAR_NEXT(popped_nodes);
        case 63: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[2];
            if (!new_member) { free(popped_nodes[1]); return head; }
            new_member->next = NULL;
            if (!head) { free(popped_nodes[1]); return new_member; }
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            free(popped_nodes[1]);
            return head;
        }
        case 64: return PASS(popped_nodes);
        case 65: return PASS(popped_nodes);
        case 66: return PASS(popped_nodes);
        case 67: return PASS(popped_nodes);
        case 68: return PASS(popped_nodes);
        case 69: return PASS(popped_nodes);
        case 70: return PASS(popped_nodes);
        case 71: return PASS(popped_nodes);
        case 72: return PASS(popped_nodes);
        case 73: return PASS(popped_nodes);
        case 74: return PASS(popped_nodes);
        case 75: return PASS(popped_nodes);
        case 76: return PASS(popped_nodes);
        case 77: return PASS(popped_nodes);
        case 78: return PASS(popped_nodes);
        case 79: return PASS(popped_nodes);
        case 80: return PASS(popped_nodes);
        case 81: return PASS(popped_nodes);
        case 82: return PASS(popped_nodes);
        case 83: return PASS(popped_nodes);
        case 84: return PASS(popped_nodes);
        case 85: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[1];
            if (!new_member) { free(popped_nodes[1]); return head; }
            new_member->next = NULL;
            if (!head) { free(popped_nodes[1]); return new_member; }
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            return head;
        }
        case 86: return PASS_CLEAR_NEXT(popped_nodes);
        case 87: return PASS(popped_nodes);
        case 88: return PASS(popped_nodes);
        case 89: return PASS(popped_nodes);
        case 90: return PASS(popped_nodes);
        case 91: return PASS(popped_nodes);
        case 92: return PASS(popped_nodes);
        case 93: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_STRUCT_DECL;
            ASTNode *name = calloc(1, sizeof(ASTNode));
            name->lexeme = strdup(popped_nodes[1]->lexeme);
            name->left = NULL;
            name->right = NULL;
            node->left = name;
            node->right = popped_nodes[2];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            return node;
        }
        case 94: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->left = popped_nodes[1];
            free(popped_nodes[0]);
            free(popped_nodes[2]);
            free(popped_nodes[3]);
            return node;
        }
        case 95: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->lexeme = strdup(popped_nodes[0]->lexeme);
            node->left = NULL;
            node->right = NULL;
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            return node;
        }
        case 96: { //struct member list(long)
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[1];
            if (!new_member) { free(popped_nodes[1]); return head; }
            new_member->next = NULL;
            if (!head) { free(popped_nodes[1]); return new_member; }
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            return head;
        }
        case 97: return PASS_CLEAR_NEXT(popped_nodes);
        case 98: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_ENUM_DECL;
            ASTNode *name = calloc(1, sizeof(ASTNode));
            name->lexeme = strdup(popped_nodes[1]->lexeme);
            node->left = name;
            node->right = popped_nodes[3];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[2]);
            free(popped_nodes[4]);
            free(popped_nodes[5]);
            return node;
        }
        case 99: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_ENUM_DECL;
            ASTNode *l_name, *r_name = calloc(1, sizeof(ASTNode));
            l_name->lexeme = strdup(popped_nodes[1]->lexeme);
            r_name->lexeme = strdup(popped_nodes[2]->lexeme);
            l_name->left = r_name->left = l_name->right = r_name->right = NULL;
            node->left = l_name;
            node->right = r_name;
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[2]);
            free(popped_nodes[3]);
            return node;
        }
        case 100: { //enum member list(long)
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[2];
            if (!new_member) { free(popped_nodes[1]); return head; }
            new_member->next = NULL;
            if (!head) { free(popped_nodes[1]); return new_member; }
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            free(popped_nodes[1]);
            return head;
        }
        case 101: return PASS_CLEAR_NEXT(popped_nodes); //enum member list(base)
        case 102: {
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
        case 103: {
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
        case 104: {//union member list(long)
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[1];
            if (!new_member) { free(popped_nodes[1]); return head; }
            new_member->next = NULL;
            if (!head) { free(popped_nodes[1]); return new_member; }
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            return head;
        }
        case 105: return PASS_CLEAR_NEXT(popped_nodes); //union member list(base)
        case 106: {
            ASTNode *list = popped_nodes[0];
            ASTNode *new_decl = popped_nodes[1];
            if(!new_decl){ return list; }
            new_decl->next = NULL;
            if(!list){ return new_decl; }
            ASTNode *curr = list;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_decl;
            return list;
        }
        case 107: return PASS_CLEAR_NEXT(popped_nodes);
        case 108: return NULL;
        case 109: {
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
        case 110: {
            ASTNode *var_decl = calloc(1, sizeof(ASTNode));
            var_decl->type = AST_VAR_DECL;
            var_decl->left = popped_nodes[0];
            var_decl->right = popped_nodes[1];
            free(popped_nodes[2]);
            return var_decl;
        }
        case 111: {
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
        case 112: {
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
        case 113: {
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
            free(popped_nodes[5]);
            return func_decl;
        }
        case 114: {
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
        case 115: {
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
        case 116: {
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
        case 117: return MODIFIER_Q(popped_nodes, TQ_CONST);// 9 + 10   
        case 118: return MODIFIER_Q(popped_nodes, TQ_MUT);
        case 119: return MODIFIER_SC(popped_nodes, SC_STATIC);
        case 120: return MODIFIER_Q(popped_nodes, TQ_VOLATILE);
        case 121: return MODIFIER_SC(popped_nodes, SC_ATOMIC);
        case 122: return MODIFIER_VS(popped_nodes, VS_SHARED);
        case 123: return MODIFIER_VS(popped_nodes, VS_PRIVATE);
        case 124: return MODIFIER_VS(popped_nodes, VS_PUBLIC);
        case 125: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_modifier = popped_nodes[1];
            if (!new_modifier) { free(popped_nodes[1]); return head; }
            new_modifier->next = NULL;
            if (!head) { free(popped_nodes[1]); return new_modifier; }
            ASTNode *curr = head;
            while (curr->next != NULL) { curr = curr->next; }
            curr->next = new_modifier;
            return head;
        }
        case 126: return PASS(popped_nodes);
        case 127: return NULL;
        case 128: return PASS(popped_nodes);
        case 129: return PASS(popped_nodes);
        case 130: {  //parser rule 34 ( ͡° ͜ʖ ͡°)
            ASTNode *node = popped_nodes[0];
            node->type_info.pointer_level++;
            free(popped_nodes[1]);
            return node;
        }
        case 131: {
            ASTNode *node = popped_nodes[0];
            node->type_info.flag |= F_REFERENCE;
            free(popped_nodes[1]);
            return node;
        }
        case 132: return PASS(popped_nodes);
        case 133: {
           ASTNode *node = popped_nodes[1];
           node->type_info.flag |= F_CHANNEL;
           free(popped_nodes[0]);
           return node;
        }
        case 134: return SET_TYPE(popped_nodes, PT_INT);
        case 135: return SET_TYPE(popped_nodes, PT_SHORT);
        case 136: return SET_TYPE(popped_nodes, PT_LONG);
        case 137: return SET_TYPE(popped_nodes, PT_BYTE);
        case 138: return SET_TYPE(popped_nodes, PT_FLOAT);
        case 139: return SET_TYPE(popped_nodes, PT_DOUBLE);
        case 140: return SET_TYPE(popped_nodes, PT_CHAR);
        case 141: return SET_TYPE(popped_nodes, PT_STRING);
        case 142: return SET_TYPE(popped_nodes, PT_BOOL);
        case 143: return SET_TYPE(popped_nodes, PT_STRUCT);
        case 144: return SET_TYPE(popped_nodes, PT_ENUM);
        case 145: return SET_TYPE(popped_nodes, PT_UNION);
        case 146: return SET_TYPE(popped_nodes, PT_VOID);
        case 147: return SET_SIMPLE_TYPE(popped_nodes, PT_MUTEX);
        case 148: {
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
        case 149: return MUTATE_AST_TERMINAL(popped_nodes, AST_INT_LITERAL);
        case 150: return MUTATE_AST_TERMINAL(popped_nodes, AST_FLOAT_LITERAL);
        case 151: return MUTATE_AST_TERMINAL(popped_nodes, AST_CHAR_LITERAL);
        case 152: return MUTATE_AST_TERMINAL(popped_nodes, AST_STRING_LITERAL);
        case 153: return MUTATE_AST_TERMINAL(popped_nodes, AST_BOOL_LITERAL);
        case 154: return FREE_NULL(popped_nodes);
        case 155: return PASS(popped_nodes);
        case 156: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_FUNC_BODY;
            node->left = popped_nodes[1];
            free(popped_nodes[0]);
            free(popped_nodes[2]);
            return node;
        }
        case 157: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_PARAM;
            node->left = popped_nodes[0];
            if(popped_nodes[1] && popped_nodes[1]->name){
                    node->name = strdup(popped_nodes[1]->name);
                }
            free(popped_nodes[1]);
            return node;
        }
        case 158: return PASS(popped_nodes);
        case 159: return NULL;
        case 160: return PASS(popped_nodes);
        case 161: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_param = popped_nodes[2];
            if (!new_param) { free(popped_nodes[1]); return head; }
            new_param->next = NULL;
            if (!head) { free(popped_nodes[1]); return new_param; }
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_param;
            free(popped_nodes[1]);
            return head;
        }
        case 162: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_BLOCK);
        case 163: return use_2_trash_2(NULL, NULL, popped_nodes[0], popped_nodes[0], AST_BLOCK);
        case 164: return use_2_trash_2(popped_nodes[1], popped_nodes[2], popped_nodes[0], popped_nodes[3], AST_CASE_BLOCK);
        case 165: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_CASE_BLOCK);
        case 166: return use_2_trash_2(popped_nodes[1], popped_nodes[2], popped_nodes[0], popped_nodes[3], AST_MATCH_BLOCK);
        case 167: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_MATCH_BLOCK);
        case 168: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[1];
            if (!new_member) { free(popped_nodes[1]); return head; }
            new_member->next = NULL;
            if (!head) { free(popped_nodes[1]); return new_member; }
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            return head;
        }
        case 169: return PASS_CLEAR_NEXT(popped_nodes);
        case 170: return use_2_trash_2(popped_nodes[0], popped_nodes[2], popped_nodes[1], popped_nodes[3], AST_MATCH_ARM);
        case 171: return use_2_trash_2(popped_nodes[0], popped_nodes[2], popped_nodes[1], popped_nodes[3], AST_ARRAY_STRUCT);
        case 172: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_ARRAY_STRUCT);
        case 173: return use_2_trash_2(NULL, NULL, popped_nodes[0], popped_nodes[1], AST_ARRAY_STRUCT);
        case 174: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_ARRAY_INIT);
        case 175: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[2];
            if (!new_member) { free(popped_nodes[1]); return head; }
            new_member->next = NULL;
            if (!head) { free(popped_nodes[1]); return new_member; }
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            free(popped_nodes[1]);
            return head;
        }
        case 176: return PASS_CLEAR_NEXT(popped_nodes);
        case 177: return PASS(popped_nodes);
        case 178: return PASS(popped_nodes);
        case 179: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_IF_STMT;
            node->left = popped_nodes[2];
            node->middle = popped_nodes[4];
            node->right = popped_nodes[5];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }
        case 180: return PASS_FREE_L(popped_nodes);
        case 181: return PASS(popped_nodes);
        case 182: return PASS_FREE_L(popped_nodes);
        case 183: return NULL;
        case 184: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_WHILE_STMT;
            node->left = popped_nodes[2];
            node->right = popped_nodes[4];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }
        case 185: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_DO_STMT;
            node->left = popped_nodes[1];
            node->right = popped_nodes[4];
            free(popped_nodes[0]);
            free(popped_nodes[2]);
            free(popped_nodes[3]);
            free(popped_nodes[5]);
            free(popped_nodes[6]);
            return node;
    }
        case 186: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_FOR_STMT;
            node->left = popped_nodes[2];
            node->right = popped_nodes[4];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }
        case 187: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_FOR_INIT;
                ASTNode *assign = calloc(1, sizeof(ASTNode));
                assign->type = AST_ASSIGNMENT;
                    ASTNode *for_expressions = calloc(1, sizeof(ASTNode));
                    for_expressions->type = AST_EXPR_LIST;
                    for_expressions->left = popped_nodes[3];
                    for_expressions->middle = popped_nodes[5];
                    for_expressions->right = popped_nodes[7];
                assign->left = popped_nodes[1];
                assign->right = for_expressions;
            node->left = popped_nodes[0];
            node->right = assign;
            free(popped_nodes[2]);
            free(popped_nodes[4]);
            free(popped_nodes[6]);
            return node;
        }
        case 188: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_SWITCH_STMT;
            node->left = popped_nodes[2];
            node->right = popped_nodes[4];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }
        case 189: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_CASE_STMT;
            node->left = popped_nodes[1];
            node->right = popped_nodes[3];
            free(popped_nodes[0]);
            free(popped_nodes[2]);
            free(popped_nodes[4]);
            free(popped_nodes[5]);
            return node;
        }
        case 190: {
            ASTNode * node = calloc(1, sizeof(ASTNode));
            node->type = AST_DEFAULT_STMT;
            node->left = popped_nodes[2];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            free(popped_nodes[4]);
            return node;
        }
        case 191: return continue_break_return(popped_nodes[0], popped_nodes[1], AST_CONTINUE_STMT);
        case 192: return continue_break_return(popped_nodes[0], popped_nodes[1], AST_BREAK_STMT);
        case 193: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[1];
            new_member->next = NULL;
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            return head;
        }
        case 194: return PASS_CLEAR_NEXT(popped_nodes);
        case 195: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_RETURN_STMT);
        case 196: return continue_break_return(popped_nodes[0], popped_nodes[1], AST_RETURN_STMT);
        case 197: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MATCH_STMT;
            node->left = popped_nodes[2];
            node->right = popped_nodes[5];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            free(popped_nodes[4]);
            return node;
        }
        case 198: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_DEFAULT_MATCH_STMT;
            node->left = popped_nodes[2];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }
        case 199: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_DEFER_STMT;
            node->left = popped_nodes[2];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            free(popped_nodes[4]);
            return node;
        }
        case 200: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_IMPORT_STMT;
            ASTNode *name = calloc(1, sizeof(ASTNode));
            name->lexeme = strdup(popped_nodes[3]->lexeme);
            name->left = name->right = NULL;
            node->left = name;
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            free(popped_nodes[2]);
            free(popped_nodes[4]);
            return node;
        }
        case 201:  {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODULE_STMT;
            node->left = popped_nodes[1];
            free(popped_nodes[0]);
            free(popped_nodes[2]);
            return node;
        }
        case 202: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODULE_LIST;
            node->op = OP_SCOPE_RES;
            ASTNode *field_node = calloc(1, sizeof(ASTNode));
            field_node->lexeme = strdup(popped_nodes[2]->lexeme);
            field_node->left = field_node->right = NULL;
            node->left = popped_nodes[0];
            node->right = field_node;
            free(popped_nodes[1]);
            free(popped_nodes[2]);
            return node;
        }
        case 203: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_MODULE_LIST;
            node->lexeme = strdup(popped_nodes[0]->lexeme);
            node->left = node->right = NULL;
            return node;
        }
        case 204: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_TRY_STMT;
            node->left = popped_nodes[3];
            node->right = popped_nodes[5];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[2]);
            free(popped_nodes[4]);
            return node;
        }
        case 205: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_CATCH_STMT;
            ASTNode *name = calloc(1, sizeof(ASTNode));
            name->lexeme = strdup(popped_nodes[2]->lexeme);
            name->left = name->right = NULL;
            node->left = name;
            node->right = popped_nodes[6];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[2]);
            free(popped_nodes[3]);
            free(popped_nodes[4]);
            free(popped_nodes[5]);
            free(popped_nodes[7]);
            return node;
        }
        case 206: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_RAISE_STMT);
        case 207: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_SPAWN_STMT);
        case 208: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], NULL, AST_SELECT_STMT);
        case 209: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_LOCK_STMT;
            node->left = popped_nodes[2];
            node->right = NULL;
            node->middle = popped_nodes[4];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }

        default: return NULL;
    }
}
ASTNode *make_terminal_ast(TokenStruct *tokens){
    ASTNode *node = calloc(1, sizeof(ASTNode));
    node->type = AST_TERMINAL;
    node->op = (Operations)tokens->token;
    node->line = tokens->line;
    node->col = tokens->column;
    if(tokens && tokens->lexeme){
        node->lexeme = strdup(tokens->lexeme);
    }else{
        node->lexeme = NULL;
    }
    switch (tokens->token){
        case TOKEN_IDENTIFIER: { if(tokens->lexeme) node->name = strdup(tokens->lexeme); break; }
        case TOKEN_STRING_LITERAL: { if(tokens->lexeme) node->name = strndup(tokens->lexeme, tokens->length); break; }
        case TOKEN_CHAR_LITERAL: { if(tokens->lexeme) node->name = strndup(tokens->lexeme, tokens->length); break; }
        case TOKEN_INT_LITERAL: {if (tokens->lexeme) {
        char buf[64] = {0};
        uint32_t len = tokens->length < 63 ? tokens->length : 63;
        memcpy(buf, tokens->lexeme, len);
        node->int_val = strtoll(buf, NULL, 0);
            } break;}
        case TOKEN_BOOL_LITERAL:{ if (tokens->lexeme) { node->int_val = (strcmp(tokens->lexeme, "true") == 0) ? 1 : 0; } break; }
        case TOKEN_FLOAT_LITERAL:{node->float_val = strtod(tokens->lexeme, 0); break; }
        default: break;
    }
    return node;
}

ParserStack create_stack(int32_t initial_stack){
    ParserStack stack;
    stack.top = -1;
    stack.capacity = initial_stack;
    stack.items = malloc(sizeof(StackItem) * initial_stack);
    return stack;
}

TokenStruct *get_current_token(TokenStream *stream) {
    if (!stream || !stream->tokens || stream->read_idx >= stream->count) {
        return &EOF_TOKEN;
    }
    return &stream->tokens[stream->read_idx];
}

ASTNode *fetch_tokens(TokenStream *stream){
    ParserStack stack = create_stack(1024);

    stack.top = 0;
    stack.items[stack.top].state = 0;
    stack.items[stack.top].node = NULL;

    for(;;){
        TokenStruct *current_token = get_current_token(stream);

        int current_state = stack.items[stack.top].state;
        int act = ACTION_TABLE[current_state][current_token->token];

        if(IS_SHIFT(act)){
            int next_state = GET_SHIFT_STATE(act);

            stack.top++;
            stack.items[stack.top].state = next_state;
            stack.items[stack.top].node = make_terminal_ast(current_token);
            
            stream->read_idx++;

        }else if(IS_REDUCE(act)){
            int rule_id = GET_REDUCE_RULE(act);
            int rhs_len = RULE_RHS_LENGTHS[rule_id];
            int lhs_nonterm = RULE_LHS_NONTERMINALS[rule_id];

            ASTNode *popped_nodes[64] = {0};
            for (int i = 0; i < rhs_len; i++) {
                popped_nodes[i] = stack.items[stack.top - rhs_len + 1 + i].node;
            }
            stack.top -= rhs_len;
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
            if (current_token->token == TOKEN_EOF && stack.top >= 0 && stack.items[stack.top].node != NULL) {
                printf("[+] Parsing successful!\n");
                ASTNode *final_ast_node = stack.items[stack.top].node;
                free(stack.items);
                free(stream->tokens);
                return final_ast_node;
            }
           fprintf(stderr, "[!] Syntax error at line %d near token '%s'\n", 
                    current_token->line, current_token->lexeme);
                    free(stack.items);
                    free(stream->tokens);
                    return NULL;
        }
    }
    
}