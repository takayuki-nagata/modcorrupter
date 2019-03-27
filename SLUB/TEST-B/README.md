# Test Cases and Steps

Corrupt Slab page with memory underrun.

~~~
     underrun
        <-----
+----+---+----+---+----+---+-
|obj0|fp0|obj1|fp1|obj2|fp2|...
+----+---+----+---+----+---+-
~~~

## TEST-B_CASE-1: after corrupting fp of free object with memory underrun

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

## TEST-B_CASE-2: after corrupting freelist with allocating new object

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

## TEST-B_CASE-3a: after using corrupted freelist with allocating new object

6. alloc obj2

## TEST-B_CASE-3b: after using corrupted freelist with freeing existing object

6. free obj1

## TEST-B_CASE-3c: after freeing all allocated objects, and destory the slab cache

6. free obj1
7. free obj0
8. destory the slab cache

# Test Results
## Red Hat Enterprise Linux 7
### TEST-B_CASE-1: after corrupting fp of free object with memory underrun

~~~
crash> kmem -S ffff9f113cf2e100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
kmem: corrupter_slab_cache: slab: ffffe39e41f19240 invalid freepointer: 11ff9f113c6495f8
ffff9f113cf2e100 corrupter_slab_cache     504          6         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffcede7fc01080
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
kmem: corrupter_slab_cache: slab: ffffe39e41f19240 invalid freepointer: 11ff9f113c6495f8
  ffffe39e41f19240  ffff9f113c649000     0      8          6     2
  FREE / [ALLOCATED]
   ffff9f113c649000  (cpu 0 cache)
kmem: corrupter_slab_cache: slab: ffffe39e41f19240 invalid freepointer: 11ff9f113c6495f8
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff9f113ef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffcede7fc01080
  freelist = 0xffff9f113c649000
~~~

~~~
crash> rd -x ffff9f113c649000 512
ffff9f113c649000:  8888888888888888 8888888888888888 
[...]
ffff9f113c6491f0:  8888888888888888 11ff9f113c649400 
                                    ~~
ffff9f113c649200:  1111111111111111 1111111111111111 
[...]
ffff9f113c6493f0:  1111111111111111 ffff9f113c649400 
ffff9f113c649400:  8888888888888888 8888888888888888 
[...]
ffff9f113c6495f0:  8888888888888888 ffff9f113c649600 
ffff9f113c649600:  8888888888888888 8888888888888888 
[...]
~~~

- System status: operatiopnal

### TEST-B_CASE-2: after corrupting freelist with allocating new object

~~~
crash> kmem -S ffff8c60fccc6100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
kmem: corrupter_slab_cache: slab: ffffdd2a41f19e40 invalid freepointer: 11ff8c60fc6795f8
ffff8c60fccc6100 corrupter_slab_cache     504          7         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffd9207fc010a0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
kmem: corrupter_slab_cache: slab: ffffdd2a41f19e40 invalid freepointer: 11ff8c60fc6795f8
  ffffdd2a41f19e40  ffff8c60fc679000     0      8          7     1
  FREE / [ALLOCATED]
kmem: corrupter_slab_cache: slab: ffffdd2a41f19e40 invalid freepointer: 11ff8c60fc6795f8
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff8c60fef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffd9207fc010a0
  freelist = 0x11ff8c60fc679400
               ~~
~~~

~~~
crash> rd -x ffff8c60fc679000 512
ffff8c60fc679000:  8888888888888888 8888888888888888 
[...]
ffff8c60fc6791f0:  8888888888888888 11ff8c60fc679400 
ffff8c60fc679200:  1111111111111111 1111111111111111 
[...]
ffff8c60fc6793f0:  1111111111111111 ffff8c60fc679400 
ffff8c60fc679400:  8888888888888888 8888888888888888 
[...]
ffff8c60fc6795f0:  8888888888888888 ffff8c60fc679600 
ffff8c60fc679600:  8888888888888888 8888888888888888 
[...]
~~~

- System status: operational

### TEST-B_CASE-3a: after using corrupted freelist with allocating new object

~~~
crash> log
[...]
[   38.969230] general protection fault: 0000 [#1] SMP 
[   38.969252] Modules linked in: corrupter_TEST_B_CASE_3a(POE+) ip6t_rpfilter ipt_REJECT nf_reject_ipv4 ip6t_REJECT nf_reject_ipv6 xt_conntrack ip_set nfnetlink ebtable_nat ebtable_broute bridge stp llc ip6table_nat nf_conntrack_ipv6 nf_defrag_ipv6 nf_nat_ipv6 ip6table_mangle ip6table_security ip6table_raw iptable_nat nf_conntrack_ipv4 nf_defrag_ipv4 nf_nat_ipv4 nf_nat nf_conntrack iptable_mangle iptable_security iptable_raw ebtable_filter ebtables ip6table_filter ip6_tables iptable_filter sunrpc iosf_mbi crc32_pclmul ppdev ghash_clmulni_intel aesni_intel lrw gf128mul glue_helper ablk_helper cryptd joydev pcspkr virtio_balloon sg parport_pc parport i2c_piix4 ip_tables xfs libcrc32c sr_mod cdrom virtio_blk virtio_console virtio_net ata_generic pata_acpi crct10dif_pclmul crct10dif_common crc32c_intel
[   38.969519]  qxl drm_kms_helper serio_raw syscopyarea sysfillrect sysimgblt fb_sys_fops ttm drm floppy ata_piix libata virtio_pci virtio_ring virtio drm_panel_orientation_quirks dm_mirror dm_region_hash dm_log dm_mod
[   38.969585] CPU: 0 PID: 4051 Comm: insmod Kdump: loaded Tainted: P           OE  ------------   3.10.0-957.el7.x86_64 #1
[   38.969610] Hardware name: QEMU Standard PC (i440FX + PIIX, 1996), BIOS ?-20180724_192412-buildhw-07.phx2.fedoraproject.org-1.fc29 04/01/2014
[   38.969640] task: ffff8ca4366ab0c0 ti: ffff8ca47a56c000 task.ti: ffff8ca47a56c000
[   38.969657] RIP: 0010:[<ffffffffb141bcf4>]  [<ffffffffb141bcf4>] kmem_cache_alloc+0x74/0x1f0
[   38.969682] RSP: 0018:ffff8ca47a56fcd8  EFLAGS: 00010282
[   38.969695] RAX: 00000000000001f8 RBX: ffff8ca47b8eb100 RCX: 0000000000000005
[   38.969712] RDX: 0000000000000004 RSI: 00000000000000d0 RDI: ffff8ca47b8eb100
[   38.969729] RBP: ffff8ca47a56fd08 R08: 000043abfd201080 R09: ffffffffc05340d5
[   38.969746] R10: ffffd0507fc01080 R11: fffff9c840030380 R12: 11ff8ca400c0e400
[   38.969763] R13: 00000000000000d0 R14: ffff8ca47b8eb100 R15: ffff8ca47b8eb100
[   38.969780] FS:  00007f385163b740(0000) GS:ffff8ca482a00000(0000) knlGS:0000000000000000
[   38.969799] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[   38.969813] CR2: 000000000041d458 CR3: 000000007b904000 CR4: 00000000001606f0
[   38.969833] Call Trace:
[   38.969842]  [<ffffffffc05340d5>] ? corrupter_slab_doit+0xa5/0xb0 [corrupter_TEST_B_CASE_3a]
[   38.969867]  [<ffffffffc0191000>] ? 0xffffffffc0190fff
[   38.969883]  [<ffffffffc05340d5>] corrupter_slab_doit+0xa5/0xb0 [corrupter_TEST_B_CASE_3a]
[   38.969904]  [<ffffffffc0191037>] corrupter_init+0x37/0x1000 [corrupter_TEST_B_CASE_3a]
[   38.969926]  [<ffffffffb120210a>] do_one_initcall+0xba/0x240
[   38.969942]  [<ffffffffb1318f8c>] load_module+0x272c/0x2bc0
[   38.969957]  [<ffffffffb15a3030>] ? ddebug_proc_write+0x100/0x100
[   38.969972]  [<ffffffffb1314b13>] ? copy_module_from_fd.isra.44+0x53/0x150
[   38.969990]  [<ffffffffb1319606>] SyS_finit_module+0xa6/0xd0
[   38.970005]  [<ffffffffb1974ddb>] system_call_fastpath+0x22/0x27
[   38.970020] Code: 54 bf 4e 49 8b 50 08 4d 8b 20 49 8b 40 10 4d 85 e4 0f 84 28 01 00 00 48 85 c0 0f 84 1f 01 00 00 49 63 46 20 48 8d 4a 01 4d 8b 06 <49> 8b 1c 04 4c 89 e0 65 49 0f c7 08 0f 94 c0 84 c0 74 ba 49 63 
[   38.970132] RIP  [<ffffffffb141bcf4>] kmem_cache_alloc+0x74/0x1f0
[   38.970150]  RSP <ffff8ca47a56fcd8>
~~~

~~~
crash> bt
PID: 4051   TASK: ffff8ca4366ab0c0  CPU: 0   COMMAND: "insmod"
 #0 [ffff8ca47a56fa48] machine_kexec at ffffffffb1263674
 #1 [ffff8ca47a56faa8] __crash_kexec at ffffffffb131ce12
 #2 [ffff8ca47a56fb78] crash_kexec at ffffffffb131cf00
 #3 [ffff8ca47a56fb90] oops_end at ffffffffb196c758
 #4 [ffff8ca47a56fbb8] die at ffffffffb122f95b
 #5 [ffff8ca47a56fbe8] do_general_protection at ffffffffb196c052
 #6 [ffff8ca47a56fc20] general_protection at ffffffffb196b6f8
    [exception RIP: kmem_cache_alloc+116]
    RIP: ffffffffb141bcf4  RSP: ffff8ca47a56fcd8  RFLAGS: 00010282
    RAX: 00000000000001f8  RBX: ffff8ca47b8eb100  RCX: 0000000000000005
    RDX: 0000000000000004  RSI: 00000000000000d0  RDI: ffff8ca47b8eb100
    RBP: ffff8ca47a56fd08   R8: 000043abfd201080   R9: ffffffffc05340d5
    R10: ffffd0507fc01080  R11: fffff9c840030380  R12: 11ff8ca400c0e400
    R13: 00000000000000d0  R14: ffff8ca47b8eb100  R15: ffff8ca47b8eb100
    ORIG_RAX: ffffffffffffffff  CS: 0010  SS: 0018
 #7 [ffff8ca47a56fd10] corrupter_slab_doit at ffffffffc05340d5 [corrupter_TEST_B_CASE_3a]
 #8 [ffff8ca47a56fd28] init_module at ffffffffc0191037 [corrupter_TEST_B_CASE_3a]
 #9 [ffff8ca47a56fd38] do_one_initcall at ffffffffb120210a
#10 [ffff8ca47a56fd68] load_module at ffffffffb1318f8c
#11 [ffff8ca47a56feb8] sys_finit_module at ffffffffb1319606
#12 [ffff8ca47a56ff50] system_call_fastpath at ffffffffb1974ddb
    RIP: 00007f3850b001c9  RSP: 00007ffcc2f491b8  RFLAGS: 00010203
    RAX: 0000000000000139  RBX: 00000000008602b0  RCX: 00007f3850b71fd0
    RDX: 0000000000000000  RSI: 000000000041a94e  RDI: 0000000000000003
    RBP: 000000000041a94e   R8: 0000000000000000   R9: 00007ffcc2f493c8
    R10: 0000000000000003  R11: 0000000000000206  R12: 0000000000000000
    R13: 0000000000860270  R14: 0000000000000000  R15: 0000000000000000
    ORIG_RAX: 0000000000000139  CS: 0033  SS: 002b
~~~

~~~
crash> dis -l kmem_cache_alloc+116
/usr/src/debug/kernel-3.10.0-957.el7/linux-3.10.0-957.el7.x86_64/mm/slub.c: 241
0xffffffffb141bcf4 <kmem_cache_alloc+116>:      mov    (%r12,%rax,1),%rbx
~~~

~~~
crash> kmem -S ffff8ca47b8eb100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
kmem: corrupter_slab_cache: slab: fffff9c840030380 invalid freepointer: 11ff8ca400c0e5f8
ffff8ca47b8eb100 corrupter_slab_cache     504          7         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffd0507fc01080
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
kmem: corrupter_slab_cache: slab: fffff9c840030380 invalid freepointer: 11ff8ca400c0e5f8
  fffff9c840030380  ffff8ca400c0e000     0      8          7     1
  FREE / [ALLOCATED]
kmem: corrupter_slab_cache: slab: fffff9c840030380 invalid freepointer: 11ff8ca400c0e5f8
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff8ca47db8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffd0507fc01080
  freelist = 0x11ff8ca400c0e400
~~~

~~~
crash> rd -x ffff8ca400c0e000 512
ffff8ca400c0e000:  8888888888888888 8888888888888888 
[...]
ffff8ca400c0e1f0:  8888888888888888 11ff8ca400c0e400 
ffff8ca400c0e200:  1111111111111111 1111111111111111 
[...]

ffff8ca400c0e3f0:  1111111111111111 ffff8ca400c0e400 
ffff8ca400c0e400:  8888888888888888 8888888888888888 
[...]
ffff8ca400c0e5f0:  8888888888888888 ffff8ca400c0e600 
ffff8ca400c0e600:  8888888888888888 8888888888888888 
[...]
~~~

- System status: kernel panic

### TEST-B_CASE-3b: after using corrupted freelist with freeing existing object

~~~
crash> kmem -S ffff981bfcd0c100
CACHE            NAME                 OBJSIZE  ALLOCATED     TOTAL  SLABS  SSIZE
kmem: corrupter_slab_cache: slab: ffffcb4b41f0e7c0 invalid freepointer: 11ff981bfc39f5f8
ffff981bfcd0c100 corrupter_slab_cache     504          6         8      1     4k
CPU 0 KMEM_CACHE_CPU:
  ffffc5fd3fc010a0
CPU 0 SLAB:
  SLAB              MEMORY            NODE  TOTAL  ALLOCATED  FREE
kmem: corrupter_slab_cache: slab: ffffcb4b41f0e7c0 invalid freepointer: 11ff981bfc39f5f8
  ffffcb4b41f0e7c0  ffff981bfc39f000     0      8          6     2
  FREE / [ALLOCATED]
kmem: corrupter_slab_cache: slab: ffffcb4b41f0e7c0 invalid freepointer: 11ff981bfc39f5f8
KMEM_CACHE_NODE   NODE  SLABS  PARTIAL  PER-CPU
ffff981bfef8f840     0      1        0        1
NODE 0 PARTIAL:
  (empty)
NODE 0 FULL:
  (not tracked)
~~~

~~~
crash> struct kmem_cache_cpu.freelist ffffc5fd3fc010a0
  freelist = 0xffff981bfc39f200
~~~

~~~
crash> rd -x ffff981bfc39f000 512
ffff981bfc39f000:  8888888888888888 8888888888888888 
[...]
ffff981bfc39f1f0:  8888888888888888 11ff981bfc39f400 
ffff981bfc39f200:  1111111111111111 1111111111111111 
[...]
ffff981bfc39f3f0:  1111111111111111 11ff981bfc39f400 
ffff981bfc39f400:  8888888888888888 8888888888888888 
[...]
ffff981bfc39f5f0:  8888888888888888 ffff981bfc39f600 
ffff981bfc39f600:  8888888888888888 8888888888888888 
[...]
~~~

- System status: operational

### TEST-B_CASE-3c: after freeing all allocated objects, and destory the slab cache

~~~
crash> log
[...]
[  247.356805] general protection fault: 0000 [#1] SMP 
[  247.356827] Modules linked in: corrupter_TEST_B_CASE_3c(POE+) macsec tcp_diag udp_diag inet_diag unix_diag af_packet_diag netlink_diag binfmt_misc ip6t_rpfilter ipt_REJECT nf_reject_ipv4 ip6t_REJECT nf_reject_ipv6 xt_conntrack ip_set nfnetlink ebtable_nat ebtable_broute bridge stp llc ip6table_nat nf_conntrack_ipv6 nf_defrag_ipv6 nf_nat_ipv6 ip6table_mangle ip6table_security ip6table_raw iptable_nat nf_conntrack_ipv4 nf_defrag_ipv4 nf_nat_ipv4 nf_nat nf_conntrack iptable_mangle iptable_security iptable_raw ebtable_filter ebtables ip6table_filter ip6_tables iptable_filter sunrpc iosf_mbi crc32_pclmul ppdev ghash_clmulni_intel aesni_intel lrw gf128mul glue_helper ablk_helper cryptd joydev pcspkr sg virtio_balloon i2c_piix4 parport_pc parport ip_tables xfs libcrc32c sr_mod cdrom virtio_blk virtio_console
[  247.357075]  virtio_net ata_generic pata_acpi crct10dif_pclmul crct10dif_common crc32c_intel qxl drm_kms_helper serio_raw syscopyarea sysfillrect sysimgblt fb_sys_fops ttm floppy drm ata_piix libata virtio_pci virtio_ring virtio drm_panel_orientation_quirks dm_mirror dm_region_hash dm_log dm_mod
[  247.357167] CPU: 0 PID: 5753 Comm: insmod Kdump: loaded Tainted: P           OE  ------------   3.10.0-957.el7.x86_64 #1
[  247.357193] Hardware name: QEMU Standard PC (i440FX + PIIX, 1996), BIOS ?-20180724_192412-buildhw-07.phx2.fedoraproject.org-1.fc29 04/01/2014
[  247.357224] task: ffff9458bcacc100 ti: ffff945875ca4000 task.ti: ffff945875ca4000
[  247.357242] RIP: 0010:[<ffffffff94019fe4>]  [<ffffffff94019fe4>] deactivate_slab+0xa4/0x3c0
[  247.357266] RSP: 0018:ffff945875ca7b10  EFLAGS: 00010006
[  247.357280] RAX: 00000000000001f8 RBX: ffffd091c1f19fc0 RCX: 0000000180080006
[  247.357297] RDX: ffff9458bc67f200 RSI: ffffd091c1f19fc0 RDI: 0000000040020000
[  247.357333] RBP: ffff945875ca7bc0 R08: ffff9458bc67f200 R09: 0000000180080006
[  247.357360] R10: ffff9458c2401500 R11: ffffffff9417551f R12: ffff9458bc67f200
[  247.357379] R13: 11ff9458bc67f400 R14: ffff9458bef8f840 R15: ffff9458bcdbd100
[  247.357396] FS:  00007f4f7a4c7740(0000) GS:ffff9458c2a00000(0000) knlGS:0000000000000000
[  247.357416] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[  247.357430] CR2: 000000000041d458 CR3: 0000000060596000 CR4: 00000000001606f0
[  247.357451] Call Trace:
[  247.357461]  [<ffffffff9417551f>] ? alloc_cpumask_var_node+0x1f/0x70
[  247.357478]  [<ffffffff94175465>] ? cpumask_next_and+0x35/0x50
[  247.357494]  [<ffffffff9401a386>] flush_cpu_slab+0x36/0x60
[  247.357509]  [<ffffffff9401a350>] ? __flush_cpu_slab+0x50/0x50
[  247.357526]  [<ffffffff93f118ae>] on_each_cpu_mask+0x4e/0x70
[  247.357540]  [<ffffffff94017400>] ? check_valid_pointer+0x80/0x80
[  247.357557]  [<ffffffff93f11994>] on_each_cpu_cond+0xc4/0x190
[  247.357573]  [<ffffffff9401a350>] ? __flush_cpu_slab+0x50/0x50
[  247.357591]  [<ffffffffc023f000>] ? 0xffffffffc023efff
[  247.357606]  [<ffffffff9401e5a5>] kmem_cache_close+0x35/0x300
[  247.357622]  [<ffffffffc0601068>] ? corrupter_slab_doit+0x38/0xd0 [corrupter_TEST_B_CASE_3c]
[  247.357648]  [<ffffffffc023f000>] ? 0xffffffffc023efff
[  247.357662]  [<ffffffff9401eb34>] __kmem_cache_shutdown+0x14/0x80
[  247.357679]  [<ffffffff93fdb8f0>] kmem_cache_destroy+0x60/0x130
[  247.357695]  [<ffffffffc06010ee>] corrupter_slab_doit+0xbe/0xd0 [corrupter_TEST_B_CASE_3c]
[  247.357716]  [<ffffffffc023f037>] corrupter_init+0x37/0x1000 [corrupter_TEST_B_CASE_3c]
[  247.357737]  [<ffffffff93e0210a>] do_one_initcall+0xba/0x240
[  247.357752]  [<ffffffff93f18f8c>] load_module+0x272c/0x2bc0
[  247.357768]  [<ffffffff941a3030>] ? ddebug_proc_write+0x100/0x100
[  247.358459]  [<ffffffff93f14b13>] ? copy_module_from_fd.isra.44+0x53/0x150
[  247.359119]  [<ffffffff93f19606>] SyS_finit_module+0xa6/0xd0
[  247.359807]  [<ffffffff94574ddb>] system_call_fastpath+0x22/0x27
[  247.360495] Code: de 48 89 4c 24 58 66 83 6c 24 58 01 4c 8b 4c 24 58 e8 31 f4 ff ff 84 c0 74 cd 4d 85 ed 0f 84 a6 02 00 00 49 63 47 20 48 8b 53 10 <49> 8b 4c 05 00 48 85 c9 74 0b 4d 89 ec 49 89 cd eb b2 4d 89 e5 
[  247.361931] RIP  [<ffffffff94019fe4>] deactivate_slab+0xa4/0x3c0
[  247.362612]  RSP <ffff945875ca7b10>
~~~

~~~
crash> bt
PID: 5753   TASK: ffff9458bcacc100  CPU: 0   COMMAND: "insmod"
 #0 [ffff945875ca7888] machine_kexec at ffffffff93e63674
 #1 [ffff945875ca78e8] __crash_kexec at ffffffff93f1ce12
 #2 [ffff945875ca79b8] crash_kexec at ffffffff93f1cf00
 #3 [ffff945875ca79d0] oops_end at ffffffff9456c758
 #4 [ffff945875ca79f8] die at ffffffff93e2f95b
 #5 [ffff945875ca7a28] do_general_protection at ffffffff9456c052
 #6 [ffff945875ca7a60] general_protection at ffffffff9456b6f8
    [exception RIP: deactivate_slab+164]
    RIP: ffffffff94019fe4  RSP: ffff945875ca7b10  RFLAGS: 00010006
    RAX: 00000000000001f8  RBX: ffffd091c1f19fc0  RCX: 0000000180080006
    RDX: ffff9458bc67f200  RSI: ffffd091c1f19fc0  RDI: 0000000040020000
    RBP: ffff945875ca7bc0   R8: ffff9458bc67f200   R9: 0000000180080006
    R10: ffff9458c2401500  R11: ffffffff9417551f  R12: ffff9458bc67f200
    R13: 11ff9458bc67f400  R14: ffff9458bef8f840  R15: ffff9458bcdbd100
    ORIG_RAX: ffffffffffffffff  CS: 0010  SS: 0018
 #7 [ffff945875ca7bc8] flush_cpu_slab at ffffffff9401a386
 #8 [ffff945875ca7be8] on_each_cpu_mask at ffffffff93f118ae
 #9 [ffff945875ca7c18] on_each_cpu_cond at ffffffff93f11994
#10 [ffff945875ca7c70] kmem_cache_close at ffffffff9401e5a5
#11 [ffff945875ca7cd0] __kmem_cache_shutdown at ffffffff9401eb34
#12 [ffff945875ca7cf0] kmem_cache_destroy at ffffffff93fdb8f0
#13 [ffff945875ca7d10] corrupter_slab_doit at ffffffffc06010ee [corrupter_TEST_B_CASE_3c]
#14 [ffff945875ca7d28] init_module at ffffffffc023f037 [corrupter_TEST_B_CASE_3c]
#15 [ffff945875ca7d38] do_one_initcall at ffffffff93e0210a
#16 [ffff945875ca7d68] load_module at ffffffff93f18f8c
#17 [ffff945875ca7eb8] sys_finit_module at ffffffff93f19606
#18 [ffff945875ca7f50] system_call_fastpath at ffffffff94574ddb
    RIP: 00007f4f7998c1c9  RSP: 00007fff480a2b08  RFLAGS: 00010203
    RAX: 0000000000000139  RBX: 00000000024742b0  RCX: 00007f4f799fdfd0
    RDX: 0000000000000000  RSI: 000000000041a94e  RDI: 0000000000000003
    RBP: 000000000041a94e   R8: 0000000000000000   R9: 00007fff480a2d18
    R10: 0000000000000003  R11: 0000000000000206  R12: 0000000000000000
    R13: 0000000002474270  R14: 0000000000000000  R15: 0000000000000000
    ORIG_RAX: 0000000000000139  CS: 0033  SS: 002b
~~~

~~~
crash> dis -l deactivate_slab+164
/usr/src/debug/kernel-3.10.0-957.el7/linux-3.10.0-957.el7.x86_64/mm/slub.c: 241
0xffffffff94019fe4 <deactivate_slab+164>:       mov    0x0(%r13,%rax,1),%rcx
~~~

~~~
crash> struct kmem_cache.name ffff9458bcdbd100
  name = 0xffff945898af4940 "corrupter_slab_cache"
~~~

~~~
crash> struct kmem_cache.cpu_slab ffff9458bcdbd100
  cpu_slab = 0x30f7fd2010a0
crash> struct kmem_cache_cpu.freelist 0x30f7fd2010a0:0
[0]: ffffc550bfc010a0
  freelist = 0xffff9458bc67f000
~~~

~~~
crash> rd -x 0xffff9458bc67f000 512
ffff9458bc67f000:  8888888888888888 8888888888888888 
[...]
ffff9458bc67f1f0:  8888888888888888 0000000000000000 
ffff9458bc67f200:  1111111111111111 1111111111111111 
[...]
ffff9458bc67f3f0:  1111111111111111 ffff9458bc67f000 
ffff9458bc67f400:  8888888888888888 8888888888888888 
[...]
ffff9458bc67f5f0:  8888888888888888 ffff9458bc67f600 
ffff9458bc67f600:  8888888888888888 8888888888888888 
~~~

- System status: kernel panic
