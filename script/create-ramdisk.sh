#!/bin/sh

SYSROOT=$(dirname $0)/../rootfs
RAMDISK=$(realpath $(dirname $0)/../ramdisk.tar)

if [ -d $SYSROOT ]; then 
    rm -r $SYSROOT
fi

if [ -f $RAMDISK ]; then 
    rm $RAMDISK 
fi

mkdir -p $SYSROOT/bin 
mkdir -p $SYSROOT/etc
mkdir -p $SYSROOT/home/root
mkdir -p $SYSROOT/home/user

echo "Hello World" > $SYSROOT/home/root/hello.txt
echo "Goodbye" > $SYSROOT/home/user/dodo.txt
echo "Heil Stan" > $SYSROOT/home/user/bad.txt

cd $SYSROOT
tar -cf $RAMDISK *
