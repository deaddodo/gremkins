#include "iter.h"

int iterator_next(iterator_t* this) {
    ++this->current;
    if(this->current == this->max+1) {
        this->reset(this);
        return 0;
    }

    return this->current;
}

int iterator_size(iterator_t* this) {
    return this->max;
}

void iterator_resize(iterator_t* this, int size) {
    this->max = size;
}

void iterator_reset(iterator_t* this) {
    this->current = 0;
}

void init_iterator(iterator_t* iterator) {
    iterator->current = 0;
    iterator->max = 0;

    iterator->next   = &iterator_next;
    iterator->size   = &iterator_size;
    iterator->resize = &iterator_resize;
    iterator->reset  = &iterator_reset;
}