# Test Steps
## 0. monitor right behavior
### TEST 0-1. create slab cache

0. create corrupter_slab_cache with a constructor which fills all objects with 0x88.

### TEST 0-2. alloc 1st object

1. alloc obj0.

### TEST 0-3. alloc 2nd object

2. alloc obj1.

### TEST 0-4. update contents of the 1st object

3. update contents of obj0.

### TEST 0-5. free the 1st object

4. free obj0.

### TEST 0-6. free all objects

5. free obj1.

### TEST 0-7. destroy slab cache

6. destrocy slab cache.

# Test Results
## Red Hat Enterprise Linux 7
### TEST 0-1

~~~
crash> kmem -S ffff9b19fe7b2100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff9b19fe7b2100 corrupter_slab_cache     504          0         0      0     4k
CPU 0 KMEM_CACHE_CPU:
  ffffcb427fc01080
CPU 0 SLAB:
  (empty)
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff9b19fef8f840     0      0        0        0
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffcb427fc01080
  freelist = 0x0
~~~

### TEST 0-2

~~~
crash> kmem -S ffff9b19fe7b2200
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff9b19fe7b2200 corrupter_slab_cache     504          1         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffcb427fc010b0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  fffffc6c41f09ec0  ffff9b19fc27b000     0      8          1     7
  FREE / [ALLOCATED]
  [ffff9b19fc27b000]
   ffff9b19fc27b200  (cpu 0 cache)
   ffff9b19fc27b400  (cpu 0 cache)
   ffff9b19fc27b600  (cpu 0 cache)
   ffff9b19fc27b800  (cpu 0 cache)
   ffff9b19fc27ba00  (cpu 0 cache)
   ffff9b19fc27bc00  (cpu 0 cache)
   ffff9b19fc27be00  (cpu 0 cache)
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff9b19fef8f880     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffcb427fc010b0
  freelist = 0xffff9b19fc27b200
~~~

~~~
crash> rd -x ffff9b19fc27b000 512
ffff9b19fc27b000:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27b1f0:  8888888888888888 ffff9b19fc27b200 
ffff9b19fc27b200:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27b3f0:  8888888888888888 ffff9b19fc27b400 
ffff9b19fc27b400:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27b5f0:  8888888888888888 ffff9b19fc27b600 
ffff9b19fc27b600:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27b7f0:  8888888888888888 ffff9b19fc27b800 
ffff9b19fc27b800:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27b9f0:  8888888888888888 ffff9b19fc27ba00 
ffff9b19fc27ba00:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27bbf0:  8888888888888888 ffff9b19fc27bc00 
ffff9b19fc27bc00:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27bdf0:  8888888888888888 ffff9b19fc27be00 
ffff9b19fc27be00:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27bff0:  8888888888888888 0000000000000000 
~~~

### TEST 0-3

~~~
crash> kmem -S ffff9b19fe7b2300
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff9b19fe7b2300 corrupter_slab_cache     504          0         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffcb427fc010e0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  fffffc6c41f09f80  ffff9b19fc27e000     0      8          0     8
  FREE / [ALLOCATED]
   ffff9b19fc27e000  (cpu 0 cache)
   ffff9b19fc27e200  (cpu 0 cache)
   ffff9b19fc27e400  (cpu 0 cache)
   ffff9b19fc27e600  (cpu 0 cache)
   ffff9b19fc27e800  (cpu 0 cache)
   ffff9b19fc27ea00  (cpu 0 cache)
   ffff9b19fc27ec00  (cpu 0 cache)
   ffff9b19fc27ee00  (cpu 0 cache)
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff9b19fef8f8c0     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffcb427fc010e0
  freelist = 0xffff9b19fc27e000
~~~

~~~
crash> rd -x ffff9b19fc27e000 512
ffff9b19fc27e000:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27e1f0:  8888888888888888 ffff9b19fc27e200 
ffff9b19fc27e200:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27e3f0:  8888888888888888 ffff9b19fc27e400 
ffff9b19fc27e400:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27e5f0:  8888888888888888 ffff9b19fc27e600 
ffff9b19fc27e600:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27e7f0:  8888888888888888 ffff9b19fc27e800 
ffff9b19fc27e800:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27e9f0:  8888888888888888 ffff9b19fc27ea00 
ffff9b19fc27ea00:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27ebf0:  8888888888888888 ffff9b19fc27ec00 
ffff9b19fc27ec00:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27edf0:  8888888888888888 ffff9b19fc27ee00 
ffff9b19fc27ee00:  8888888888888888 8888888888888888 
[...]
ffff9b19fc27eff0:  8888888888888888 0000000000000000 
~~~
