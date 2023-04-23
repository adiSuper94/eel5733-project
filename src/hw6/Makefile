
obj-m := assignment6.o

KERNEL_DIR = /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) modules
	gcc -o deadlock1 deadlock1.c -lpthread
	gcc -o deadlock2 deadlock2.c -lpthread
	gcc -o deadlock3 deadlock3.c -lpthread
	
clean:
	rm -rf *.cmd *.o *.ko *.mod.* *.symvers *.order *~
	rm deadlock1
	rm deadlock2
	rm deadlock3
