#!/bin/sh 

path=""
if [ $(dirname $0) = "." ]; then 
    path=".." 
else 
    path="."
fi


make -C $path/third-party/limine
make -C $path/third-party/limine limine-install
make -C $path/third-party/echfs 

