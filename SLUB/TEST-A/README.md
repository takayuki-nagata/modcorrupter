# Test Cases and Steps

Corrupt slab page with memory overrun.

~~~
memory overrun
----------------->
+----+---+----+---+----+---+-
|obj0|fp0|obj1|fp1|obj2|fp2|...
+----+---+----+---+----+---+-
~~~

## TEST-A_CASE-1: after corrupting fp of free object with memory overrun

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

## TEST-A_CASE-2: after corrupting freelist with allocating new object

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

## TEST-A_CASE-3a: after using corrupted freelist with allocating new object

4. alloc obj2

## TEST-A_CASE-3b: after using corrupted freelist with freeing allocated object

4. free obj1

## TEST-A_CASE-3c: after freeing all allocated objects, and destroy the slab cache

4. free obj1
5. free obj0
6. destory the slab cache

# Test Results
## Red Hat Enterprise Linux 7
### TEST-A_CASE-1: after corrupting fp of free object with memory overrun

~~~
crash> kmem -S ffff93ab3b5c9100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff93ab3b5c9100 corrupter_slab_cache     504          7         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffcca1ffc010a0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  fffff8ef81b44e40  ffff93ab2d139000     0      8          7     1
  FREE / [ALLOCATED]
  [ffff93ab2d139000]
   ffff93ab2d139200  (cpu 0 cache)
  [ffff93ab2d139400]
  [ffff93ab2d139600]
  [ffff93ab2d139800]
  [ffff93ab2d139a00]
  [ffff93ab2d139c00]
  [ffff93ab2d139e00]
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff93ab3d78f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffcca1ffc010a0
  freelist = 0xffff93ab2d139200
~~~

~~~
crash> rd -x ffff93ab2d139000 512
ffff93ab2d139000:  8811881188118811 8811881188118811 
[...]
ffff93ab2d1391f0:  8811881188118811 ff1193112d119211 
                                    ~~~~~~~~~~~~~~~~
ffff93ab2d139200:  8811881188118811 8811881188118811 
[...]
ffff93ab2d1393f0:  8811881188118811 ff1193112d119411
                                    ~~~~~~~~~~~~~~~~ 
ffff93ab2d139400:  8888888888888888 8888888888888888 
[...]
ffff93ab2d1395f0:  8888888888888888 ffff93ab2d139600 
ffff93ab2d139600:  8888888888888888 8888888888888888 
[...]
~~~

- System status: operational

### TEST-A_CASE-2: after corrupting freelist with allocating new object

~~~
crash> kmem -S ffff9848bcf07100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff9848bcf07100 corrupter_slab_cache     504          8         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffd7467fc010b0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  ffffec2f81f1a780  ffff9848bc69e000     0      8          8     0
  FREE / [ALLOCATED]
  [ffff9848bc69e000]
  [ffff9848bc69e200]
  [ffff9848bc69e400]
  [ffff9848bc69e600]
  [ffff9848bc69e800]
  [ffff9848bc69ea00]
  [ffff9848bc69ec00]
  [ffff9848bc69ee00]
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff9848bef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffd7467fc010b0
  freelist = 0xff119811bc11e411
               ~~~~~~~~~~~~~~~~
~~~

~~~
crash> rd -x ffff9848bc69e000 512
ffff9848bc69e000:  8811881188118811 8811881188118811 
[...]
ffff9848bc69e1f0:  8811881188118811 ff119811bc11e211 
ffff9848bc69e200:  8811881188118811 8811881188118811 
[...]
ffff9848bc69e3f0:  8811881188118811 ff119811bc11e411 
ffff9848bc69e400:  8888888888888888 8888888888888888 
[...]
ffff9848bc69e5f0:  8888888888888888 ffff9848bc69e600 
ffff9848bc69e600:  8888888888888888 8888888888888888 
[...]
~~~

- System status: operational

### TEST-A_CASE-3a: after using corrupted freelist with allocating new object

~~~
crash> log
[...]
[   50.002936] general protection fault: 0000 [#1] SMP 
[   50.002958] Modules linked in: corrupter_TEST_A_CASE_3a(POE+) ip6t_rpfilter ipt_REJECT nf_reject_ipv4 ip6t_REJECT nf_reject_ipv6 xt_conntrack ip_set nfnetlink ebtable_nat ebtable_broute bridge stp llc ip6table_nat nf_conntrack_ipv6 nf_defrag_ipv6 nf_nat_ipv6 ip6table_mangle ip6table_security ip6table_raw iptable_nat nf_conntrack_ipv4 nf_defrag_ipv4 nf_nat_ipv4 nf_nat nf_conntrack iptable_mangle iptable_security iptable_raw ebtable_filter ebtables ip6table_filter ip6_tables iptable_filter sunrpc ppdev iosf_mbi crc32_pclmul ghash_clmulni_intel aesni_intel lrw gf128mul glue_helper ablk_helper cryptd joydev pcspkr virtio_balloon sg parport_pc parport i2c_piix4 ip_tables xfs libcrc32c sr_mod cdrom virtio_blk virtio_console virtio_net ata_generic pata_acpi crct10dif_pclmul crct10dif_common crc32c_intel
[   50.003203]  qxl serio_raw drm_kms_helper syscopyarea sysfillrect sysimgblt fb_sys_fops ttm drm floppy ata_piix libata virtio_pci virtio_ring virtio drm_panel_orientation_quirks dm_mirror dm_region_hash dm_log dm_mod
[   50.003270] CPU: 0 PID: 4021 Comm: insmod Kdump: loaded Tainted: P           OE  ------------   3.10.0-957.el7.x86_64 #1
[   50.003296] Hardware name: QEMU Standard PC (i440FX + PIIX, 1996), BIOS ?-20180724_192412-buildhw-07.phx2.fedoraproject.org-1.fc29 04/01/2014
[   50.003327] task: ffff8d3dfcab8000 ti: ffff8d3dfda64000 task.ti: ffff8d3dfda64000
[   50.003345] RIP: 0010:[<ffffffff9a81bcf4>]  [<ffffffff9a81bcf4>] kmem_cache_alloc+0x74/0x1f0
[   50.003370] RSP: 0018:ffff8d3dfda67cd8  EFLAGS: 00010282
[   50.003383] RAX: 00000000000001f8 RBX: ffff8d3dfcd3e100 RCX: 0000000000000003
[   50.003401] RDX: 0000000000000002 RSI: 00000000000000d0 RDI: ffff8d3dfcd3e100
[   50.003418] RBP: ffff8d3dfda67d08 R08: 00002f82bd201070 R09: ffffffffc08730aa
[   50.003436] R10: 0000000000000000 R11: fffffffffffffff6 R12: ff118d11fc11a411
[   50.003453] R13: 00000000000000d0 R14: ffff8d3dfcd3e100 R15: ffff8d3dfcd3e100
[   50.003471] FS:  00007facec2cd740(0000) GS:ffff8d3e02a00000(0000) knlGS:0000000000000000
[   50.003491] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[   50.003505] CR2: 000000000041d458 CR3: 000000007b944000 CR4: 00000000001606f0
[   50.003526] Call Trace:
[   50.003537]  [<ffffffffc08730aa>] ? corrupter_slab_doit+0x7a/0x90 [corrupter_TEST_A_CASE_3a]
[   50.003560]  [<ffffffffc04c7000>] ? 0xffffffffc04c6fff
[   50.003574]  [<ffffffffc08730aa>] corrupter_slab_doit+0x7a/0x90 [corrupter_TEST_A_CASE_3a]
[   50.003618]  [<ffffffffc04c7037>] corrupter_init+0x37/0x1000 [corrupter_TEST_A_CASE_3a]
[   50.003649]  [<ffffffff9a60210a>] do_one_initcall+0xba/0x240
[   50.003665]  [<ffffffff9a718f8c>] load_module+0x272c/0x2bc0
[   50.003681]  [<ffffffff9a9a3030>] ? ddebug_proc_write+0x100/0x100
[   50.003697]  [<ffffffff9a714b13>] ? copy_module_from_fd.isra.44+0x53/0x150
[   50.003715]  [<ffffffff9a719606>] SyS_finit_module+0xa6/0xd0
[   50.003731]  [<ffffffff9ad74ddb>] system_call_fastpath+0x22/0x27
[   50.003746] Code: 54 7f 65 49 8b 50 08 4d 8b 20 49 8b 40 10 4d 85 e4 0f 84 28 01 00 00 48 85 c0 0f 84 1f 01 00 00 49 63 46 20 48 8d 4a 01 4d 8b 06 <49> 8b 1c 04 4c 89 e0 65 49 0f c7 08 0f 94 c0 84 c0 74 ba 49 63 
[   50.003864] RIP  [<ffffffff9a81bcf4>] kmem_cache_alloc+0x74/0x1f0
[   50.003882]  RSP <ffff8d3dfda67cd8>
~~~

~~~
crash> bt
PID: 4021   TASK: ffff8d3dfcab8000  CPU: 0   COMMAND: "insmod"
 #0 [ffff8d3dfda67a48] machine_kexec at ffffffff9a663674
 #1 [ffff8d3dfda67aa8] __crash_kexec at ffffffff9a71ce12
 #2 [ffff8d3dfda67b78] crash_kexec at ffffffff9a71cf00
 #3 [ffff8d3dfda67b90] oops_end at ffffffff9ad6c758
 #4 [ffff8d3dfda67bb8] die at ffffffff9a62f95b
 #5 [ffff8d3dfda67be8] do_general_protection at ffffffff9ad6c052
 #6 [ffff8d3dfda67c20] general_protection at ffffffff9ad6b6f8
    [exception RIP: kmem_cache_alloc+116]
    RIP: ffffffff9a81bcf4  RSP: ffff8d3dfda67cd8  RFLAGS: 00010282
    RAX: 00000000000001f8  RBX: ffff8d3dfcd3e100  RCX: 0000000000000003
    RDX: 0000000000000002  RSI: 00000000000000d0  RDI: ffff8d3dfcd3e100
    RBP: ffff8d3dfda67d08   R8: 00002f82bd201070   R9: ffffffffc08730aa
    R10: 0000000000000000  R11: fffffffffffffff6  R12: ff118d11fc11a411
    R13: 00000000000000d0  R14: ffff8d3dfcd3e100  R15: ffff8d3dfcd3e100
    ORIG_RAX: ffffffffffffffff  CS: 0010  SS: 0018
 #7 [ffff8d3dfda67d10] corrupter_slab_doit at ffffffffc08730aa [corrupter_TEST_A_CASE_3a]
 #8 [ffff8d3dfda67d28] init_module at ffffffffc04c7037 [corrupter_TEST_A_CASE_3a]
 #9 [ffff8d3dfda67d38] do_one_initcall at ffffffff9a60210a
#10 [ffff8d3dfda67d68] load_module at ffffffff9a718f8c
#11 [ffff8d3dfda67eb8] sys_finit_module at ffffffff9a719606
#12 [ffff8d3dfda67f50] system_call_fastpath at ffffffff9ad74ddb
    RIP: 00007faceb7921c9  RSP: 00007fffb8ee95c8  RFLAGS: 00010203
    RAX: 0000000000000139  RBX: 00000000006b92b0  RCX: 00007faceb803fd0
    RDX: 0000000000000000  RSI: 000000000041a94e  RDI: 0000000000000003
    RBP: 000000000041a94e   R8: 0000000000000000   R9: 00007fffb8ee97d8
    R10: 0000000000000003  R11: 0000000000000202  R12: 0000000000000000
    R13: 00000000006b9270  R14: 0000000000000000  R15: 0000000000000000
    ORIG_RAX: 0000000000000139  CS: 0033  SS: 002b
~~~

~~~
crash> dis -l kmem_cache_alloc+116
/usr/src/debug/kernel-3.10.0-957.el7/linux-3.10.0-957.el7.x86_64/mm/slub.c: 241
0xffffffff9a81bcf4 <kmem_cache_alloc+116>:      mov    (%r12,%rax,1),%rbx
~~~

~~~
crash> kmem -S ffff8d3dfcd3e100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff8d3dfcd3e100 corrupter_slab_cache     504          8         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffbcc0bfc01070
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  ffffc27681f18a80  ffff8d3dfc62a000     0      8          8     0
  FREE / [ALLOCATED]
  [ffff8d3dfc62a000]
  [ffff8d3dfc62a200]
  [ffff8d3dfc62a400]
  [ffff8d3dfc62a600]
  [ffff8d3dfc62a800]
  [ffff8d3dfc62aa00]
  [ffff8d3dfc62ac00]
  [ffff8d3dfc62ae00]
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff8d3dfef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffbcc0bfc01070
  freelist = 0xff118d11fc11a411
~~~

~~~
crash> rd -x ffff8d3dfc62a000 512
ffff8d3dfc62a000:  8811881188118811 8811881188118811 
[...]
ffff8d3dfc62a1f0:  8811881188118811 ff118d11fc11a211 
ffff8d3dfc62a200:  8811881188118811 8811881188118811 
[...]
ffff8d3dfc62a3f0:  8811881188118811 ff118d11fc11a411 
ffff8d3dfc62a400:  8888888888888888 8888888888888888 
[...]
ffff8d3dfc62a5f0:  8888888888888888 ffff8d3dfc62a600 
ffff8d3dfc62a600:  8888888888888888 8888888888888888 
[...]
~~~

- System status: kernel panic

### TEST-A_CASE-3b: after using corrupted freelist with freeing allocated object

~~~
crash> kmem -S ffff99c63ccb7100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
ffff99c63ccb7100 corrupter_slab_cache     504          7         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffcb9d7fc010b0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
  ffffe4b901f198c0  ffff99c63c663000     0      8          7     1
  FREE / [ALLOCATED]
  [ffff99c63c663000]
   ffff99c63c663200  (cpu 0 cache)
  [ffff99c63c663400]
  [ffff99c63c663600]
  [ffff99c63c663800]
  [ffff99c63c663a00]
  [ffff99c63c663c00]
  [ffff99c63c663e00]
CPU 0 PARTIAL:
  (empty)
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff99c63ef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffcb9d7fc010b0
  freelist = 0xffff99c63c663200
~~~

~~~
crash> rd -x ffff99c63c663000 512
ffff99c63c663000:  8811881188118811 8811881188118811 
[...]
ffff99c63c6631f0:  8811881188118811 ff1199113c113211 
ffff99c63c663200:  8811881188118811 8811881188118811 
[...]
ffff99c63c6633f0:  8811881188118811 ff1199113c113411 
ffff99c63c663400:  8888888888888888 8888888888888888 
[...]
ffff99c63c6635f0:  8888888888888888 ffff99c63c663600 
ffff99c63c663600:  8888888888888888 8888888888888888 
[...]
~~~

- System status: operational

### TEST-A_CASE-3c: after freeing all allocated objects, and destroy the slab cache

~~~
[   54.443911] general protection fault: 0000 [#1] SMP 
[   54.443932] Modules linked in: corrupter_TEST_A_CASE_3c(POE+) ip6t_rpfilter ipt_REJECT nf_reject_ipv4 ip6t_REJECT nf_reject_ipv6 xt_conntrack ip_set nfnetlink ebtable_nat ebtable_broute bridge stp llc ip6table_nat nf_conntrack_ipv6 nf_defrag_ipv6 nf_nat_ipv6 ip6table_mangle ip6table_security ip6table_raw iptable_nat nf_conntrack_ipv4 nf_defrag_ipv4 nf_nat_ipv4 nf_nat nf_conntrack iptable_mangle iptable_security iptable_raw ebtable_filter ebtables ip6table_filter ip6_tables iptable_filter sunrpc iosf_mbi crc32_pclmul ppdev ghash_clmulni_intel aesni_intel lrw gf128mul glue_helper ablk_helper cryptd joydev pcspkr virtio_balloon sg parport_pc parport i2c_piix4 ip_tables xfs libcrc32c sr_mod cdrom virtio_blk virtio_console virtio_net ata_generic pata_acpi crct10dif_pclmul crct10dif_common crc32c_intel
[   54.444179]  qxl serio_raw drm_kms_helper syscopyarea sysfillrect sysimgblt fb_sys_fops ttm drm floppy ata_piix libata virtio_pci virtio_ring virtio drm_panel_orientation_quirks dm_mirror dm_region_hash dm_log dm_mod
[   54.444247] CPU: 0 PID: 4039 Comm: insmod Kdump: loaded Tainted: P           OE  ------------   3.10.0-957.el7.x86_64 #1
[   54.444284] Hardware name: QEMU Standard PC (i440FX + PIIX, 1996), BIOS ?-20180724_192412-buildhw-07.phx2.fedoraproject.org-1.fc29 04/01/2014
[   54.444315] task: ffff9612766cc100 ti: ffff9612bcd68000 task.ti: ffff9612bcd68000
[   54.444333] RIP: 0010:[<ffffffff8ba19fe4>]  [<ffffffff8ba19fe4>] deactivate_slab+0xa4/0x3c0
[   54.444358] RSP: 0018:ffff9612bcd6bb10  EFLAGS: 00010086
[   54.444372] RAX: 00000000000001f8 RBX: ffffe83ac1f19840 RCX: 0000000180080006
[   54.444389] RDX: ffff9612bc661200 RSI: ffffe83ac1f19840 RDI: 0000000040020000
[   54.444407] RBP: ffff9612bcd6bbc0 R08: ffff9612bc661200 R09: 0000000180080006
[   54.444424] R10: ffff9612c2401500 R11: ffffffff8bb7551f R12: ffff9612bc661200
[   54.444441] R13: ff119611bc111411 R14: ffff9612bef8f840 R15: ffff9612bccd6100
[   54.444459] FS:  00007f488d9d7740(0000) GS:ffff9612c2a00000(0000) knlGS:0000000000000000
[   54.444479] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[   54.444493] CR2: 000000000041d458 CR3: 000000007ccae000 CR4: 00000000001606f0
[   54.444513] Call Trace:
[   54.444524]  [<ffffffff8bb7551f>] ? alloc_cpumask_var_node+0x1f/0x70
[   54.444540]  [<ffffffff8bb75465>] ? cpumask_next_and+0x35/0x50
[   54.444557]  [<ffffffff8ba1a386>] flush_cpu_slab+0x36/0x60
[   54.444572]  [<ffffffff8ba1a350>] ? __flush_cpu_slab+0x50/0x50
[   54.444588]  [<ffffffff8b9118ae>] on_each_cpu_mask+0x4e/0x70
[   54.444603]  [<ffffffff8ba17400>] ? check_valid_pointer+0x80/0x80
[   54.444619]  [<ffffffff8b911994>] on_each_cpu_cond+0xc4/0x190
[   54.444634]  [<ffffffff8ba1a350>] ? __flush_cpu_slab+0x50/0x50
[   54.444653]  [<ffffffffc04b3000>] ? 0xffffffffc04b2fff
[   54.444668]  [<ffffffff8ba1e5a5>] kmem_cache_close+0x35/0x300
[   54.444684]  [<ffffffffc085a068>] ? corrupter_slab_doit+0x38/0xa0 [corrupter_TEST_A_CASE_3c]
[   54.444706]  [<ffffffffc04b3000>] ? 0xffffffffc04b2fff
[   54.444721]  [<ffffffff8ba1eb34>] __kmem_cache_shutdown+0x14/0x80
[   54.444738]  [<ffffffff8b9db8f0>] kmem_cache_destroy+0x60/0x130
[   54.444772]  [<ffffffffc085a0bf>] corrupter_slab_doit+0x8f/0xa0 [corrupter_TEST_A_CASE_3c]
[   54.444802]  [<ffffffffc04b3037>] corrupter_init+0x37/0x1000 [corrupter_TEST_A_CASE_3c]
[   54.444822]  [<ffffffff8b80210a>] do_one_initcall+0xba/0x240
[   54.444838]  [<ffffffff8b918f8c>] load_module+0x272c/0x2bc0
[   54.444853]  [<ffffffff8bba3030>] ? ddebug_proc_write+0x100/0x100
[   54.444869]  [<ffffffff8b914b13>] ? copy_module_from_fd.isra.44+0x53/0x150
[   54.445519]  [<ffffffff8b919606>] SyS_finit_module+0xa6/0xd0
[   54.446197]  [<ffffffff8bf74ddb>] system_call_fastpath+0x22/0x27
[   54.446867] Code: de 48 89 4c 24 58 66 83 6c 24 58 01 4c 8b 4c 24 58 e8 31 f4 ff ff 84 c0 74 cd 4d 85 ed 0f 84 a6 02 00 00 49 63 47 20 48 8b 53 10 <49> 8b 4c 05 00 48 85 c9 74 0b 4d 89 ec 49 89 cd eb b2 4d 89 e5 
[   54.448292] RIP  [<ffffffff8ba19fe4>] deactivate_slab+0xa4/0x3c0
[   54.448973]  RSP <ffff9612bcd6bb10>
~~~

~~~
crash> bt
PID: 4039   TASK: ffff9612766cc100  CPU: 0   COMMAND: "insmod"
 #0 [ffff9612bcd6b888] machine_kexec at ffffffff8b863674
 #1 [ffff9612bcd6b8e8] __crash_kexec at ffffffff8b91ce12
 #2 [ffff9612bcd6b9b8] crash_kexec at ffffffff8b91cf00
 #3 [ffff9612bcd6b9d0] oops_end at ffffffff8bf6c758
 #4 [ffff9612bcd6b9f8] die at ffffffff8b82f95b
 #5 [ffff9612bcd6ba28] do_general_protection at ffffffff8bf6c052
 #6 [ffff9612bcd6ba60] general_protection at ffffffff8bf6b6f8
    [exception RIP: deactivate_slab+164]
    RIP: ffffffff8ba19fe4  RSP: ffff9612bcd6bb10  RFLAGS: 00010086
    RAX: 00000000000001f8  RBX: ffffe83ac1f19840  RCX: 0000000180080006
    RDX: ffff9612bc661200  RSI: ffffe83ac1f19840  RDI: 0000000040020000
    RBP: ffff9612bcd6bbc0   R8: ffff9612bc661200   R9: 0000000180080006
    R10: ffff9612c2401500  R11: ffffffff8bb7551f  R12: ffff9612bc661200
    R13: ff119611bc111411  R14: ffff9612bef8f840  R15: ffff9612bccd6100
    ORIG_RAX: ffffffffffffffff  CS: 0010  SS: 0018
 #7 [ffff9612bcd6bbc8] flush_cpu_slab at ffffffff8ba1a386
 #8 [ffff9612bcd6bbe8] on_each_cpu_mask at ffffffff8b9118ae
 #9 [ffff9612bcd6bc18] on_each_cpu_cond at ffffffff8b911994
#10 [ffff9612bcd6bc70] kmem_cache_close at ffffffff8ba1e5a5
#11 [ffff9612bcd6bcd0] __kmem_cache_shutdown at ffffffff8ba1eb34
#12 [ffff9612bcd6bcf0] kmem_cache_destroy at ffffffff8b9db8f0
#13 [ffff9612bcd6bd10] corrupter_slab_doit at ffffffffc085a0bf [corrupter_TEST_A_CASE_3c]
#14 [ffff9612bcd6bd28] init_module at ffffffffc04b3037 [corrupter_TEST_A_CASE_3c]
#15 [ffff9612bcd6bd38] do_one_initcall at ffffffff8b80210a
#16 [ffff9612bcd6bd68] load_module at ffffffff8b918f8c
#17 [ffff9612bcd6beb8] sys_finit_module at ffffffff8b919606
#18 [ffff9612bcd6bf50] system_call_fastpath at ffffffff8bf74ddb
    RIP: 00007f488ce9c1c9  RSP: 00007ffe06373328  RFLAGS: 00010203
    RAX: 0000000000000139  RBX: 000000000149b2b0  RCX: 00007f488cf0dfd0
    RDX: 0000000000000000  RSI: 000000000041a94e  RDI: 0000000000000003
    RBP: 000000000041a94e   R8: 0000000000000000   R9: 00007ffe06373538
    R10: 0000000000000003  R11: 0000000000000202  R12: 0000000000000000
    R13: 000000000149b270  R14: 0000000000000000  R15: 0000000000000000
    ORIG_RAX: 0000000000000139  CS: 0033  SS: 002b
~~~

~~~
crash> dis -l deactivate_slab+164
/usr/src/debug/kernel-3.10.0-957.el7/linux-3.10.0-957.el7.x86_64/mm/slub.c: 241
0xffffffff8ba19fe4 <deactivate_slab+164>:       mov    0x0(%r13,%rax,1),%rcx
~~~

~~~
crash> struct kmem_cache.name ffff9612bccd6100
  name = 0xffff9612bca5c1e0 "corrupter_slab_cache"
~~~

~~~
crash> struct kmem_cache.cpu_slab ffff9612bccd6100
  cpu_slab = 0x2edc7d2010a0
crash> struct kmem_cache_cpu.freelist 0x2edc7d2010a0:0
[0]: ffffc4ef3fc010a0
  freelist = 0xffff9612bc661000
~~~

~~~
crash> rd -x 0xffff9612bc661000 512
ffff9612bc661000:  8811881188118811 8811881188118811 
[...]
ffff9612bc6611f0:  8811881188118811 0000000000000000 
ffff9612bc661200:  8811881188118811 8811881188118811 
[...]
ffff9612bc6613f0:  8811881188118811 ffff9612bc661000 
ffff9612bc661400:  8888888888888888 8888888888888888 
[...]
ffff9612bc6615f0:  8888888888888888 ffff9612bc661600 
ffff9612bc661600:  8888888888888888 8888888888888888 
[...]
~~~

- System status: kernel panic
