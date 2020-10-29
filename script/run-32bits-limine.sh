#!/bin/sh 

path="$(dirname $0)/../"

if [ -f $path/navy.img ]; then 
    rm navy.img
fi

$(dirname $0)/disk-limine.sh 

qemu-system-i386 -drive format=raw,file=$path/navy.img -serial stdio -m 128M -enable-kvm 

