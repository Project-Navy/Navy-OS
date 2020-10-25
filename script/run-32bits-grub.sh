#!/bin/sh 

path="$(dirname $0)/../"

$(dirname $0)/iso-grub.sh 
qemu-system-i386 -drive format=raw,file=$path/navy.iso -serial stdio -m 128M -enable-kvm -display sdl 

