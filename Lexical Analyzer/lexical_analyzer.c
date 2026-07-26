#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "lexical_analyzer.h"



const char *process_identifier(const char *buf){
    int first_char = (unsigned char)buf[0];
    if (!is_valid_identifier_start(first_char)) {
        fprintf(stderr, "ERROR: Invalid starting character for identifier: %s\n", buf);
        exit(1);
    }

    size_t len = strlen(buf);
    for (size_t idx = 0; idx < len; idx++) {
        int ch = (unsigned char)buf[idx];
        if (!is_valid_char(ch)) {
            fprintf(stderr, "ERROR: Invalid character at index %zu of identifier: %s\n", idx, buf);
            exit(1);
        }
    }

    return buf;
}

bool is_valid_char(int ch){
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
        return true;
    }
    if (ch >= '0' && ch <= '9') {
        return true;
    }
    if (ch == '_') {
        return true;
    }
    return false;
}

bool is_valid_identifier_start(int ch){
    return (ch >= 'a' && ch <= 'z') || 
           (ch >= 'A' && ch <= 'Z') || 
           (ch == '_');
}

void return_token(const char *string_buf){
    printf("Token: %s\n", string_buf);
}

int parse_file(const char *filepath){
    
    FILE *atl_file = fopen(filepath, "r");
    if(atl_file == NULL){
        perror("Error Opening File!");
        return 1;
    }

    char buf[256];
    int buf_index = 0;
    int ch;
    while((ch = fgetc(atl_file)) != EOF){
        if (isspace(ch)) {
            if (buf_index > 0) {
                buf[buf_index] = '\0';
                const char *string_buf = process_identifier(buf);
                return_token(string_buf);
                buf_index = 0;
            }
            continue;
        }

        if (ispunct((unsigned char)ch) && ch != '_') {

            if (buf_index > 0) {
                buf[buf_index] = '\0';
                const char *string_buf = process_identifier(buf);
                return_token(string_buf);
                buf_index = 0;
            }

            char op_buf[2] = {(char)ch, '\0'};
            return_token(op_buf);
            continue;
        }

        if (buf_index < sizeof(buf) - 1) {
            buf[buf_index++] = (char)ch;
        }
    }

    if (buf_index > 0) {
        buf[buf_index] = '\0';
        const char *string_buf = process_identifier(buf);
        return_token(string_buf);
    }

    fclose(atl_file);
    return 0;
}