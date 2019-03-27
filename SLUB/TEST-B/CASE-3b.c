/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 *
 * TEST B-3b: use corrupted freelist with freeing existing object
 */

#include <linux/slab.h>
#include "corrupter.h"
#include "STEP.h"

static struct corrupter_obj *obj0, *obj1, *obj2;

void corrupter_slab_doit(struct kmem_cache *cachep)
{
	obj0 = obj1 = obj2 = NULL;

	STEP_B1;
	STEP_B2;
	STEP_B3b;
}

int corrupter_slab_cleanup(struct kmem_cache *cachep)
{
        return -1;
}
