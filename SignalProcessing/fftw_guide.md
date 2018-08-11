# FFTW Guide

## Examples

2D DFT: http://micro.stanford.edu/wiki/Install_FFTW3#Parallel_MPI

## Basics

```c++
typedef double fftw_complex[2];
```

## Build

cmake build is experimental. It works for NDK on Windows. It doesn't support enabling neon.

### Android NDK build

Based on [this guide](http://rajanyadhar.blogspot.com/2017/06/it-and-fftw-integration-with-android-ndk.html) with modication to work on Ubuntu

http://www.fftw.org/fftw3_doc/Installation-on-Unix.html#Installation-on-Unix

```sh
# NOTE: this assumes aarch64 build
export NDK_ROOT="/mnt/sd/qgao/android-ndk-r17b/"
# aarch64
export ARCH_NAME="aarch64-linux-android"
export ARCH_VER="4.9"
export PATH="$NDK_ROOT/toolchains/${ARCH_NAME}-${ARCH_VER}/prebuilt/linux-x86_64/bin/:$PATH"
echo $PATH | tr ":" "\n"
export SYS_ROOT="$NDK_ROOT/platforms/android-27/arch-arm64/"
export CC="${ARCH_NAME}-gcc --sysroot=$SYS_ROOT"
export LD="${ARCH_NAME}-ld"
export AR="${ARCH_NAME}-ar"
export RANLIB="${ARCH_NAME}-ranlib"
export STRIP="${ARCH_NAME}-strip"
export CFLAGS_INC_DIR="-I$NDK_ROOT/sysroot/usr/include/"
export CFLAGS_INC_DIR="-I$NDK_ROOT/sysroot/usr/include/x86_64-linux-android/ $CFLAGS_INC_DIR"
export CFLAGS_INC_DIR="-I$NDK_ROOT/toolchains/${ARCH_NAME}-${ARCH_VER}/prebuilt/linux-x86_64/lib/gcc/aarch64-linux-android/4.9.x/include/ $CFLAGS_INC_DIR"
 # location to install the lib
export DEST_DIR="$NDK_ROOT/toolchains/${ARCH_NAME}-${ARCH_VER}/prebuilt/linux-x86_64/user/"

# PIE: need to create Position Indep Exe as required by Android 5+ "-mfloat-abi=softfp -mfpu=neon
./configure --host=aarch64 --prefix=$DEST_DIR LIBS="-lc -lgcc" CFLAGS="$CFLAGS_INC_DIR -fPIE" LDFLAGS="-pie" --disable-fortran #--enable-neon 
```

Issues: 
- [neon version is much slower](https://github.com/FFTW/fftw3/issues/129)