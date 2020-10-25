#!/bin/sh

path="$(dirname $0)/../"

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
