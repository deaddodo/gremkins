#pragma once

typedef struct iterator_t_ {
    int current, max;
    int  (*next)(struct iterator_t_* this);
    int  (*size)(struct iterator_t_* this);
    void (*resize)(struct iterator_t_* this, int size);
    void (*reset)(struct iterator_t_* this);
} iterator_t;

void init_iterator(iterator_t* iterator);
