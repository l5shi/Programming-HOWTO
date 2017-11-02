# Dynamic Library HOWTO

On both Windows and Linux:
> - Each process has its own address space, meaning that there is never any memory being shared between processes (unless you use some inter-process communication library or extensions).
>- The One Definition Rule (ODR) still applies, meaning that you can only have one definition of the global variable visible at link-time (static or dynamic linking).

[Excellent explanation on SO](https://stackoverflow.com/questions/19373061/what-happens-to-global-and-static-variables-in-a-shared-library-when-it-is-dynam)
> Windows and Unix-like systems are completely different
>- On Windows, you have to explicitly **export** functions and global variable
```c++
#ifdef COMPILING_THE_DLL
#define MY_DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define MY_DLL_EXPORT extern "C" __declspec(dllimport)
#endif
```
MY_DLL_EXPORT int my_global;
>- On Linux, so exports all extern global variables (or functions). Basically, Unix-like systems are designed to make it so that there is virtually no difference between linking with a static or a dynamic library.

## DLL

https://www.tutorialspoint.com/dll/dll_writing.htm
https://docs.microsoft.com/en-us/cpp/cpp/using-dllimport-and-dllexport-in-cpp-classes

## so

gcc options
- `-shared`
Produce a shared object which can then be linked with other objects to form an executable. Not all systems support this option. For predictable results, you must also specify the same set of options that were used to generate code (`-fpic`, `-fPIC`, or model suboptions) when you specify this option.
- `-fPIC`: If supported for the target machine, emit position-independent code (PIC), suitable for dynamic linking and avoiding any limit on the size of the global offset table. This option makes a difference on the m68k, PowerPC and SPARC .
Position-independent code requires special support, and therefore works only on certain machines.