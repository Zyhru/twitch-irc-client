#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>

#define LIST_CAPACITY 50
#define BUFFER_LIMIT 512

typedef enum {
    PARAM,
    TRAILING_PARAM
} irc_param_e;

typedef struct {
    char *data;
    size_t length;
} string_t;

typedef struct {
    size_t cap;
    size_t size;
    char **data;
} string_list_t;

string_list_t* init_string();
void append_string(string_list_t *s, char *item);

#endif
