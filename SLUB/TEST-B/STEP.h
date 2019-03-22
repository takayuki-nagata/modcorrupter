/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 */

#define STEP_B1 \
		obj0 = kmem_cache_alloc(cachep, GFP_KERNEL); \
		obj1 = kmem_cache_alloc(cachep, GFP_KERNEL); \
		kmem_cache_free(cachep, obj0); \
		obj1->data[-1] = 0x11;
#define STEP_B2 \
		obj0 = kmem_cache_alloc(cachep, GFP_KERNEL);
#define STEP_B3a \
		obj2 = kmem_cache_alloc(cachep, GFP_KERNEL);
#define STEP_B3c \
		kmem_cache_free(cachep, obj1); \
		kmem_cache_free(cachep, obj0); \
		kmem_cache_destroy(cachep);
