CC := x86_64-elf-gcc 
LD := x86_64-elf-ld 
CFLAGS += -m64 -mcmodel=kernel
AS := nasm 
ASFLAGS := -f elf64
LDFLAGS += -z max-page-size=0x1000