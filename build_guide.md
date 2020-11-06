# Prequired
Global:
- Make
- Fuse

For cross-compiler:
- wget
- Bison
- Flex
- GMP
- MPC
- MPFR
- Texinfo

# Setup environment
Before doing anything, you have to recursively clone the repository of the project.
```bash
$ git clone --recursive https://github.com/Project-Navy/NavyOS
$ cd NavyOS/
```

NavyOS depends on some tools to work properly. You must setup them before trying to compile the system.
```bash
$ ./script/build-limine.sh
```
*Note: If the compilation of echfs fails try to install a newer version of fuse.*

# Build a cross-compiler
*Note: It is possible that you have already compiled a cross-compiler targeting the specified platform. If it is the case, please go directly to the _Build system and run it on QEMU_ section.*


*Note: You may want to compile the OS with another version of the compiler. To do so, don't forget to change the version number of the extracted folders.*

To compile NavyOS, you will firstly need to get access to a cross-compiler targeting on the supported platform (i686-elf). The compiler used during the development of the project is GCC (10.2.0). The compiler's compiling process hardly depends on another feature provided by GNU, which is Binutils (2.35). Follow these instructions to get a properly working cross-compiler:

## Download and extract the compressed folders of Binutils and GCC
```bash
$ wget https://ftp.gnu.org/gnu/binutils/binutils-2.35.tar.xz
$ wget https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.xz
$ for i in ./*.tar.xz; do tar -xf $i; done
```
Download projects from the main GNU mirrors. For each ".tar.xz" files, extract it.

## Setup the compilation of Binutils and GCC
*Note: It is possible to install the cross-compiler in another folder. One more time, adapt the guide to your situation.*

Create a folder where the cross compiler will be installed and add its `bin` folder in PATH, to directly use them do:
```bash
$ mkdir /opt/cross-compiler
$ export PATH="/opt/cross-compiler/bin/:$PATH"
```

Create a folder where you will compile the sources and configure it to target i686-elf platform, for the C language.
```bash
$ mkdir build_binutils/
$ mkdir build_gcc/
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

## Compile Binutils and GCC
Compile the sources with `make` and install it to the previously given directory (/opt/cross-compiler).
```bash
$ cd ../build_binutils
$ make -j$(nproc)
$ make install
```

```bash
$ cd ../build_gcc
$ make -j$(nproc) all-gcc
$ make install-gcc
```

# Build the system and run it on QEMU
Compile the system and run it in 32bits with the limine bootloader.
```bash
$ make -j$(nproc)
$ ./script/run-32bits-limine.sh
```

If you prefer Grub, you can run instead 
```bash
$ make -j$(nproc)
$ ./script/run-32bits-grub.sh 
```

Now, enjoy :)


