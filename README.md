# Kernel

-   `Kernel` - Backend, the main core of the program.

-   This project is a cross-platform `C++` project that support many operating systems such: `Windows x64+`, `Linux x64+`, `Macintosh x64+`, `iPhone iOS 10.0+`, `Android 7+`

## Development

-   [CMake 3.8+](https://cmake.org/)

-   [Powershell for Windows](https://learn.microsoft.com/en-us/powershell/)

-   [Ubuntu for Linux](https://ubuntu.com/tutorials/command-line-for-beginners)

-   [Terminal for Macintosh](https://developer.apple.com/library/archive/documentation/OpenSource/Conceptual/ShellScripting/Introduction/Introduction.html)

-   [MSVC 19.38 with MSVC for Windows](https://visualstudio.microsoft.com/downloads/)

-   [Clang 17.0 for Linux](https://llvm.org/)

-   [Clang 17.0 for Macintosh](https://llvm.org/)

-   [Clang 14.0 for Android (NDK r26.1)](https://developer.android.com/ndk/downloads)

-   [Clang 17.0 for iPhone](https://llvm.org/)

## Building for Android

To build the project as an Android dynamic library (`.so` file), follow these steps:

1.  **Prerequisites:**
    *   Install the Android NDK (Native Development Kit). You can download it from the [Android NDK website](https://developer.android.com/ndk/downloads).
    *   Ensure you have CMake installed.

2.  **Configure and Build:**
    *   Open your terminal and navigate to the root directory of this project.
    *   Run the following CMake command to configure the build. Replace `<path_to_android_ndk>` with the actual path to your Android NDK installation:

        ```bash
        cmake -B build -S . \
          -DCMAKE_TOOLCHAIN_FILE=<path_to_android_ndk>/build/cmake/android.toolchain.cmake \
          -DANDROID_ABI=arm64-v8a \
          -DANDROID_PLATFORM=android-21 \
          -DANDROID_STL=c++_shared
        ```

        *   `ANDROID_ABI`: Specifies the target Application Binary Interface. Common values include `arm64-v8a`, `armeabi-v7a`, `x86_64`, and `x86`.
        *   `ANDROID_PLATFORM`: Specifies the minimum Android API level. For example, `android-21` for Android 5.0 (Lollipop).
        *   `ANDROID_STL`: Specifies the C++ standard library to use. `c++_shared` links the shared version of libc++.

    *   After configuration, build the library:

        ```bash
        cmake --build build --target kernel
        ```

3.  **Output:**
    *   The compiled dynamic library will be located in the `build/` directory (e.g., `build/libkernel.so`). The exact path might vary based on your CMake configuration within `kernel/CMakeLists.txt`.

4.  **Using the Library in an Android Project:**
    *   Create a `jniLibs` directory in your Android project's `app/src/main/` folder.
    *   Inside `jniLibs`, create subdirectories for each ABI you built for (e.g., `arm64-v8a`, `armeabi-v7a`).
    *   Copy the respective `.so` file into the corresponding ABI subdirectory.
    *   You can then load the library in your Java/Kotlin code using `System.loadLibrary("kernel");` (assuming your library is named `libkernel.so`).
    *   You will need to define JNI (Java Native Interface) functions in your C++ code to expose functionality to your Android application.

Alternatively, you can download pre-built libraries from the [GitHub Actions artifacts](https://github.com/Haruma-VN/Sen.Environment/actions) for this repository.

## Dependencies

-   [quickjs](https://github.com/bellard/quickjs/): JavaScript Engine used by `Kernel`, for `Clang`

-   [quickjs.c_smile](https://github.com/c-smile/quickjspp/): JavaScript Engine used by `Kernel`, for `MSVC`

-   [libpng](http://www.libpng.org/pub/png/libpng.html): PNG Decode/Encode

-   [zlib](https://www.zlib.net/): Zlib compress/uncompress

-   [fmt](https://github.com/fmtlib/fmt): Format library

-   [json](https://github.com/nlohmann/json): JSON Deserialize/Serialize

-   [tinyxml2](https://github.com/leethomason/tinyxml2): XML Deserialize/Serialize

-   [simdjson](https://simdjson.org/): JSON Deserialize

-   [md5](https://github.com/JieweiWei/md5): MD5 Hash

-   [open_vcdiff](https://github.com/google/open-vcdiff): Diff two file and generate the different after compare

-   [SHA](https://github.com/pr0f3ss/SHA): SHA-384, SHA-512 Hash

-   [SHA256](https://github.com/System-Glitch/SHA256): Sha-256 Hash

-   [SHA224CryptoServiceProvider](https://github.com/redduxi/SHA-224): SHA-224 Hash

-   [Rijndael](#): Rijndael encrypt/decrypt

-   [etcpak](https://github.com/wolfpld/etcpak): ETC1 Encode algorithm

-   [zip](https://github.com/kuba--/zip): zip compress/uncompress

-   [lzma](https://www.7-zip.org/sdk.html): lzma compress/uncompress

-   [ETCPACK](https://github.com/Ericsson/ETCPACK): ETC1 Decode algorithm

-   [cpp-base64](https://github.com/ReneNyffenegger/cpp-base64): Base64 Encode/Decode

-   [compile-time-regular-expressions](https://github.com/hanickadot/compile-time-regular-expressions): Regex for modern C++, support `constexpr` regex

-   [bzip2](https://sourceware.org/bzip2): Bzip2 Compress/Uncompress

-   [avir](https://github.com/avaneev/avir): Image scaling algorithm

-   [dirent](https://github.com/tronkko/dirent): dirent for `MSVC`

-   [canvas_ity](https://github.com/a-e-k/canvas_ity/tree/main): canvas for drawing image

-   [tsl](https://github.com/Tessil/ordered-map): Actual ordered map for C++ comes with support of First-in-first-out

## Modification

-   To make sure all the libraries work perfectly fine with `Kernel`, there are some modification compare to original project. All modification will be added `Haruma :: Add`, `Haruma :: Change`, `Haruma :: Delete`

-   Special thanks to [TwinStar](https://github.com/twinkles-twinstar/) for his modification on some libraries which makes all the process easier.

-   All the modification has been made:

|                           Library                           |                    Modification                    |
| :---------------------------------------------------------: | :------------------------------------------------: |
|       [quickjs](https://github.com/bellard/quickjs/)        |         Adapt some `ES2022/ES2023` methods         |
|   [quickjs.c_smile](https://github.com/c-smile/quickjspp)   |         Adapt some `ES2022/ES2023` methods         |
|         [etcpak](https://github.com/wolfpld/etcpak)         | Added `PubDecodeETCRGB` methods in `BlockData.cpp` |
|              [simdjson](https://simdjson.org)               |     Fix issues with opening Unicode file path      |
| [canvas_ity](https://github.com/a-e-k/canvas_ity/tree/main) |      Fix some issues when assigning JS class       |
