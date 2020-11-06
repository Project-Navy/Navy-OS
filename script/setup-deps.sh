#!/bin/sh

# Compile and install echfs
cd ./third-party/echfs
make -j12 all
sudo make install

# Compile limine-install.c
cd ../limine
gcc limine-install.c -o limine-install
