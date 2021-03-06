/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 */

#define STEP_A1 \
		obj0 = kmem_cache_alloc(cachep, GFP_KERNEL); \
		{ \
			int i; \
			for(i = 0; i < OBJ_SIZE; i++){ \
				obj0->data[2*i] = 0x11; \
			} \
		}
#define STEP_A2 \
		obj1 = kmem_cache_alloc(cachep, GFP_KERNEL); 
#define STEP_A3a \
		obj2 = kmem_cache_alloc(cachep, GFP_KERNEL);
#define STEP_A3b \
		kmem_cache_free(cachep, obj1);
#define STEP_A3c \
		kmem_cache_free(cachep, obj1); \
		kmem_cache_free(cachep, obj0); \
		kmem_cache_destroy(cachep);
