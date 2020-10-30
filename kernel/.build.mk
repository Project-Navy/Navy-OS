KERNEL_SOURCES = \
	$(wildcard kernel/*/*.c) \
	$(wildcard kernel/*.c) \
	$(wildcard arch/$(BUILD_ARCH)/*.c) \
	$(wildcard arch/$(BUILD_ARCH)/*/*.c) \

ifeq ($(BUILD_ARCH), x86_32)
	KERNEL_SOURCES  += \
		$(wildcard arch/x86/*.c) \
		$(wildcard arch/x86/*/*.c)

else ifeq ($(BUILD_ARCH), x86_64)
	KERNEL_SOURCES  += \
		$(wildcard arch/x86/*.c) \
		$(wildcard arch/x86/*/*.c)
else 
	$(error "$(BUILD_ARCH) is not supported by NavyOS (yet)")
endif

KERNEL_ASSEMBLY_SOURCES = \
	$(wildcard arch/$(BUILD_ARCH)/*.asm) \
	$(wildcard arch/$(BUILD_ARCH)/*/*.asm)

KERNEL_LIBRARIES_SOURCES = \
	$(wildcard lib/libc/*.c) \
	$(wildcard lib/multiboot/*.c) \
	$(wildcard lib/Navy/*.c) 

LIBALLOC_SOURCES = $(wildcard lib/liballoc/*.c)
	

KERNEL_BINARY = kernel.elf

KERNEL_OBJECTS = \
	$(patsubst %.c, $(BUILD_DIRECTORY)/%.o, $(KERNEL_SOURCES)) \
	$(patsubst %.asm, $(BUILD_DIRECTORY)/%.asm.o, $(KERNEL_ASSEMBLY_SOURCES)) \
	$(patsubst lib/%.c, $(BUILD_DIRECTORY)/kernel/%.o, $(KERNEL_LIBRARIES_SOURCES)) \
	$(patsubst lib/liballoc/%.c, $(BUILD_DIRECTORY)/lib/liballoc/%.o, $(LIBALLOC_SOURCES))

OBJECTS += $(KERNEL_OBJECTS)

$(BUILD_DIRECTORY)/kernel/%.o: lib/%.c
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [CC] $<
	@$(CC) $(CFLAGS) -ansi -c -o $@ $<

$(BUILD_DIRECTORY)/kernel/%.o: kernel/%.c
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [CC] $<
	@$(CC) $(CFLAGS) -ansi -c -o $@ $<

$(BUILD_DIRECTORY)/arch/%.o: arch/%.c
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [CC] $<
	@$(CC) $(CFLAGS) -ansi -c -o $@ $<

$(BUILD_DIRECTORY)/arch/%.asm.o: arch/%.asm
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [AS] $<
	@$(AS) $(ASFLAGS) $^ -o $@

$(BUILD_DIRECTORY)/lib/liballoc/%.o: lib/liballoc/%.c 
	$(DIRECTORY_GUARD)
	@echo [LIBALLOC] [CC] $<
	@$(CC) $(CFLAGS) -std=gnu11 -c -o $@ $<

$(KERNEL_BINARY): $(KERNEL_OBJECTS)
	$(DIRECTORY_GUARD)
	@echo [KERNEL] [LD] $(KERNEL_BINARY)
	@$(CC) $(LDFLAGS) -T arch/$(BUILD_ARCH)/link.ld -o $@ -ffreestanding $^ -nostdlib
