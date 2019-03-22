/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 *
 * TEST 0-4: monitor right behavior in updating contents of 1st object
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
}

int corrupter_slab_cleanup(struct kmem_cache *cachep)
{
	STEP_04
	STEP_05
        return 0;
}
