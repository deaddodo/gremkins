#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../shared/iter.h"
#include "../shared/point.h"

/**
 * @brief primary entity structure
 *
 * This structure reflects the main "character entities in the game. This should stay small and only extend as necessary
 * as this will be alloc'd dozens-hundreds of times/level
 */
typedef struct {
    point_t position, velocity;
    unsigned char status, type;
    unsigned short id;
} gremlin_t;

enum {
    GREMLIN_STATUS_UNSPAWNED = 0, // buffered gremlins
    GREMLIN_STATUS_ALIVE     = 1, // spawned gremlins
    GREMLIN_STATUS_DEAD      = 2, // dead gremlins
    GREMLIN_STATUS_WARPED    = 3  // gremlin succeeded
};

enum {
    GREMLIN_TYPE_PLAYER      = 0,
    GREMLIN_TYPE_ENEMY       = 1,
    GREMLIN_TYPE_NPC         = 2
};

typedef struct gremlin_factory_t_ {
    gremlin_t* spawns;
    int max;
    iterator_t iter;
    bool       (*spawn)(struct gremlin_factory_t_* this);
    void       (*unspawn)(struct gremlin_factory_t_* this);
    int        (*count)(struct gremlin_factory_t_* this);
    gremlin_t* (*yield)(struct gremlin_factory_t_* this);
    void       (*execute)(struct gremlin_factory_t_* this, bool (*cb)(gremlin_t*));
} gremlin_factory_t;

bool init_gremlin_factory(gremlin_factory_t* factory);
void destroy_gremlin_factory(gremlin_factory_t* factory);
