#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "engine/gremlins.h"

bool gremlin_collide(gremlin_t* gremlin) {
    printf("[%hu] (%i) x: %hu, y: %hu\n",
           gremlin->id, gremlin->status, gremlin->position.x, gremlin->position.y
    );
    return true;
}

static int SPAWN_COUNT = 15;

int main() {
    gremlin_factory_t factory;
    init_gremlin_factory(&factory);

    int spawns = SPAWN_COUNT;

    while(spawns) {
        printf("spawns: %i\n", spawns);
        factory.spawn(&factory);
        --spawns;
    }

    factory.execute(&factory, &gremlin_collide);

    destroy_gremlin_factory(&factory);
    return 0;
}
