#!/bin/sh 

path="$(dirname $0)/../"

make -C $path/third-party/limine
make -C $path/third-party/limine limine-install
make -C $path/third-party/echfs 

