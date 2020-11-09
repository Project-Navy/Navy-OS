.SUFFIXES:
.DEFAULT_GOAL := all

BUILD_DIRECTORY = $(shell pwd)/build
DIRECTORY_GUARD=@mkdir -p $(@D)

BUILD_ARCH ?= x86_32

CFLAGS := \
    -MD			                 \
    -Wall		                 \
    -Wextra		                 \
    -Werror                      \
    -I.                          \
    -Ilib/multiboot              \
    -Ithird-party/limine/stivale \
    -Ilib/libc					 \
    -Ilib						 \
    -ffreestanding	             \
    -g			                 \
    -nostdlib			         \
    -pedantic					 \
    -Wpedantic					 \
    -ffreestanding 				 \
    -nostdlib

LDFLAGS :=

include arch/$(BUILD_ARCH)/.build.mk
include kernel/.build.mk

all: $(KERNEL_BINARY) 

format:
	sh ./format.sh

clean:
	rm -rf build/
	rm -rf kernel.elf
	rm -rf navy.*

.PHONY: run-bochs
.PHONY: run-qemu
.PHONY: run-gdb
.PHONY: format 
.PHONY: all
.PHONY: clean
