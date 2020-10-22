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

$(dirname $0)/disk-limine.sh -r
qemu-system-x86_64 -drive format=raw,file=$otherpath/navy.img -serial stdio

