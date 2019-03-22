/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 */

#define STEP_02 \
		obj0 = kmem_cache_alloc(cachep, GFP_KERNEL);
#define STEP_03 \
		obj1 = kmem_cache_alloc(cachep, GFP_KERNEL);
#define STEP_04 \
		obj0->data[0] = 0xee;
#define STEP_05 \
		kmem_cache_free(cachep, obj0);
#define STEP_06 \
		kmem_cache_free(cachep, obj1);
#define STEP_07 \
		kmem_cache_destroy(cachep);
