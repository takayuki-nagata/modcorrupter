#ifndef _CORRUPTER_H
#define _CORRUPTER_H

#include <linux/slab.h>

/* objsize=512, objperslab=8, pagesperslab=1 */
#define DATA_SIZE 504
#define OBJ_SIZE 512

struct corrupter_obj
{
        char data[DATA_SIZE];
};

extern void corrupter_slab_doit(struct kmem_cache* cachep);

#endif /* _CORRUPTER_H */
