/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 *
 * TEST 0-3: monitor right behavior in allocating the 2nd object
 */

#include <linux/slab.h>
#include "corrupter.h"
#include "STEP.h"

static struct corrupter_obj *obj0, *obj1, *obj2;

void corrupter_slab_doit(struct kmem_cache *cachep)
{
	obj0 = obj1 = obj2 = NULL;

	STEP_01
	STEP_02
}

int corrupter_slab_cleanup(struct kmem_cache *cachep)
{
	STEP_04
	STEP_05
        return 0;
}
