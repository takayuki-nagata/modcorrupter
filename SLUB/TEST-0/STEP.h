/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 */

#define STEP_01 \
		obj0 = kmem_cache_alloc(cachep, GFP_KERNEL);
#define STEP_02 \
		obj1 = kmem_cache_alloc(cachep, GFP_KERNEL);
#define STEP_03 \
		obj0->data[0] = 0xee;
#define STEP_04 \
		kmem_cache_free(cachep, obj0);
#define STEP_05 \
		kmem_cache_free(cachep, obj1);
#define STEP_06 \
		kmem_cache_destroy(cachep);
