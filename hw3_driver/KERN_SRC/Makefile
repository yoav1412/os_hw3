obj-m := message_slot.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: modules
	gcc -O3 -std=c99 message_reader.c -o message_reader
	gcc -O3 -std=c99 message_sender.c -o message_sender
	
modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
