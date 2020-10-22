#!/bin/sh 

path=""
if [ $(dirname $0) = "." ]; then 
    path=".." 
else 
    path="."
fi

if [ -f $path/navy.img ]; then 
    rm -rf $path/navy.img
fi

dd if=/dev/zero bs=1M count=0 seek=64 of=$path/navy.img
parted -s $path/navy.img mklabel msdos 
parted -s $path/navy.img mkpart primary 2048s 100% 
$path/third-party/echfs/echfs-utils -m -p0 $path/navy.img quick-format 512 
$path/third-party/echfs/echfs-utils -m -p0 $path/navy.img import $path/limine.cfg limine.cfg 
$path/third-party/echfs/echfs-utils -m -p0 $path/navy.img import $path/kernel.elf kernel.elf 
$path/third-party/limine/limine-install $path/third-party/limine/limine.bin $path/navy.img 
