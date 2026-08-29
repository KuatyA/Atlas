#include "parser.h"
#include "parse_structs.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

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
        case 17: return BINARY(popped_nodes, AST_BINARY_EXPR, OP_AND);
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
        case 40: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_UPLUS);
        case 41: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_UMINUS);
        case 42: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_NOT);
        case 43: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_BIT_NOT);
        case 44: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_ADDR_OF);
        case 45: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_DEREF);
        case 46: return UNARY(popped_nodes, AST_UNARY_EXPR, OP_AWAIT);
        case 47: return PASS(popped_nodes);
        case 48: return PASS(popped_nodes);
        case 57: return PASS(popped_nodes);
        
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
        case 107: return NULL;
        case 108: {
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
        case 109: {
            ASTNode *var_decl = calloc(1, sizeof(ASTNode));
            var_decl->type = AST_VAR_DECL;
            var_decl->left = popped_nodes[0];
            var_decl->right = popped_nodes[1];
            free(popped_nodes[2]);
            return var_decl;
        }
        case 110: {
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
        case 111: {
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
        case 112: {
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
        case 113: {
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
            func_decl->type_info.flag |= F_INLINE;
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[4]);
            free(popped_nodes[6]);
            return func_decl;
        }
        case 115: {
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
        case 116: return MODIFIER_Q(popped_nodes, TQ_CONST);// 9 + 10   
        case 117: return MODIFIER_Q(popped_nodes, TQ_MUT);
        case 118: return MODIFIER_SC(popped_nodes, SC_STATIC);
        case 119: return MODIFIER_Q(popped_nodes, TQ_VOLATILE);
        case 120: return MODIFIER_SC(popped_nodes, SC_ATOMIC);
        case 121: return MODIFIER_VS(popped_nodes, VS_SHARED);
        case 122: return MODIFIER_VS(popped_nodes, VS_PRIVATE);
        case 123: return MODIFIER_VS(popped_nodes, VS_PUBLIC);
        case 128: {  //parser rule 34 ( ͡° ͜ʖ ͡°)
            ASTNode *node = popped_nodes[0];
            node->type_info.pointer_level++;
            free(popped_nodes[1]);
            return node;
        }
        case 129: {
            ASTNode *node = popped_nodes[0];
            node->type_info.flag |= F_REFERENCE;
            free(popped_nodes[1]);
            return node;
        }
        case 131: {
           ASTNode *node = popped_nodes[1];
           node->type_info.flag |= F_CHANNEL;
           free(popped_nodes[0]);
           return node;
        }
        case 132: return SET_TYPE(popped_nodes, PT_INT);
        case 133: return SET_TYPE(popped_nodes, PT_SHORT);
        case 134: return SET_TYPE(popped_nodes, PT_LONG);
        case 135: return SET_TYPE(popped_nodes, PT_BYTE);
        case 136: return SET_TYPE(popped_nodes, PT_FLOAT);
        case 137: return SET_TYPE(popped_nodes, PT_DOUBLE);
        case 138: return SET_TYPE(popped_nodes, PT_CHAR);
        case 139: return SET_TYPE(popped_nodes, PT_STRING);
        case 140: return SET_TYPE(popped_nodes, PT_BOOL);
        case 141: return SET_TYPE(popped_nodes, PT_STRUCT);
        case 142: return SET_TYPE(popped_nodes, PT_ENUM);
        case 143: return SET_TYPE(popped_nodes, PT_UNION);
        case 144: return SET_TYPE(popped_nodes, PT_VOID);
        case 145: return SET_SIMPLE_TYPE(popped_nodes, PT_MUTEX);
        case 146: {
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
        case 147: return MUTATE_AST_TERMINAL(popped_nodes, AST_INT_LITERAL);
        case 148: return MUTATE_AST_TERMINAL(popped_nodes, AST_FLOAT_LITERAL);
        case 149: return MUTATE_AST_TERMINAL(popped_nodes, AST_CHAR_LITERAL);
        case 150: return MUTATE_AST_TERMINAL(popped_nodes, AST_STRING_LITERAL);
        case 151: return MUTATE_AST_TERMINAL(popped_nodes, AST_BOOL_LITERAL);
        case 152: return FREE_NULL(popped_nodes);
        case 153: return PASS(popped_nodes);
        case 154: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_FUNC_BODY;
            node->left = popped_nodes[1];
            free(popped_nodes[0]);
            free(popped_nodes[2]);
            return node;
        }
        case 155: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_PARAM;
            node->left = popped_nodes[0];
            if(popped_nodes[1] && popped_nodes[1]->name){
                    node->name = strdup(popped_nodes[1]->name);
                }
            free(popped_nodes[1]);
            return node;
        }
        case 156: return PASS(popped_nodes);
        case 157: return NULL;
        case 158: return PASS(popped_nodes);
        case 159: {
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
        case 160: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_BLOCK);
        case 161: return use_2_trash_2(NULL, NULL, popped_nodes[0], popped_nodes[0], AST_BLOCK);
        case 162: return use_2_trash_2(popped_nodes[1], popped_nodes[2], popped_nodes[0], popped_nodes[3], AST_CASE_BLOCK);
        case 163: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_CASE_BLOCK);
        case 164: return use_2_trash_2(popped_nodes[1], popped_nodes[2], popped_nodes[0], popped_nodes[3], AST_MATCH_BLOCK);
        case 165: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_MATCH_BLOCK);
        case 166: {
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
        case 167: return PASS_CLEAR_NEXT(popped_nodes);
        case 168: return use_2_trash_2(popped_nodes[0], popped_nodes[2], popped_nodes[1], popped_nodes[3], AST_MATCH_ARM);
        case 169: return use_2_trash_2(popped_nodes[0], popped_nodes[2], popped_nodes[1], popped_nodes[3], AST_ARRAY_STRUCT);
        case 170: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_ARRAY_STRUCT);
        case 171: return use_2_trash_2(NULL, NULL, popped_nodes[0], popped_nodes[1], AST_ARRAY_STRUCT);
        case 172: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_ARRAY_INIT);
        case 173: {
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
        case 174: return PASS_CLEAR_NEXT(popped_nodes);
        case 175: return PASS(popped_nodes);
        case 176: return PASS(popped_nodes);
        case 177: {
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
        case 178: return PASS_FREE_L(popped_nodes);
        case 179: return PASS(popped_nodes);
        case 180: return PASS_FREE_L(popped_nodes);
        case 181: return NULL;
        case 182: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_WHILE_STMT;
            node->left = popped_nodes[2];
            node->right = popped_nodes[4];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }
        case 183: {
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
        case 184: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_FOR_STMT;
            node->left = popped_nodes[2];
            node->right = popped_nodes[4];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }
        case 185: {
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
        case 186: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_SWITCH_STMT;
            node->left = popped_nodes[2];
            node->right = popped_nodes[4];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }
        case 187: {
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
        case 188: {
            ASTNode * node = calloc(1, sizeof(ASTNode));
            node->type = AST_DEFAULT_STMT;
            node->left = popped_nodes[2];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            free(popped_nodes[4]);
            return node;
        }
        case 189: return continue_break_return(popped_nodes[0], popped_nodes[1], AST_CONTINUE_STMT);
        case 190: return continue_break_return(popped_nodes[0], popped_nodes[1], AST_BREAK_STMT);
        case 191: {
            ASTNode *head = popped_nodes[0];
            ASTNode *new_member = popped_nodes[1];
            new_member->next = NULL;
            ASTNode *curr = head;
            while(curr->next != NULL){ curr = curr->next; }
            curr->next = new_member;
            return head;
        }
        case 192: return PASS_CLEAR_NEXT(popped_nodes);
        case 193: return use_2_trash_2(popped_nodes[1], NULL, popped_nodes[0], popped_nodes[2], AST_RETURN_STMT);
        case 194: return continue_break_return(popped_nodes[0], popped_nodes[1], AST_RETURN_STMT);
        case 195: {
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
        case 196: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_DEFAULT_MATCH_STMT;
            node->left = popped_nodes[2];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            return node;
        }
        case 197: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_DEFER_STMT;
            node->left = popped_nodes[2];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[3]);
            free(popped_nodes[4]);
            return node;
        }
        case 198: {
            ASTNode *node = calloc(1, sizeof(ASTNode));
            node->type = AST_IMPORT_STMT;
            node->left = popped_nodes[3];
            free(popped_nodes[0]);
            free(popped_nodes[1]);
            free(popped_nodes[2]);
            free(popped_nodes[4]);
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
    switch (tokens->token){
        case TOKEN_IDENTIFIER: { if(tokens->lexeme) node->name = strdup(tokens->lexeme); break; }
        case TOKEN_STRING_LITERAL: { if(tokens->lexeme) node->name = strdup(tokens->lexeme); break; }
        case TOKEN_CHAR_LITERAL: { if(tokens->lexeme) node->name = strdup(tokens->lexeme); break; }
        case TOKEN_INT_LITERAL: { node->int_val = strtoll(tokens->lexeme, NULL, 0); break;}
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