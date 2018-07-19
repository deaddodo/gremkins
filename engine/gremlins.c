#include "gremlins.h"

/**
 * Makes sure to set the initial gremlin parameters to *sane* values, not necessarily *correct* (leave that to the
 * post-spawn callback)
 *
 * @param gremlin
 * @param id
 */
void init_gremlin(gremlin_t* gremlin, unsigned short id) {
    gremlin->position.x = 0;
    gremlin->position.y = 0;
    gremlin->status = GREMLIN_STATUS_ALIVE;
    gremlin->type = GREMLIN_TYPE_PLAYER;
    gremlin->id = id; // probably unnecessary, but let's track ids regardless
}

/**
 * Spawns a new gremlin character, returns true on spawn
 *
 * @param this
 * @return bool
 */
bool gremlin_factory_spawn(gremlin_factory_t* this) {
    int count = this->count(this);
    unsigned short next = (unsigned short)(count + 1);

    if(count == this->max) {
        return false;
    }

    if(next > 0xffff) {
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

/**
 * Unspawns the current set of gremlins
 *
 * @param this
 */
void gremlin_factory_unspawn(gremlin_factory_t* this) {
    this->iter.reset(&(this->iter));
    this->iter.resize(&(this->iter), 0);
    free(this->spawns);
}

/**
 * Use the factory iterator to get a current count of spawns
 *
 * @param this
 * @return int
 */
int gremlin_factory_count(gremlin_factory_t* this) {
    return this->iter.size(&(this->iter));
}

/**
 * Yields the next gremlin in the iterator.
 *
 * @param this
 * @return gremlin_t*
 */
gremlin_t* gremlin_factory_yield(gremlin_factory_t* this) {
    int idx = this->iter.next(&(this->iter));

    if(!idx) {
        return 0;
    }

    return &(this->spawns[idx-1]);
}

/**
 * Executes a callback function on each spawned gremlin
 *
 * @param this
 * @param cb
 */
void gremlin_factory_execute(gremlin_factory_t* this, bool (*cb)(gremlin_t*)) {
    while(true) {
        gremlin_t* current = this->yield(this);

        if(current == 0) {
            break; // we're done iterating
        }

        cb(current);
    }
}

/**
 * Primary method to scaffold the gremlin factory
 *
 * @param factory
 * @return bool
 */
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

/**
 * Deinitializes the gremlin factory
 *
 * @param factory
 */
void destroy_gremlin_factory(gremlin_factory_t* factory) {
    factory->unspawn(factory);
}
