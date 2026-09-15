#ifndef PRESCANNER_H
#define PRESCANNER_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
    const char *name;
    uint32_t len;
}IdentifierEntry;

typedef struct {
    IdentifierEntry *entry;
    uint32_t count;
    uint32_t size;
}IdentifierTable;

void prescanner(const char *source, IdentifierTable *table);
void add_to_table(IdentifierTable *table, const char *name, uint32_t length);
void init_table(IdentifierTable *table, size_t inital_size);
bool check_table(const IdentifierTable *table, const char *name, uint32_t length);
void free_table(IdentifierTable *table);


#endif