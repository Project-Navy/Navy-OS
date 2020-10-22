.SUFFIXES:
.DEFAULT_GOAL := all

BUILD_DIRECTORY = $(shell pwd)/build
DIRECTORY_GUARD=@mkdir -p $(@D)

BUILD_ARCH ?= x86_32

CFLAGS := \
    -MD			                \
    -Wall		                \
    -Wextra		                \
    -Werror                     \
    -I.                         \
    -Ilib/multiboot             \
	-Ilib/libc					\
	-Ilib						\
    -ffreestanding	            \
    -g			                \
	-ansi						\
    -nostdlib			        \
    -pedantic                   \
    -pedantic-errors            

LDFLAGS :=

include arch/$(BUILD_ARCH)/.build.mk
include kernel/.build.mk

all: $(KERNEL_BINARY) 

format:
	sh ./format.sh
	
run-qemu: 
	sh ./make-iso.sh
	qemu-system-x86_64 -cdrom navy.iso -serial stdio -smp cores=4 

run-gdb:
	sh ./make-iso.sh
	qemu-system-x86_64 -s -S -cdrom navy.iso -serial stdio -smp cores=4 

run-debug:
	sh ./make-iso.sh
	qemu-system-x86_64 -cdrom navy.iso -serial stdio -smp cores=4 --no-shutdown --no-reboot

run-bochs:
	sh ./make-iso.sh
	bochs -q

clean:
	rm -rf build/

.PHONY: run-bochs
.PHONY: run-qemu
.PHONY: run-gdb
.PHONY: format 
.PHONY: all
.PHONY: clean
