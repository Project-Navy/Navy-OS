#!/bin/sh 

path=""
if [ $(dirname $0) = "." ]; then 
    path="." 
    otherpath=".."
else 
    path=".."
    otherpath="."
fi

$path/iso-grub.sh
qemu-system-x86_64 -drive format=raw,file=$otherpath/navy.iso -serial stdio
