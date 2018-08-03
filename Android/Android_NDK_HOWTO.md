# NDK structure:

`C:\Apps\Android\sdk\ndk-bundle\prebuilt\windows-x86_64\bin`: make.exe, gdb.exe
`C:\Apps\Android\sdk\ndk-bundle\toolchains\aarch64-linux-android-4.9\prebuilt\windows-x86_64\bin`: C++ compiler and etc.

# Build Command-line NDK app

```dos
set CMAKE=C:\Apps\Android\sdk\cmake\3.6.4111459\bin\cmake.exe
set  MAKE=C:\Apps\Android\sdk\cmake\3.6.4111459\bin\ninja.exe
set ANDROID_NDK_HOME=C:\Apps\Android\sdk\ndk-bundle
set CMAKE_TC=%ANDROID_NDK_HOME%\build\cmake\android.toolchain.cmake

%CMAKE% -DCMAKE_TOOLCHAIN_FILE=%CMAKE_TC% -DANDROID_ABI=arm64-v8a -DCMAKE_MAKE_PROGRAM=%MAKE% -G Ninja ..

REM use ninja to make the build
C:\Apps\Android\sdk\cmake\3.6.4111459\bin\ninja.exe
```

# Background

## `ndk-build` & `gradle`

```dos
set ANDROID_HOME=C:\Apps\Android\sdk
set ANDROID_NDK_HOME=C:\Apps\Android\sdk\ndk-bundle
set JAVA_HOME="C:\Apps\Android\Android Studio\jre"
gradlew.bat assembleDebug
```

## `ndk-build`
you need to have at least two makefiles: 
- `Android.mk`: it is similar to the makefiles. In this file you can define
  - the source files to build, the header include directories, compiler flag options, libraries and their locations, the module name, and much more. 
- `Application.mk` is for specifying Android-specific properties, such as the target Android platform, SDK version, and platform architecture.

# `cmake`

## Concepts
Android Studio saves the build arguments it uses for executing a CMake build, in a `cmake_build_command.txt` file (see [here](https://developer.android.com/ndk/guides/cmake)).
```dos
Executable : C:\Apps\Android\sdk\cmake\3.6.4111459\bin\cmake.exe
arguments : 
-HD:\code\android-ndk-samples\hello-jni\app\src\main\cpp
-BD:\code\android-ndk-samples\hello-jni\app\.externalNativeBuild\cmake\arm8Debug\arm64-v8a
-DANDROID_ABI=arm64-v8a
-DANDROID_PLATFORM=android-23
-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=D:\code\android-ndk-samples\hello-jni\app\build\intermediates\cmake\arm8\debug\obj\arm64-v8a
-DCMAKE_BUILD_TYPE=Debug
-DANDROID_NDK=C:\Apps\Android\sdk\ndk-bundle
-DCMAKE_TOOLCHAIN_FILE=C:\Apps\Android\sdk\ndk-bundle\build\cmake\android.toolchain.cmake
-DCMAKE_MAKE_PROGRAM=C:\Apps\Android\sdk\cmake\3.6.4111459\bin\ninja.exe
-GAndroid Gradle - Ninja
jvmArgs : 
```

Android Studio cmake: `C:\Apps\Android\sdk\cmake\3.6.4111459\bin`
[`ninja`](https://www.wikiwand.com/en/Ninja_(build_system)): meant to replace `make`

Android Studio 

[`android-cmake`](https://github.com/taka-no-me/android-cmake): lots of info on make for windows

[Build Android command line app](https://software.intel.com/en-us/articles/building-an-android-command-line-application-using-the-ndk-build-tools): Use ndk-build
