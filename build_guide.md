# Build a cross-compiler
To compile NavyOS, you will firstly need to get access to a cross-compiler targeting on the supported platform (i686-elf). The compiler used during the development of the project is GCC (10.2.0). The compiler's compiling process hardly depends on another feature provided by GNU, which is Binutils (2.35). Follow these instructions to get a properly working cross-compiler:

## Prerequisites
- wget
- Make
- Bison
- Flex
- GMP
- MPC
- MPFR
- Texinfo

## Download and extract the compressed folders of Binutils and GCC
```bash
$ wget https://ftp.gnu.org/gnu/binutils/binutils-2.35.tar.xz
$ wget https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.xz
$ for i in ./*.tar.xz; do tar -xf $i; done
```
Download projects from the main GNU mirrors. For each ".tar.xz" files, extract it.

## Setup the compilation of Binutils and GCC
```bash
$ mkdir /opt/cross-compiler
$ export PATH='$PATH:/opt/cross-compiler/bin/'
```
Create a folder where the cross compiler will be installed and add its binaries folder in PATH, to directly use them.

```bash
$ mkdir build_binutils/ && mkdir build_gcc/
$ cd build_binutils
$ ../binutils-2.35/configure    \
  --prefix=/opt/cross-compiler/ \
  --target=i686-elf             \
  --enable-languages=c
```

```bash
$ cd ../build_gcc
$ ../gcc-10.2.0/configure       \
  --prefix=/opt/cross-compiler/ \
  --target=i686-elf             \
  --enable-languages=c
```
Create a folder where you will compile the sources and configure it to target i686-elf platform, for C language.

## Compile Binutils and GCC
```bash
$ cd ../build_binutils
$ make -j6
$ make install
```

```bash
$ cd ../build_gcc
$ make -j6 all-gcc
$ make install-gcc
```
Compile the sources with Make and install it to the previously given directory (/opt/cross-compiler).

# Build the system and run it on QEMU
```bash
$ make -j6 && ./run-32bits-limine.sh
```
Compile the system and run it in 32bits with the limine bootloader.


