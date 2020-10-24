#!/bin/sh 

path=""
echo $(dirname $0)
if [ $(dirname $0) = "." ]; then 
    path="." 
    otherpath=".."
else 
    path=".."
    otherpath="."
fi

$(dirname $0)/iso-grub.sh -r

if [ $1 = "-d" ]; then
    qemu-system-i386 -s -S -drive format=raw,file=$otherpath/navy.iso -serial stdio -m 128M -enable-kvm -display sdl 
else
    qemu-system-i386 -drive format=raw,file=$otherpath/navy.iso -serial stdio -m 128M -enable-kvm -display sdl 
fi

