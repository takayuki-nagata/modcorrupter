/* For testing memory corruption.
 * Copyright (c) 2019 Takayuki Nagata
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>
#include "corrupter.h"

static struct kmem_cache *corrupter_slab_cachep;
static char pad = 0;

module_param(pad, byte, 0);
MODULE_PARM_DESC(pad, "Fill objects with pad in the constuctor");

void corrupter_slab_constructor(void *p)
{
	struct corrupter_obj *obj = p;
	int i;

	for(i = 0; i < DATA_SIZE; i++){
		obj->data[i] = pad;
	}
}

static int __init corrupter_init(void)
{
	corrupter_slab_cachep = kmem_cache_create("corrupter_slab_cache",
						sizeof(struct corrupter_obj),
						0, (SLAB_RECLAIM_ACCOUNT|
						   SLAB_MEM_SPREAD),
						pad ? corrupter_slab_constructor: NULL);

	if(corrupter_slab_cachep == NULL)
		return -ENOMEM;

	corrupter_slab_doit(corrupter_slab_cachep);

	return 0;
}

static void __exit corrupter_exit(void)
{
	if(!corrupter_slab_cleanup(corrupter_slab_cachep))
		kmem_cache_destroy(corrupter_slab_cachep);
	return;
}

module_init(corrupter_init);
module_exit(corrupter_exit);
