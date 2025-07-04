#include "buffer.h"

string_list_t* init_string() {
    string_list_t *s = malloc(sizeof(string_list_t));
    s->cap = LIST_CAPACITY;
    s->size = 0;
    s->data = malloc(sizeof(char *) * s->cap);

    if(!s->data) {
        fprintf(stderr, "Failed to allocate memory for string_t"); 
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < LIST_CAPACITY; ++i) {
        s->data[i] = malloc(sizeof(char) * BUFFER_LIMIT);
        if(!s->data[i]) {
            fprintf(stderr, "Failed to allocate memory for string_t"); 
            for(int j = 0; j < BUFFER_LIMIT; ++j) {
                free(s->data[j]);
            }

            free(s->data);
            exit(EXIT_FAILURE);
        }
    }

    return s;    
}

void append_string(string_list_t *s, char *item) {
    if(s->size >= s->cap)  {
        size_t new_cap = s->cap * 2;
        char **temp = realloc(s->data, sizeof(char *) * new_cap);
        if(!temp) {
            fprintf(stderr, "Failed to reallocate memory for String"); 
            free(temp);
            exit(EXIT_FAILURE);
        }
        
        s->cap = new_cap;
        s->data = temp;
    }
    
    // size is 0
    // s->data[0] = :tmi.twitch.tv
    // s->data[1] = 421    
    // s->data[2] = you
    // s->data[3] = QPFGDKJGKLJGLKJGLK 
    // s->data[4] = :Unknown
    // s->data[5] = command
    // final size is 6
    s->data[s->size++] = item;
}
