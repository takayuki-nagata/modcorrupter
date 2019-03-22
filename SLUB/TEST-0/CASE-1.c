/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 *
 * TEST 0-1: monitor right behavior in creating slab cache
 */

#include <linux/slab.h>
#include "corrupter.h"
#include "STEP.h"

static struct corrupter_obj *obj0, *obj1, *obj2;

void corrupter_slab_doit(struct kmem_cache *cachep)
{
	obj0 = obj1 = obj2 = NULL;
	/* corrupter_slab_cache is already created. */
}

int corrupter_slab_cleanup(struct kmem_cache *cachep)
{
	return 0;
}
