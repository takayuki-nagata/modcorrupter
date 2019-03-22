/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 *
 * TEST A-2: corrupt freelist with allocating new object
 */

#include <linux/slab.h>
#include "corrupter.h"
#include "corrupter_TEST.h"

void corrupter_slab_doit(struct kmem_cache *cachep)
{
	struct corrupter_obj *obj0, *obj1, *obj2;
	obj0 = obj1 = obj2 = NULL;

	STEP_A1;
	STEP_A2;
}
