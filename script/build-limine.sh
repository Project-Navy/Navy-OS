#!/bin/sh 

path="$(dirname $0)/../"

make -C $path/third-party/limine -j $(nproc)
make -C $path/third-party/limine limine-install -j $(nproc)
make -C $path/third-party/echfs -j $(nproc)

