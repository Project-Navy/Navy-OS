#!/bin/sh

path="$(dirname $0)/../"

if [ -d isodir ]; then
		rm -r isodir
fi

mkdir -p isodir/boot/grub

cat << EOF > isodir/boot/grub/grub.cfg
set timeout=0

menuentry "Navy" {
	multiboot2 /boot/navy.elf
	module2 /boot/ramdisk.tar ramdisk
}
EOF

$(dirname $0)/create-ramdisk.sh

cp $path/kernel.elf isodir/boot/navy.elf
cp $path/ramdisk.tar isodir/boot/ramdisk.tar
grub-mkrescue -o $path/navy.iso isodir
rm -rf isodir
