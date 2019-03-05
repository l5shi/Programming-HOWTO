# Install

## Install LLVM

Do NOT use llvm7.0. Use llvm 6.0

### Pre-build Binaries

- [Ubuntu18.04: use apt to install clang/llvm 6.0](https://askubuntu.com/questions/1058534/installing-clang-6-0-on-ubuntu-18-04-lts-bionic)

- **Windows** pre-built binary doesn't have llvm-config. Build from source instead.

### Build from Source
*Steps*
- git clone llvm from github and switch to branch release_60
- git clone clang from github to llvm\tools and switch to branch release_60
- run cmake: see Halide github page for more details

```batch
cmake^
  -DCMAKE_INSTALL_PREFIX=../llvm-install^
  -DLLVM_ENABLE_TERMINFO=OFF^
  -DLLVM_TARGETS_TO_BUILD=X86;ARM;NVPTX;AArch64;Hexagon^
  -DLLVM_ENABLE_ASSERTIONS=ON^
  -DLLVM_BUILD_32_BITS=OFF^
  -DCMAKE_BUILD_TYPE=Release^
  -Thost=x64^
  .. -G "Visual Studio 15 2017 Win64"
  
REM launch VS command window
MSBuild.exe /m /t:Build /p:Configuration=Release .\INSTALL.vcxproj
```

## Build Halid

Define environment variable LLVM_CONFIG and CLANG set or add llvm-config and clang to your PATH.

### Compilation on Linux

**Use clang to build Halide**
```shell
# select clang as the default compile
update-alternatives --config c++
update-alternatives --config cc

# run cmake to create makefile. The Makefile coming with Halide is for g++
```

```shell
sudo apt-get install libpng-dev
sudo apt-get install zlib1g-dev  # not needed
sudo apt-get install libjpeg-dev

#terminfo issue https://github.com/halide/Halide/issues/1112
#install libtinfo from the AUR and inserting -ltinfo alongside each instance of -lpthread
# updating apps/support/Makefile.inc works for me
sudo apt-get install libtinfo-dev

```

### Compilation with Visual Studio 2017

```shell
cmake -DLLVM_DIR=<llvm_install_dir>/lib/cmake/llvm -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 15 2017 Win64" <halide_src_dir>
```

#### Issues

##### `__cpuidex`: identifier not found (this issue has been closed)

See https://github.com/halide/Halide/issues/3254
> Adding `#include<intrin.h>` to `src/Target.cpp` seems to fix the problem (see below):
```c++
#ifdef _MSC_VER
#include<intrin.h>
static void cpuid(int info[4], int infoType, int extra) {
    __cpuidex(info, infoType, extra);
}
#else
```

##### `unresolved external symbol __imp__fprintf and __imp____iob_func`
[ Stackoverflow post on this](https://stackoverflow.com/questions/30412951/unresolved-external-symbol-imp-fprintf-and-imp-iob-func-sdl2)
- Add `legacy_stdio_definitions.lib` to vcxproj
- Make a dumb lib `iob.lib` at `lib\Release` inside Halide's build folder (create `lib\Release` first) and add it to vcxproj
```c++
FILE _iob[] = { *stdin, *stdout, *stderr }; 
//extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
extern "C" FILE * __cdecl __imp___iob_func(void) { return _iob; }
```

Scripts to update all vcxproj files:
```shell
grep -lrZ --include="*vcxproj" jpeg.lib * | xargs -0 sed -i 's/jpeg.lib;/jpeg.lib;iob.lib;legacy_stdio_definitions.lib;/g'

# use iob.lib's absolute path if it is NOT in any lib path
grep -lrZ --include="*vcxproj" jpeg.lib * | xargs -0 sed -i 's/jpeg.lib;/jpeg.lib;c:\\\\lib\\\\iob.lib;legacy_stdio_definitions.lib;/g'
```
Then build it inside Visual Studio x64 native tool console
```
MSBuild.exe /m /t:Build /p:Configuration=Release .\ALL_BUILD.vcxproj
```
