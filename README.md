<div id="top"></div>

# RSH (Raccoon Shell) - A simple shell written in c

## Description

RSH is a simple shell written in c following [Stephen Brennan's](https://brennan.io) LSH tutorial ["Write a Shell in C"](https://brennan.io/2015/01/16/write-a-shell-in-c/). This is not a feature rich shell and is in no way a replacement for mature shells like bash or zsh.

## Build

You can build rsh using cmake, first verify that cmake and a c compiler is installed.

```sh
cmake --version

#cmake version 3.30.0
#
#CMake suite maintained and supported by Kitware (kitware.com/cmake).

clang --version

#Apple clang version 15.0.0 (clang-1500.3.9.4)
#Target: arm64-apple-darwin23.5.0
#Thread model: posix
#InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
```

now run the steps below to compile and run the code

```sh
mkdir build
cd build

cmake ..
make

./rsh
```

## License

Gossht is licensed under the [MIT License](https://opensource.org/license/mit).
