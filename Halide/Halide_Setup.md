
# Install

```shell
llvm/clang from github mirror
http://llvm.org/docs/GettingStarted.html#checkout

```

## Compilation on Linux

```shell
sudo apt-get install libpng-dev
sudo apt-get install zlib1g-dev  # not needed
sudo apt-get install libjpeg-dev

#terminfo issue https://github.com/halide/Halide/issues/1112
#install libtinfo from the AUR and inserting -ltinfo alongside each instance of -lpthread
# updating apps/support/Makefile.inc works for me
sudo apt-get install libtinfo-dev

```

## Compilation with Visual Studio 2017

### Issues

#### `__cpuidex`: identifier not found

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

#### `unresolved external symbol __imp__fprintf and __imp____iob_func`
[ Stackoverflow post on this](https://stackoverflow.com/questions/30412951/unresolved-external-symbol-imp-fprintf-and-imp-iob-func-sdl2)
- Add `legacy_stdio_definitions.lib` to vcxproj
- Make a dumb lib `iob_func.lib` at `lib\Release` and add it to vcxproj
```c++
FILE _iob[] = { *stdin, *stdout, *stderr }; 
//extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
extern "C" FILE * __cdecl __imp___iob_func(void) { return _iob; }
```

Scripts to update all vcxproj files:
```shell
grep -lrZ --include="*vcxproj" jpeg.lib * | xargs -0 sed -i 's/jpeg.lib;/jpeg.lib;legacy_stdio_definitions.lib;/g'
```