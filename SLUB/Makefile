obj-m += TEST-0/
obj-m += TEST-A/
obj-m += TEST-B/

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
