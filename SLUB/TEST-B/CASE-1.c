/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 *
 * TEST B-1: corrupt fp of free object with memory underrun
 */

#include <linux/slab.h>
#include "corrupter.h"
#include "STEP.h"

static struct corrupter_obj *obj0, *obj1, *obj2;

void corrupter_slab_doit(struct kmem_cache *cachep)
{
	obj0 = obj1 = obj2 = NULL;

	STEP_B1;
}

int corrupter_slab_cleanup(struct kmem_cache *cachep)
{
        return -1;
}
