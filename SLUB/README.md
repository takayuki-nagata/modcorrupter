# Test Cases and Steps
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

## A. corrupt with memory overrun

~~~
memory overrun
+-------------->
+----+---+----+---+----+---+-
|obj0|fp0|obj1|fp1|obj2|fp2|...
+----+---+----+---+----+---+-
~~~

### TEST A-1. corrupt fp of free object with memory overrun

1. alloc obj0.
2. corrupt fp1 with memory overrun using obj0.

Before:
~~~
freelist -> obj0 -> obj1 -> obj2 -> ...
~~~

~~~
 obj0|fp0  <- freelist
 obj1|fp1
 obj2|fp2
 ...
~~~

After:
~~~
freelist -> obj1 -> ??? (obj2) -> ...
~~~

~~~
[obj0|fp0]
 obj1|fp1  <- freelist
 obj2|fp2
 ...
~~~

#### TEST A-2. corrupt freelist with allocating new object

3. alloc obj1

After:
~~~
freelist -> ??? (obj2) -> ...
~~~

~~~
[obj0|fp0]
[obj1|fp1]
 obj2|fp2   ??? <- freelist
 ...
~~~

##### TEST A-3a. use corrupted freelist with allocating new object

4. alloc obj2

##### TEST A-3b. use corrupted freelist with freeing allocated object

4. free obj1

##### TEST A-3c. free all allocated objects, and destroy the slab cache

4. free obj1
5. free obj0
6. destory the slab cache

## B. corrupt with memory underrun

~~~
     underrun
       <-+
+----+---+----+---+----+---+-
|obj0|fp0|obj1|fp1|obj2|fp2|...
+----+---+----+---+----+---+-
~~~

### TEST B-1. corrupt fp of free object with memory underrun

1. alloc obj0
2. alloc obj1
3. free obj0
4. corrupt fp0 with memory underrun using obj1

Before:
~~~
freelist -> obj0 -> obj1 -> obj2 -> ...
~~~

~~~
 obj0|fp0  <- freelist
 obj1|fp1
 obj2|fp2
 ...
~~~

After:
~~~
freelist -> obj0 -> ??? (obj2) -> ...
~~~

~~~
 obj0|fp0   <- freelist
[obj1|fp1]
 obj2|fp2
 ...
~~~

#### TEST B-2. corrupt freelist with allocating new object

5. alloc obj0

After:
~~~
freelist -> ??? (obj2) -> ...
~~~

~~~
[obj0|fp0]
[obj1|fp1]
 obj2|fp2  ??? <- freelist
 ...
~~~

##### TEST B-3a. use corrupted freelist with allocating new object

6. alloc obj2

##### TEST B-3b. use corrupted freelist with freeing existing object

6. free obj1

##### TEST B-3c. free all allocated objects, and destory the slab cache

6. free obj1
7. free obj0
8. destory the slab cache

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

### TEST A-1

System Status: Operational

~~~
crash> kmem -S ffff9ef7bb8f0100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
kmem: corrupter_slab_cache: slab: fffff67a41f897c0 invalid freepointer: 11ff9ef7be25f5f8
ffff9ef7bb8f0100 corrupter_slab_cache     504          6         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffde397fc010b0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
kmem: corrupter_slab_cache: slab: fffff67a41f897c0 invalid freepointer: 11ff9ef7be25f5f8
  fffff67a41f897c0  ffff9ef7be25f000     0      8          6     2
  FREE / [ALLOCATED]
kmem: corrupter_slab_cache: slab: fffff67a41f897c0 invalid freepointer: 11ff9ef7be25f5f8
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff9ef7bef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffde397fc010b0
  freelist = 0xffff9ef7be25f200
~~~

~~~
crash> rd -x ffff9ef7be25f000 512
ffff9ef7be25f000:  8888888888888888 8888888888888888 
[...]
ffff9ef7be25f1f0:  8888888888888888 ffff9ef7be25f200 
ffff9ef7be25f200:  8888888888888888 8888888888888888 
[...]
ffff9ef7be25f3f0:  8888888888888888 11ff9ef7be25f400 
ffff9ef7be25f400:  8888888888888888 8888888888888888 
[...]
ffff9ef7be25f5f0:  8888888888888888 ffff9ef7be25f600 
ffff9ef7be25f600:  8888888888888888 8888888888888888 
[...]
~~~

### TEST A-2

System Status: Operational

~~~
crash> kmem -S ffff8aa23cdc2100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
kmem: corrupter_slab_cache: slab: ffffda3781f1a280 invalid freepointer: 11ff8aa23c68a5f8
ffff8aa23cdc2100 corrupter_slab_cache     504          7         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffbd5effc010a0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
kmem: corrupter_slab_cache: slab: ffffda3781f1a280 invalid freepointer: 11ff8aa23c68a5f8
  ffffda3781f1a280  ffff8aa23c68a000     0      8          7     1
  FREE / [ALLOCATED]
kmem: corrupter_slab_cache: slab: ffffda3781f1a280 invalid freepointer: 11ff8aa23c68a5f8
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff8aa23ef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffbd5effc010a0
  freelist = 0x11ff8aa23c68a400
~~~

~~~
crash> rd -x ffff8aa23c68a000 512
ffff8aa23c68a000:  8888888888888888 8888888888888888 
[...]
ffff8aa23c68a1f0:  8888888888888888 ffff8aa23c68a200 
ffff8aa23c68a200:  8888888888888888 8888888888888888 
[...]
ffff8aa23c68a3f0:  8888888888888888 11ff8aa23c68a400 
ffff8aa23c68a400:  8888888888888888 8888888888888888 
[...]
ffff8aa23c68a5f0:  8888888888888888 ffff8aa23c68a600 
ffff8aa23c68a600:  8888888888888888 8888888888888888 
[...]
~~~

### TEST A-3a

System Status: Panic

~~~
crash> log
[...]
[   47.084156] general protection fault: 0000 [#1] SMP 
[   47.084177] Modules linked in: kmod_corrupter_TEST_A_3a(POE+) ip6t_rpfilter ipt_REJECT nf_reject_ipv4 ip6t_REJE
CT nf_reject_ipv6 xt_conntrack ip_set nfnetlink ebtable_nat ebtable_broute bridge stp llc ip6table_nat nf_conntrack_ipv6 nf_defrag_ipv6 nf_nat_ipv6 ip6table_mangle ip6table_security ip6table_raw iptable_nat nf_conntrack_ipv4 nf_defrag_ipv4 nf_nat_ipv4 nf_nat nf_conntrack iptable_mangle iptable_security iptable_raw ebtable_filter ebtables ip6table_filter ip6_tables iptable_filter sunrpc iosf_mbi crc32_pclmul ppdev ghash_clmulni_intel aesni_intel lrw gf128mul glue_helper ablk_helper cryptd joydev pcspkr virtio_balloon sg parport_pc parport i2c_piix4 ip_tables xfs libcrc32c sr_mod cdrom virtio_blk virtio_console virtio_net ata_generic pata_acpi crct10dif_pclmul crct10dif_common crc32c_intel
[   47.084417]  qxl serio_raw drm_kms_helper syscopyarea sysfillrect sysimgblt fb_sys_fops ttm drm floppy ata_piix libata virtio_pci virtio_ring virtio drm_panel_orientation_quirks dm_mirror dm_region_hash dm_log dm_mod
[   47.084482] CPU: 0 PID: 4022 Comm: insmod Kdump: loaded Tainted: P           OE  ------------   3.10.0-957.el7.x86_64 #1
[   47.084508] Hardware name: QEMU Standard PC (i440FX + PIIX, 1996), BIOS ?-20180724_192412-buildhw-07.phx2.fedoraproject.org-1.fc29 04/01/2014
[   47.084538] task: ffff9ce8b66f8000 ti: ffff9ce8fe7c8000 task.ti: ffff9ce8fe7c8000
[   47.084555] RIP: 0010:[<ffffffffa561bcf4>]  [<ffffffffa561bcf4>] kmem_cache_alloc+0x74/0x1f0
[   47.084579] RSP: 0018:ffff9ce8fe7cbcd8  EFLAGS: 00010282
[   47.084592] RAX: 00000000000001f8 RBX: ffff9ce8fcca0100 RCX: 0000000000000003
[   47.084610] RDX: 0000000000000002 RSI: 00000000000000d0 RDI: ffff9ce8fcca0100
[   47.084627] RBP: ffff9ce8fe7cbd08 R08: 000034ae7d201070 R09: ffffffffc07c5068
[   47.084643] R10: 0000000000000002 R11: fffffffffffffff6 R12: 11ff9ce8fc7a9400
[   47.084660] R13: 00000000000000d0 R14: ffff9ce8fcca0100 R15: ffff9ce8fcca0100
[   47.084678] FS:  00007f19633e4740(0000) GS:ffff9ce902a00000(0000) knlGS:0000000000000000
[   47.084697] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[   47.084711] CR2: 000000000041d458 CR3: 000000007e4d0000 CR4: 00000000001606f0
[   47.084730] Call Trace:
[   47.084740]  [<ffffffffc07c5068>] ? corrupter_slab_doit+0x38/0x3b [kmod_corrupter_TEST_A_3a]
[   47.084761]  [<ffffffffc041d000>] ? 0xffffffffc041cfff
[   47.084775]  [<ffffffffc07c5068>] corrupter_slab_doit+0x38/0x3b [kmod_corrupter_TEST_A_3a]
[   47.084795]  [<ffffffffc041d037>] corrupter_init+0x37/0x1000 [kmod_corrupter_TEST_A_3a]
[   47.084838]  [<ffffffffa540210a>] do_one_initcall+0xba/0x240
[   47.084865]  [<ffffffffa5518f8c>] load_module+0x272c/0x2bc0
[   47.084880]  [<ffffffffa57a3030>] ? ddebug_proc_write+0x100/0x100
[   47.084895]  [<ffffffffa5514b13>] ? copy_module_from_fd.isra.44+0x53/0x150
[   47.084913]  [<ffffffffa5519606>] SyS_finit_module+0xa6/0xd0
[   47.084929]  [<ffffffffa5b74ddb>] system_call_fastpath+0x22/0x27
[   47.084943] Code: 54 9f 5a 49 8b 50 08 4d 8b 20 49 8b 40 10 4d 85 e4 0f 84 28 01 00 00 48 85 c0 0f 84 1f 01 00 00 49 63 46 20 48 8d 4a 01 4d 8b 06 <49> 8b 1c 04 4c 89 e0 65 49 0f c7 08 0f 94 c0 84 c0 74 ba 49 63 
[   47.085055] RIP  [<ffffffffa561bcf4>] kmem_cache_alloc+0x74/0x1f0
[   47.085072]  RSP <ffff9ce8fe7cbcd8>
~~~

~~~
crash> bt
PID: 4022   TASK: ffff9ce8b66f8000  CPU: 0   COMMAND: "insmod"
 #0 [ffff9ce8fe7cba48] machine_kexec at ffffffffa5463674
 #1 [ffff9ce8fe7cbaa8] __crash_kexec at ffffffffa551ce12
 #2 [ffff9ce8fe7cbb78] crash_kexec at ffffffffa551cf00
 #3 [ffff9ce8fe7cbb90] oops_end at ffffffffa5b6c758
 #4 [ffff9ce8fe7cbbb8] die at ffffffffa542f95b
 #5 [ffff9ce8fe7cbbe8] do_general_protection at ffffffffa5b6c052
 #6 [ffff9ce8fe7cbc20] general_protection at ffffffffa5b6b6f8
    [exception RIP: kmem_cache_alloc+116]
    RIP: ffffffffa561bcf4  RSP: ffff9ce8fe7cbcd8  RFLAGS: 00010282
    RAX: 00000000000001f8  RBX: ffff9ce8fcca0100  RCX: 0000000000000003
    RDX: 0000000000000002  RSI: 00000000000000d0  RDI: ffff9ce8fcca0100
    RBP: ffff9ce8fe7cbd08   R8: 000034ae7d201070   R9: ffffffffc07c5068
    R10: 0000000000000002  R11: fffffffffffffff6  R12: 11ff9ce8fc7a9400
    R13: 00000000000000d0  R14: ffff9ce8fcca0100  R15: ffff9ce8fcca0100
    ORIG_RAX: ffffffffffffffff  CS: 0010  SS: 0018
 #7 [ffff9ce8fe7cbd10] corrupter_slab_doit at ffffffffc07c5068 [kmod_corrupter_TEST_A_3a]
 #8 [ffff9ce8fe7cbd28] init_module at ffffffffc041d037 [kmod_corrupter_TEST_A_3a]
 #9 [ffff9ce8fe7cbd38] do_one_initcall at ffffffffa540210a
#10 [ffff9ce8fe7cbd68] load_module at ffffffffa5518f8c
#11 [ffff9ce8fe7cbeb8] sys_finit_module at ffffffffa5519606
#12 [ffff9ce8fe7cbf50] system_call_fastpath at ffffffffa5b74ddb
    RIP: 00007f19628a91c9  RSP: 00007ffd00e8ad38  RFLAGS: 00010203
    RAX: 0000000000000139  RBX: 0000000000ad82b0  RCX: 00007f196291afd0
    RDX: 0000000000000000  RSI: 000000000041a94e  RDI: 0000000000000003
    RBP: 000000000041a94e   R8: 0000000000000000   R9: 00007ffd00e8af48
    R10: 0000000000000003  R11: 0000000000000202  R12: 0000000000000000
    R13: 0000000000ad8270  R14: 0000000000000000  R15: 0000000000000000
    ORIG_RAX: 0000000000000139  CS: 0033  SS: 002b
~~~

~~~
crash> dis -l kmem_cache_alloc+116
/usr/src/debug/kernel-3.10.0-957.el7/linux-3.10.0-957.el7.x86_64/mm/slub.c: 241
0xffffffffa561bcf4 <kmem_cache_alloc+116>:      mov    (%r12,%rax,1),%rbx
~~~

~~~
crash> kmem -S ffff9ce8fcca0100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
kmem: corrupter_slab_cache: slab: ffffef2f01f1ea40 invalid freepointer: 11ff9ce8fc7a95f8
ffff9ce8fcca0100 corrupter_slab_cache     504          7         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffd1977fc01070
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
kmem: corrupter_slab_cache: slab: ffffef2f01f1ea40 invalid freepointer: 11ff9ce8fc7a95f8
  ffffef2f01f1ea40  ffff9ce8fc7a9000     0      8          7     1
  FREE / [ALLOCATED]
kmem: corrupter_slab_cache: slab: ffffef2f01f1ea40 invalid freepointer: 11ff9ce8fc7a95f8
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff9ce8fef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffd1977fc01070
  freelist = 0x11ff9ce8fc7a9400
~~~

~~~
crash> rd -x ffff9ce8fc7a9000 512
ffff9ce8fc7a9000:  8888888888888888 8888888888888888 
[...]
ffff9ce8fc7a91f0:  8888888888888888 ffff9ce8fc7a9200 
ffff9ce8fc7a9200:  8888888888888888 8888888888888888 
[...]
ffff9ce8fc7a93e0:  8888888888888888 8888888888888888 
ffff9ce8fc7a93f0:  8888888888888888 11ff9ce8fc7a9400 
[...]
ffff9ce8fc7a95f0:  8888888888888888 ffff9ce8fc7a9600 
ffff9ce8fc7a9600:  8888888888888888 8888888888888888 
[...]
~~~

### TEST A-3b

System Status: Operational

~~~
crash> kmem -S ffff8c3bfcdbf100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
kmem: corrupter_slab_cache: slab: ffffe2ab8155d580 invalid freepointer: 11ff8c3bd57565f8
ffff8c3bfcdbf100 corrupter_slab_cache     504          6         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffcebbbfc01080
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
kmem: corrupter_slab_cache: slab: ffffe2ab8155d580 invalid freepointer: 11ff8c3bd57565f8
  ffffe2ab8155d580  ffff8c3bd5756000     0      8          6     2
  FREE / [ALLOCATED]
kmem: corrupter_slab_cache: slab: ffffe2ab8155d580 invalid freepointer: 11ff8c3bd57565f8
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff8c3bfef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffcebbbfc01080
  freelist = 0xffff8c3bd5756200
~~~

~~~
crash> rd -x ffff8c3bd5756000 512
ffff8c3bd5756000:  8888888888888888 8888888888888888 
[...]
ffff8c3bd57561f0:  8888888888888888 ffff8c3bd5756200 
ffff8c3bd5756200:  8888888888888888 8888888888888888 
[...]
ffff8c3bd57563f0:  8888888888888888 11ff8c3bd5756400 
ffff8c3bd5756400:  8888888888888888 8888888888888888 
[...]
ffff8c3bd57565f0:  8888888888888888 ffff8c3bd5756600 
ffff8c3bd5756600:  8888888888888888 8888888888888888 
[...]
~~~

### TEST A-3c

System Status: Panic

~~~
crash> log
[...]
[  427.657337] general protection fault: 0000 [#1] SMP 
[  427.657359] Modules linked in: kmod_corrupter_TEST_A_3c(POE+) macsec tcp_diag udp_diag inet_diag unix_diag af_packet_diag netlink_diag binfmt_misc ip6t_rpfilter ipt_REJECT nf_reject_ipv4 ip6t_REJECT nf_reject_ipv6 xt_conntrack ip_set nfnetlink ebtable_nat ebtable_broute bridge stp llc ip6table_nat nf_conntrack_ipv6 nf_defrag_ipv6 nf_nat_ipv6 ip6table_mangle ip6table_security ip6table_raw iptable_nat nf_conntrack_ipv4 nf_defrag_ipv4 nf_nat_ipv4 nf_nat nf_conntrack iptable_mangle iptable_security iptable_raw ebtable_filter ebtables ip6table_filter ip6_tables iptable_filter sunrpc iosf_mbi crc32_pclmul ppdev ghash_clmulni_intel aesni_intel lrw gf128mul glue_helper ablk_helper cryptd joydev pcspkr virtio_balloon sg parport_pc parport i2c_piix4 ip_tables xfs libcrc32c sr_mod cdrom virtio_blk virtio_console
[  427.657604]  virtio_net ata_generic pata_acpi crct10dif_pclmul crct10dif_common crc32c_intel qxl drm_kms_helper serio_raw syscopyarea sysfillrect sysimgblt fb_sys_fops ttm ata_piix floppy drm libata drm_panel_orientation_quirks virtio_pci virtio_ring virtio dm_mirror dm_region_hash dm_log dm_mod
[  427.657693] CPU: 0 PID: 5934 Comm: insmod Kdump: loaded Tainted: P           OE  ------------   3.10.0-957.el7.x86_64 #1
[  427.657718] Hardware name: QEMU Standard PC (i440FX + PIIX, 1996), BIOS ?-20180724_192412-buildhw-07.phx2.fedoraproject.org-1.fc29 04/01/2014
[  427.657748] task: ffff96a49b766180 ti: ffff96a476638000 task.ti: ffff96a476638000
[  427.657766] RIP: 0010:[<ffffffff9c819fe4>]  [<ffffffff9c819fe4>] deactivate_slab+0xa4/0x3c0
[  427.657790] RSP: 0018:ffff96a47663bb10  EFLAGS: 00010006
[  427.657803] RAX: 00000000000001f8 RBX: ffffeceac1589180 RCX: 0000000180080006
[  427.657820] RDX: ffff96a496246200 RSI: ffffeceac1589180 RDI: 0000000040020000
[  427.657837] RBP: ffff96a47663bbb8 R08: ffff96a496246200 R09: 0000000180080006
[  427.657854] R10: ffff96a4c2401500 R11: ffff96a476335400 R12: ffff96a496246200
[  427.657871] R13: 11ff96a496246400 R14: ffff96a4bef8f840 R15: ffff96a4bcf39100
[  427.657888] FS:  00007f465bc4f740(0000) GS:ffff96a4c2a00000(0000) knlGS:0000000000000000
[  427.657908] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[  427.657922] CR2: 000000000041d458 CR3: 0000000035ce0000 CR4: 00000000001606f0
[  427.657941] Call Trace:
[  427.657952]  [<ffffffff9c975465>] ? cpumask_next_and+0x35/0x50
[  427.657968]  [<ffffffff9c81a386>] flush_cpu_slab+0x36/0x60
[  427.657982]  [<ffffffff9c81a350>] ? __flush_cpu_slab+0x50/0x50
[  427.657998]  [<ffffffff9c7118ae>] on_each_cpu_mask+0x4e/0x70
[  427.658013]  [<ffffffff9c817400>] ? check_valid_pointer+0x80/0x80
[  427.658028]  [<ffffffff9c711994>] on_each_cpu_cond+0xc4/0x190
[  427.658044]  [<ffffffff9c81a350>] ? __flush_cpu_slab+0x50/0x50
[  427.658062]  [<ffffffffc00f4000>] ? 0xffffffffc00f3fff
[  427.658076]  [<ffffffff9c81e5a5>] kmem_cache_close+0x35/0x300
[  427.658092]  [<ffffffffc056e049>] ? corrupter_slab_doit+0x19/0x53 [kmod_corrupter_TEST_A_3c]
[  427.658114]  [<ffffffffc00f4000>] ? 0xffffffffc00f3fff
[  427.658128]  [<ffffffff9c81eb34>] __kmem_cache_shutdown+0x14/0x80
[  427.658144]  [<ffffffff9c7db8f0>] kmem_cache_destroy+0x60/0x130
[  427.658160]  [<ffffffffc056e07e>] corrupter_slab_doit+0x4e/0x53 [kmod_corrupter_TEST_A_3c]
[  427.658208]  [<ffffffffc00f4037>] corrupter_init+0x37/0x1000 [kmod_corrupter_TEST_A_3c]
[  427.658239]  [<ffffffff9c60210a>] do_one_initcall+0xba/0x240
[  427.658254]  [<ffffffff9c718f8c>] load_module+0x272c/0x2bc0
[  427.658270]  [<ffffffff9c9a3030>] ? ddebug_proc_write+0x100/0x100
[  427.658286]  [<ffffffff9c714b13>] ? copy_module_from_fd.isra.44+0x53/0x150
[  427.658932]  [<ffffffff9c719606>] SyS_finit_module+0xa6/0xd0
[  427.659606]  [<ffffffff9cd74ddb>] system_call_fastpath+0x22/0x27
[  427.660270] Code: de 48 89 4c 24 58 66 83 6c 24 58 01 4c 8b 4c 24 58 e8 31 f4 ff ff 84 c0 74 cd 4d 85 ed 0f 84 a6 02 00 00 49 63 47 20 48 8b 53 10 <49> 8b 4c 05 00 48 85 c9 74 0b 4d 89 ec 49 89 cd eb b2 4d 89 e5 
[  427.661745] RIP  [<ffffffff9c819fe4>] deactivate_slab+0xa4/0x3c0
[  427.662431]  RSP <ffff96a47663bb10>
~~~

~~~
crash> bt
PID: 5934   TASK: ffff96a49b766180  CPU: 0   COMMAND: "insmod"
 #0 [ffff96a47663b888] machine_kexec at ffffffff9c663674
 #1 [ffff96a47663b8e8] __crash_kexec at ffffffff9c71ce12
 #2 [ffff96a47663b9b8] crash_kexec at ffffffff9c71cf00
 #3 [ffff96a47663b9d0] oops_end at ffffffff9cd6c758
 #4 [ffff96a47663b9f8] die at ffffffff9c62f95b
 #5 [ffff96a47663ba28] do_general_protection at ffffffff9cd6c052
 #6 [ffff96a47663ba60] general_protection at ffffffff9cd6b6f8
    [exception RIP: deactivate_slab+164]
    RIP: ffffffff9c819fe4  RSP: ffff96a47663bb10  RFLAGS: 00010006
    RAX: 00000000000001f8  RBX: ffffeceac1589180  RCX: 0000000180080006
    RDX: ffff96a496246200  RSI: ffffeceac1589180  RDI: 0000000040020000
    RBP: ffff96a47663bbb8   R8: ffff96a496246200   R9: 0000000180080006
    R10: ffff96a4c2401500  R11: ffff96a476335400  R12: ffff96a496246200
    R13: 11ff96a496246400  R14: ffff96a4bef8f840  R15: ffff96a4bcf39100
    ORIG_RAX: ffffffffffffffff  CS: 0010  SS: 0018
 #7 [ffff96a47663bbc0] flush_cpu_slab at ffffffff9c81a386
 #8 [ffff96a47663bbe0] on_each_cpu_mask at ffffffff9c7118ae
 #9 [ffff96a47663bc10] on_each_cpu_cond at ffffffff9c711994
#10 [ffff96a47663bc68] kmem_cache_close at ffffffff9c81e5a5
#11 [ffff96a47663bcc8] __kmem_cache_shutdown at ffffffff9c81eb34
#12 [ffff96a47663bce8] kmem_cache_destroy at ffffffff9c7db8f0
#13 [ffff96a47663bd08] corrupter_slab_doit at ffffffffc056e07e [kmod_corrupter_TEST_A_3c]
#14 [ffff96a47663bd28] init_module at ffffffffc00f4037 [kmod_corrupter_TEST_A_3c]
#15 [ffff96a47663bd38] do_one_initcall at ffffffff9c60210a
#16 [ffff96a47663bd68] load_module at ffffffff9c718f8c
#17 [ffff96a47663beb8] sys_finit_module at ffffffff9c719606
#18 [ffff96a47663bf50] system_call_fastpath at ffffffff9cd74ddb
    RIP: 00007f465b1141c9  RSP: 00007ffdc825b2f8  RFLAGS: 00010203
    RAX: 0000000000000139  RBX: 00000000015972b0  RCX: 00007f465b185fd0
    RDX: 0000000000000000  RSI: 000000000041a94e  RDI: 0000000000000003
    RBP: 000000000041a94e   R8: 0000000000000000   R9: 00007ffdc825b508
    R10: 0000000000000003  R11: 0000000000000202  R12: 0000000000000000
    R13: 0000000001597270  R14: 0000000000000000  R15: 0000000000000000
    ORIG_RAX: 0000000000000139  CS: 0033  SS: 002b
~~~

~~~
crash> dis -l deactivate_slab+164
/usr/src/debug/kernel-3.10.0-957.el7/linux-3.10.0-957.el7.x86_64/mm/slub.c: 241
0xffffffff9c819fe4 <deactivate_slab+164>:       mov    0x0(%r13,%rax,1),%rcx
~~~

~~~
crash> kmem -s | head -n3
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff96a4bcf39000 nf_conntrack_ffffffff9d311640 312    20        48      4     4k
ffff96a47647ff00 rpc_inode_cache          624         13        24      2     8k
~~~

~~~
crash> struct kmem_cache.cpu_slab ffff96a4bcf39100
  cpu_slab = 0x2b777d201070
~~~

~~~
crash> struct kmem_cache_cpu.freelist 0x2b777d201070:0
[0]: ffffc21c3fc01070
  freelist = 0xffff96a496246000
~~~

~~~
crash> rd -x 0xffff96a496246000 512
ffff96a496246000:  8888888888888888 8888888888888888 
[...]
ffff96a4962461f0:  8888888888888888 0000000000000000 
ffff96a496246200:  8888888888888888 8888888888888888 
[...]
ffff96a4962463f0:  8888888888888888 ffff96a496246000 
ffff96a496246400:  8888888888888888 8888888888888888 
[...]
ffff96a4962465f0:  8888888888888888 ffff96a496246600 
ffff96a496246600:  8888888888888888 8888888888888888 
[...]
~~~

## Red Hat Enterprise Linux 7 with slub_debug=F
