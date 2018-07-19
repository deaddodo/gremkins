#include "gremlins.h"

void init_gremlin(gremlin_t* gremlin, unsigned short id) {
    printf("gremlin init\n");
    gremlin->position.x = 0;
    gremlin->position.y = 0;
    gremlin->status = GREMLIN_STATUS_ALIVE;
    gremlin->type = GREMLIN_TYPE_PLAYER;
    gremlin->id = id; // probably unnecessary, but let's track ids regardless
}

bool gremlin_factory_spawn(gremlin_factory_t* this) {
    int count = this->count(this);
    unsigned short next = (unsigned short)(count + 1);

    if(count == this->max) {
        return false;
    }

    // 65536 gremlins is plenty
    if(next > sizeof(unsigned short)) {
        return false;
    }

    if(count == 0) {
        this->spawns = malloc(sizeof(gremlin_t));
    } else {
        // TODO: make a buffered alloc
        // this is the worst possible alloc, since we know we'll have more
        // than a few spawns. we should switch this to use a buffered growth factor
        // of ~1.5
        this->spawns = realloc(
                this->spawns,
                sizeof(gremlin_t) * (next)
        );
    }

    init_gremlin(&(this->spawns[count]), next);

    this->iter.resize(&(this->iter), next);
    return true;
}

void gremlin_factory_unspawn(gremlin_factory_t* this) {
    free(this->spawns);
}

int gremlin_factory_count(gremlin_factory_t* this) {
    return this->iter.size(&(this->iter));
}

gremlin_t* gremlin_factory_yield(gremlin_factory_t* this) {
    int idx = this->iter.next(&(this->iter));

    if(!idx) {
        return 0;
    }

    return &(this->spawns[idx-1]);
}

void gremlin_factory_execute(gremlin_factory_t* this, bool (*cb)(gremlin_t*)) {
    while(true) {
        gremlin_t* current = this->yield(this);

        if(current == 0) {
            break; // we're done iterating
        }

        cb(current);
    }
}

bool init_gremlin_factory(gremlin_factory_t* factory) {
    factory->max = 255;

    // TODO: convert this to a vtable
    factory->spawn = &gremlin_factory_spawn;
    factory->unspawn = &gremlin_factory_unspawn;
    factory->count = &gremlin_factory_count;
    factory->yield = &gremlin_factory_yield;
    factory->execute = &gremlin_factory_execute;

    init_iterator(&(factory->iter));

    return true;
}

void destroy_gremlin_factory(gremlin_factory_t* factory) {
    factory->unspawn(factory);
}