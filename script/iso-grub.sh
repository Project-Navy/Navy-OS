#!/bin/sh

path=""
if [ $(dirname $0) = "." ]; then 
    path=".." 
else 
    path="."
fi

if [ -d isodir/ ]; then
		rm -rf isodir/
fi

mkdir -p isodir/boot/grub

cat << EOF > isodir/boot/grub/grub.cfg
set timeout=0

menuentry "Navy" {
	multiboot2 /boot/navy.elf
}
EOF

cp $path/kernel.elf isodir/boot/navy.elf
grub-mkrescue -o $path/navy.iso isodir
rm -rf isodir
