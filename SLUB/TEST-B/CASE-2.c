/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 *
 * TEST B-2: corrupt freelist with allocating new object
 */

#include <linux/slab.h>
#include "corrupter.h"
#include "STEP.h"

void corrupter_slab_doit(struct kmem_cache *cachep)
{
	struct corrupter_obj *obj0, *obj1, *obj2;
	obj0 = obj1 = obj2 = NULL;

	STEP_B1;
	STEP_B2;
}
