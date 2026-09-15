#include "prescanner.h"
#include <stdio.h>



static const char *scan_type_declaration(const char *cursor,
        const char *keyword, size_t keyword_length, IdentifierTable *table){
    if(strncmp(cursor, keyword, keyword_length) != 0
        || isalnum((unsigned char)cursor[keyword_length])
        || cursor[keyword_length] == '_') {
        return NULL;
    }

    cursor += keyword_length;
    while(isspace((unsigned char)*cursor)){cursor++;}

    if(!isalpha((unsigned char)*cursor) && *cursor != '_') return NULL;

    const char *name_start = cursor;
    while(isalnum((unsigned char)*cursor) || *cursor == '_'){cursor++;}
    add_to_table(table, name_start, (uint32_t)(cursor - name_start));
    return cursor;
}

void prescanner(const char *source, IdentifierTable *table){
    const char *cursor = source;

    while(*cursor != '\0'){
        if(*cursor == '/' && cursor[1] == '/') {
            while (*cursor != '\n' && *cursor != '\0') cursor++;
            continue;
        }

        const char *next = scan_type_declaration(cursor, "struct", 6, table);
        if(!next) next = scan_type_declaration(cursor, "enum", 4, table);
        if(!next) next = scan_type_declaration(cursor, "union", 5, table);
        if(!next) next = scan_type_declaration(cursor, "typealias", 9, table);

        cursor = next ? next : cursor + 1;
    }
}
void add_to_table(IdentifierTable *table, const char *name, uint32_t length){
    if(check_table(table, name, length)) return;

    if(table->count >= table->size){
        table->size *= 2;
        table->entry = realloc(table->entry, table->size * sizeof(IdentifierEntry));
    }

    table->entry[table->count].name = name;
    table->entry[table->count].len = length;
    //printf("Name: %s, Length: %d\n", table->entry[table->count].name, table->entry[table->count].len);
    table->count++;
}
void init_table(IdentifierTable *table, size_t inital_size){
    table->size = inital_size > 0 ? inital_size : 16;
    table->count = 0;
    table->entry = malloc(table->size * sizeof(IdentifierEntry));
}
bool check_table(const IdentifierTable *table, const char *name, uint32_t length){
    for(size_t i = 0; i < table->count; i++){
        if(table->entry[i].len == length && 
        strncmp(table->entry[i].name, name, length) == 0){
            return true;
        }
    }
    return false;
}
void free_table(IdentifierTable *table){
    free(table->entry);
    table->entry = NULL;
    table->count = 0;
    table->size = 0;
}