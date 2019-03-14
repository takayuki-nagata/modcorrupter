obj-m += kmod_corrupter_TEST-A-1.o
obj-m += kmod_corrupter_TEST-A-2.o
obj-m += kmod_corrupter_TEST-A-3a.o
obj-m += kmod_corrupter_TEST-A-3b.o
obj-m += kmod_corrupter_TEST-A-3c.o
kmod_corrupter_TEST-A-1-objs := corrupter_main.o corrupter_TEST-A-1.o
kmod_corrupter_TEST-A-2-objs := corrupter_main.o corrupter_TEST-A-2.o
kmod_corrupter_TEST-A-3a-objs := corrupter_main.o corrupter_TEST-A-3a.o
kmod_corrupter_TEST-A-3b-objs := corrupter_main.o corrupter_TEST-A-3b.o
kmod_corrupter_TEST-A-3c-objs := corrupter_main.o corrupter_TEST-A-3c.o

obj-m += kmod_corrupter_TEST-B-1.o
obj-m += kmod_corrupter_TEST-B-2.o
obj-m += kmod_corrupter_TEST-B-3a.o
obj-m += kmod_corrupter_TEST-B-3b.o
obj-m += kmod_corrupter_TEST-B-3c.o
kmod_corrupter_TEST-B-1-objs := corrupter_main.o corrupter_TEST-B-1.o
kmod_corrupter_TEST-B-2-objs := corrupter_main.o corrupter_TEST-B-2.o
kmod_corrupter_TEST-B-3a-objs := corrupter_main.o corrupter_TEST-B-3a.o
kmod_corrupter_TEST-B-3b-objs := corrupter_main.o corrupter_TEST-B-3b.o
kmod_corrupter_TEST-B-3c-objs := corrupter_main.o corrupter_TEST-B-3c.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
