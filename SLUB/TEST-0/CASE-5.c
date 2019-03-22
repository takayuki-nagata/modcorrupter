/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 *
 * TEST 0-5: monitor right behavior in freeing the 1st object
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
	STEP_03
	STEP_04
}

int corrupter_slab_cleanup(struct kmem_cache *cachep)
{
	STEP_05
        return 0;
}
