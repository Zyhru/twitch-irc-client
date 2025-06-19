#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>

#define LIST_CAPACITY 50
#define BUFFER_LIMIT 500

typedef struct {
    size_t cap;
    size_t size;
    char **data;
} string_t;

string_t* init_string();
void append_string(string_t *s, char *item);

#endif
