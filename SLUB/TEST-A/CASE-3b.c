/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 *
 * TEST A-3b: use corrupted freelist with freeing allocated object
 */

#include <linux/slab.h>
#include "corrupter.h"
#include "STEP.h"

void corrupter_slab_doit(struct kmem_cache *cachep)
{
	struct corrupter_obj *obj0, *obj1, *obj2;
	obj0 = obj1 = obj2 = NULL;

	STEP_A1;
	STEP_A2;
	STEP_A3b;
}
