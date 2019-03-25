# Test Cases and Steps
## TEST-0_CASE-1: after creating slab cache

0. create corrupter_slab_cache with a constructor which fills all objects with 0x88.

## TEST-0_CASE-2: after allocating 1st object

1. alloc obj0.

## TEST-0_CASE-3: after allocating 2nd object

2. alloc obj1.

## TEST-0_CASE-4: after updating contents of the 1st object

3. fill contents of obj0 with 0x11.

## TEST-0_CASE-5: after freeing the 1st object

4. free obj0.

## TEST-0_CASE-6: after freeing the 2nd object

5. free obj1.

## TEST-0_CASE-7: after destroying slab cache

6. destroy corrupter_slab_cache.

# Test Results
## Red Hat Enterprise Linux 7
### TEST-0_CASE-1: after creating slab cache

~~~
crash> kmem -S ffff91f1fcd25100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff91f1fcd25100 corrupter_slab_cache     504          0         0      0     4k
CPU 0 KMEM_CACHE_CPU:
  ffffd13bffc010a0
CPU 0 SLAB:
  (empty)
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff91f1fef8f840     0      0        0        0
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

- A `kmem_slab` for `corrupter_slab_cache` is allocated.
- No slab page is allocated.

### TEST-0_CASE-2: after allocating 1st object

~~~
crash> kmem -S ffff91f1fcd25100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff91f1fcd25100 corrupter_slab_cache     504          1         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffd13bffc010a0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  fffffe2b81f05880  ffff91f1fc162000     0      8          1     7
  FREE / [ALLOCATED]
  [ffff91f1fc162000]
   ffff91f1fc162200  (cpu 0 cache)
   ffff91f1fc162400  (cpu 0 cache)
   ffff91f1fc162600  (cpu 0 cache)
   ffff91f1fc162800  (cpu 0 cache)
   ffff91f1fc162a00  (cpu 0 cache)
   ffff91f1fc162c00  (cpu 0 cache)
   ffff91f1fc162e00  (cpu 0 cache)
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff91f1fef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffd13bffc010a0
  freelist = 0xffff91f1fc162200
~~~

~~~
crash> rd -x ffff91f1fc162000 512
ffff91f1fc162000:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1621f0:  8888888888888888 ffff91f1fc162200 
ffff91f1fc162200:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1623f0:  8888888888888888 ffff91f1fc162400 
ffff91f1fc162400:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1625f0:  8888888888888888 ffff91f1fc162600 
ffff91f1fc162600:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1627f0:  8888888888888888 ffff91f1fc162800 
ffff91f1fc162800:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1629f0:  8888888888888888 ffff91f1fc162a00 
ffff91f1fc162a00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162be0:  8888888888888888 8888888888888888 
ffff91f1fc162bf0:  8888888888888888 ffff91f1fc162c00 
[...]
ffff91f1fc162df0:  8888888888888888 ffff91f1fc162e00 
ffff91f1fc162e00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162ff0:  8888888888888888 0000000000000000 
~~~

- A slab page is allocated.
- 8 slab objects are deployed on the page.
- The 1st slab object is allocated.
- `freelist` points the 2nd slab object.
- Next free pointer (fp) is put after each object.

### TEST-0_CASE-3: after allocating 2nd object

~~~
crash> kmem -S ffff91f1fcd25100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff91f1fcd25100 corrupter_slab_cache     504          2         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffd13bffc010a0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  fffffe2b81f05880  ffff91f1fc162000     0      8          2     6
  FREE / [ALLOCATED]
  [ffff91f1fc162000]
  [ffff91f1fc162200]
   ffff91f1fc162400  (cpu 0 cache)
   ffff91f1fc162600  (cpu 0 cache)
   ffff91f1fc162800  (cpu 0 cache)
   ffff91f1fc162a00  (cpu 0 cache)
   ffff91f1fc162c00  (cpu 0 cache)
   ffff91f1fc162e00  (cpu 0 cache)
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff91f1fef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffd13bffc010a0
  freelist = 0xffff91f1fc162400
                            ~~~
~~~

~~~
crash> rd -x ffff91f1fc162000 512
ffff91f1fc162000:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1621f0:  8888888888888888 ffff91f1fc162200 
ffff91f1fc162200:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1623f0:  8888888888888888 ffff91f1fc162400 
ffff91f1fc162400:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1625f0:  8888888888888888 ffff91f1fc162600 
ffff91f1fc162600:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1627f0:  8888888888888888 ffff91f1fc162800 
ffff91f1fc162800:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1629f0:  8888888888888888 ffff91f1fc162a00 
ffff91f1fc162a00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162bf0:  8888888888888888 ffff91f1fc162c00 
ffff91f1fc162c00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162df0:  8888888888888888 ffff91f1fc162e00 
ffff91f1fc162e00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162ff0:  8888888888888888 0000000000000000 
~~~

- 2nd slab object is allocated.
- `freelist` ponts the 3rd free object.
- Allocated objects `fp` are still there.

### TEST-0_CASE-4: after updating contents of the 1st object

~~~
crash> kmem -S ffff91f1fcd25100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff91f1fcd25100 corrupter_slab_cache     504          2         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffd13bffc010a0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  fffffe2b81f05880  ffff91f1fc162000     0      8          2     6
  FREE / [ALLOCATED]
  [ffff91f1fc162000]
  [ffff91f1fc162200]
   ffff91f1fc162400  (cpu 0 cache)
   ffff91f1fc162600  (cpu 0 cache)
   ffff91f1fc162800  (cpu 0 cache)
   ffff91f1fc162a00  (cpu 0 cache)
   ffff91f1fc162c00  (cpu 0 cache)
   ffff91f1fc162e00  (cpu 0 cache)
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff91f1fef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffd13bffc010a0
  freelist = 0xffff91f1fc162400
~~~

~~~
crash> rd -x ffff91f1fc162000 512
ffff91f1fc162000:  88888888888888ee 8888888888888888 
[...]                            ~~
ffff91f1fc1621f0:  8888888888888888 ffff91f1fc162200 
ffff91f1fc162200:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1623f0:  8888888888888888 ffff91f1fc162400 
ffff91f1fc162400:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1625f0:  8888888888888888 ffff91f1fc162600 
ffff91f1fc162600:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1627f0:  8888888888888888 ffff91f1fc162800 
ffff91f1fc162800:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1629f0:  8888888888888888 ffff91f1fc162a00 
ffff91f1fc162a00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162bf0:  8888888888888888 ffff91f1fc162c00 
ffff91f1fc162c00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162df0:  8888888888888888 ffff91f1fc162e00 
ffff91f1fc162e00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162ff0:  8888888888888888 0000000000000000 
~~~

- The 1st object is updated.

### TEST-0_CASE-5: after freeing the 1st object

~~~
crash> kmem -S ffff91f1fcd25100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff91f1fcd25100 corrupter_slab_cache     504          1         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffd13bffc010a0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  fffffe2b81f05880  ffff91f1fc162000     0      8          1     7
  FREE / [ALLOCATED]
   ffff91f1fc162000  (cpu 0 cache)
  [ffff91f1fc162200]
   ffff91f1fc162400  (cpu 0 cache)
   ffff91f1fc162600  (cpu 0 cache)
   ffff91f1fc162800  (cpu 0 cache)
   ffff91f1fc162a00  (cpu 0 cache)
   ffff91f1fc162c00  (cpu 0 cache)
   ffff91f1fc162e00  (cpu 0 cache)
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff91f1fef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffd13bffc010a0
  freelist = 0xffff91f1fc162000
                            ~~~
~~~

~~~
crash> rd -x ffff91f1fc162000 512
ffff91f1fc162000:  88888888888888ee 8888888888888888 
[...]                            ~~
ffff91f1fc1621f0:  8888888888888888 ffff91f1fc162400 
                                                 ~~~
ffff91f1fc162200:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1623f0:  8888888888888888 ffff91f1fc162400 
ffff91f1fc162400:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1625f0:  8888888888888888 ffff91f1fc162600 
ffff91f1fc162600:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1627f0:  8888888888888888 ffff91f1fc162800 
ffff91f1fc162800:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1629f0:  8888888888888888 ffff91f1fc162a00 
ffff91f1fc162a00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162bf0:  8888888888888888 ffff91f1fc162c00 
ffff91f1fc162c00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162df0:  8888888888888888 ffff91f1fc162e00 
ffff91f1fc162e00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162ff0:  8888888888888888 0000000000000000 
~~~

- The 1st object is freed.
- `freelist` ponts the 1st object.
- The 1st object's `fp` is updated, and ponts 3rd object which is the last `freelist`.
- The updated contents are still there.

### TEST-0_CASE-6: after freeing the 2nd object

~~~
crash> kmem -S ffff91f1fcd25100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff91f1fcd25100 corrupter_slab_cache     504          0         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffd13bffc010a0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  fffffe2b81f05880  ffff91f1fc162000     0      8          0     8
  FREE / [ALLOCATED]
   ffff91f1fc162000  (cpu 0 cache)
   ffff91f1fc162200  (cpu 0 cache)
   ffff91f1fc162400  (cpu 0 cache)
   ffff91f1fc162600  (cpu 0 cache)
   ffff91f1fc162800  (cpu 0 cache)
   ffff91f1fc162a00  (cpu 0 cache)
   ffff91f1fc162c00  (cpu 0 cache)
   ffff91f1fc162e00  (cpu 0 cache)
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff91f1fef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffd13bffc010a0
  freelist = 0xffff91f1fc162200
                            ~~~
~~~

~~~
crash> rd -x ffff91f1fc162000 512
ffff91f1fc162000:  88888888888888ee 8888888888888888 
[...]
ffff91f1fc1621f0:  8888888888888888 ffff91f1fc162400 
ffff91f1fc162200:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1623f0:  8888888888888888 ffff91f1fc162000 
                                                 ~~~
ffff91f1fc162400:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1625f0:  8888888888888888 ffff91f1fc162600 
ffff91f1fc162600:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1627f0:  8888888888888888 ffff91f1fc162800 
ffff91f1fc162800:  8888888888888888 8888888888888888 
[...]
ffff91f1fc1629f0:  8888888888888888 ffff91f1fc162a00 
ffff91f1fc162a00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162bf0:  8888888888888888 ffff91f1fc162c00 
ffff91f1fc162c00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162df0:  8888888888888888 ffff91f1fc162e00 
ffff91f1fc162e00:  8888888888888888 8888888888888888 
[...]
ffff91f1fc162ff0:  8888888888888888 0000000000000000 
~~~

- The 2nd object is freed.
- `freelist` ponts the 2nd object.
- The 2st object's `fp` is updated, and ponts 1rd object which is the last `freelist`.
- Slab page is still there.

### TEST-0_CASE-7: after destroying slab cache

- The `slab_cache` and page are freed.
