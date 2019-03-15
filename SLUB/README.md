# Test Cases and Steps
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

##### TEST A-3c. free all allocated objects, and unload module

4. free obj1
5. free obj0
6. unload module

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

##### TEST B-3c. free all allocated objects, and unload module

6. free obj1
7. free obj0
8. rmmod

# Test Results
## Red Hat Enterprise Linux 7
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
[  153.093119] general protection fault: 0000 [#1] SMP 
[  153.093142] Modules linked in: kmod_corrupter_TEST_A_3c(POE-) macsec tcp_diag udp_diag inet_diag unix_diag af_packet_diag netlink_diag binfmt_misc ip6t_rpfilter ipt_REJECT nf_reject_ipv4 ip6t_REJECT nf_reject_ipv6 xt_conntrack ip_set nfnetlink ebtable_nat ebtable_broute bridge stp llc ip6table_nat nf_conntrack_ipv6 nf_defrag_ipv6 nf_nat_ipv6 ip6table_mangle ip6table_security ip6table_raw iptable_nat nf_conntrack_ipv4 nf_defrag_ipv4 nf_nat_ipv4 nf_nat nf_conntrack iptable_mangle iptable_security iptable_raw ebtable_filter ebtables ip6table_filter ip6_tables iptable_filter sunrpc iosf_mbi crc32_pclmul ppdev ghash_clmulni_intel aesni_intel lrw gf128mul glue_helper ablk_helper cryptd joydev pcspkr virtio_balloon sg parport_pc parport i2c_piix4 ip_tables xfs libcrc32c sr_mod cdrom virtio_blk virtio_console
[  153.093416]  virtio_net ata_generic pata_acpi crct10dif_pclmul crct10dif_common crc32c_intel qxl drm_kms_helper serio_raw syscopyarea sysfillrect sysimgblt fb_sys_fops ttm drm floppy ata_piix libata virtio_pci virtio_ring virtio drm_panel_orientation_quirks dm_mirror dm_region_hash dm_log dm_mod
[  153.093507] CPU: 0 PID: 5679 Comm: rmmod Kdump: loaded Tainted: P           OE  ------------   3.10.0-957.el7.x86_64 #1
[  153.093532] Hardware name: QEMU Standard PC (i440FX + PIIX, 1996), BIOS ?-20180724_192412-buildhw-07.phx2.fedoraproject.org-1.fc29 04/01/2014
[  153.093562] task: ffff9edd757a2080 ti: ffff9eddbbc98000 task.ti: ffff9eddbbc98000
[  153.093580] RIP: 0010:[<ffffffffa7019fe4>]  [<ffffffffa7019fe4>] deactivate_slab+0xa4/0x3c0
[  153.093604] RSP: 0018:ffff9eddbbc9bcc0  EFLAGS: 00010006
[  153.093617] RAX: 00000000000001f8 RBX: fffffede0152c1c0 RCX: 0000000180080006
[  153.093634] RDX: ffff9edd94b07200 RSI: fffffede0152c1c0 RDI: 0000000040020000
[  153.093661] RBP: ffff9eddbbc9bd68 R08: ffff9edd94b07200 R09: 0000000180080006
[  153.093678] R10: ffff9eddc2401500 R11: ffffffffa717551f R12: ffff9edd94b07200
[  153.093695] R13: 11ff9edd94b07400 R14: ffff9eddbef8f840 R15: ffff9eddbcce7100
[  153.093723] FS:  00007f01919ca740(0000) GS:ffff9eddc2a00000(0000) knlGS:0000000000000000
[  153.093742] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[  153.093756] CR2: 00007f0190e95390 CR3: 000000007caf0000 CR4: 00000000001606f0
[  153.093777] Call Trace:
[  153.093787]  [<ffffffffa6f2fd00>] ? cpu_stop_should_run+0x50/0x50
[  153.093803]  [<ffffffffa7175465>] ? cpumask_next_and+0x35/0x50
[  153.093820]  [<ffffffffa701a386>] flush_cpu_slab+0x36/0x60
[  153.093835]  [<ffffffffa701a350>] ? __flush_cpu_slab+0x50/0x50
[  153.093851]  [<ffffffffa6f118ae>] on_each_cpu_mask+0x4e/0x70
[  153.093866]  [<ffffffffa7017400>] ? check_valid_pointer+0x80/0x80
[  153.093881]  [<ffffffffa6f11994>] on_each_cpu_cond+0xc4/0x190
[  153.093896]  [<ffffffffa701a350>] ? __flush_cpu_slab+0x50/0x50
[  153.093912]  [<ffffffffa701e5a5>] kmem_cache_close+0x35/0x300
[  153.093927]  [<ffffffffa6f3049f>] ? stop_cpus+0x3f/0x50
[  153.093940]  [<ffffffffa6f13a30>] ? module_refcount+0xc0/0xc0
[  153.093955]  [<ffffffffa701eb34>] __kmem_cache_shutdown+0x14/0x80
[  153.093972]  [<ffffffffa6fdb8f0>] kmem_cache_destroy+0x60/0x130
[  153.093989]  [<ffffffffc066b08b>] corrupter_exit+0x10/0xf85 [kmod_corrupter_TEST_A_3c]
[  153.094008]  [<ffffffffa6f165de>] SyS_delete_module+0x19e/0x310
[  153.094025]  [<ffffffffa7574ddb>] system_call_fastpath+0x22/0x27
[  153.094039] Code: de 48 89 4c 24 58 66 83 6c 24 58 01 4c 8b 4c 24 58 e8 31 f4 ff ff 84 c0 74 cd 4d 85 ed 0f 84 a6 02 00 00 49 63 47 20 48 8b 53 10 <49> 8b 4c 05 00 48 85 c9 74 0b 4d 89 ec 49 89 cd eb b2 4d 89 e5 
[  153.094152] RIP  [<ffffffffa7019fe4>] deactivate_slab+0xa4/0x3c0
[  153.094869]  RSP <ffff9eddbbc9bcc0>
~~~

~~~
crash> bt
PID: 5679   TASK: ffff9edd757a2080  CPU: 0   COMMAND: "rmmod"
 #0 [ffff9eddbbc9ba38] machine_kexec at ffffffffa6e63674
 #1 [ffff9eddbbc9ba98] __crash_kexec at ffffffffa6f1ce12
 #2 [ffff9eddbbc9bb68] crash_kexec at ffffffffa6f1cf00
 #3 [ffff9eddbbc9bb80] oops_end at ffffffffa756c758
 #4 [ffff9eddbbc9bba8] die at ffffffffa6e2f95b
 #5 [ffff9eddbbc9bbd8] do_general_protection at ffffffffa756c052
 #6 [ffff9eddbbc9bc10] general_protection at ffffffffa756b6f8
    [exception RIP: deactivate_slab+164]
    RIP: ffffffffa7019fe4  RSP: ffff9eddbbc9bcc0  RFLAGS: 00010006
    RAX: 00000000000001f8  RBX: fffffede0152c1c0  RCX: 0000000180080006
    RDX: ffff9edd94b07200  RSI: fffffede0152c1c0  RDI: 0000000040020000
    RBP: ffff9eddbbc9bd68   R8: ffff9edd94b07200   R9: 0000000180080006
    R10: ffff9eddc2401500  R11: ffffffffa717551f  R12: ffff9edd94b07200
    R13: 11ff9edd94b07400  R14: ffff9eddbef8f840  R15: ffff9eddbcce7100
    ORIG_RAX: ffffffffffffffff  CS: 0010  SS: 0018
 #7 [ffff9eddbbc9bd70] flush_cpu_slab at ffffffffa701a386
 #8 [ffff9eddbbc9bd90] on_each_cpu_mask at ffffffffa6f118ae
 #9 [ffff9eddbbc9bdc0] on_each_cpu_cond at ffffffffa6f11994
#10 [ffff9eddbbc9be18] kmem_cache_close at ffffffffa701e5a5
#11 [ffff9eddbbc9be78] __kmem_cache_shutdown at ffffffffa701eb34
#12 [ffff9eddbbc9be98] kmem_cache_destroy at ffffffffa6fdb8f0
#13 [ffff9eddbbc9beb8] cleanup_module at ffffffffc066b08b [kmod_corrupter_TEST_A_3c]
#14 [ffff9eddbbc9bec8] sys_delete_module at ffffffffa6f165de
#15 [ffff9eddbbc9bf50] system_call_fastpath at ffffffffa7574ddb
    RIP: 00007f0190e95397  RSP: 00007ffc6c5db0c8  RFLAGS: 00010202
    RAX: 00000000000000b0  RBX: 00000000025f7270  RCX: 0000000000000000
    RDX: 00007f0190f09b20  RSI: 0000000000000800  RDI: 00000000025f72d8
    RBP: 0000000000000000   R8: 00007f019115e060   R9: 00007f0190f09b20
    R10: 00007ffc6c5dab20  R11: 0000000000000202  R12: 00007ffc6c5dc7dd
    R13: 0000000000000000  R14: 00000000025f7270  R15: 00000000025f7010
    ORIG_RAX: 00000000000000b0  CS: 0033  SS: 002b
~~~

~~~
crash> dis -l deactivate_slab+164
/usr/src/debug/kernel-3.10.0-957.el7/linux-3.10.0-957.el7.x86_64/mm/slub.c: 241
0xffffffffa7019fe4 <deactivate_slab+164>:       mov    0x0(%r13,%rax,1),%rcx
~~~

~~~
crash> kmem -s | head -n3
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff9eddbcce7000 nf_conntrack_ffffffffa7b11640 312    31        48      4     4k
ffff9edd7647ff00 rpc_inode_cache          624         13        24      2     8k
~~~

~~~
crash> struct kmem_cache.cpu_slab ffff9eddbcce7100
  cpu_slab = 0x3d3a3d201080
~~~

~~~
crash> struct kmem_cache_cpu.freelist 0x3d3a3d201080:0
[0]: ffffdc17ffc01080
  freelist = 0xffff9edd94b07000
~~~

~~~
crash> rd -x 0xffff9edd94b07000 512
ffff9edd94b07000:  8888888888888888 8888888888888888 
[...]
ffff9edd94b071f0:  8888888888888888 0000000000000000 
ffff9edd94b07200:  8888888888888888 8888888888888888 
[...]
ffff9edd94b073f0:  8888888888888888 ffff9edd94b07000 
ffff9edd94b07400:  8888888888888888 8888888888888888 
[...]
ffff9edd94b075f0:  8888888888888888 ffff9edd94b07600 
ffff9edd94b07600:  8888888888888888 8888888888888888 
[...]
~~~

### TEST B-1
### TEST B-2
### TEST B-3a
### TEST B-3b
### TEST B-3c
