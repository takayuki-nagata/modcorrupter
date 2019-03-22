/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 *
 * TEST B-3a: use corrupted freelist with allocating new object
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
	STEP_B3a;
}

int corrupter_slab_cleanup(struct kmem_cache *cachep)
{
        return -1;
}
