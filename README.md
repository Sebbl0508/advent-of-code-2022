# Advent of Code 2022
In C!  

> :warning: **WARNING**: Highly inefficient code

## Compatibility
This will not work when compiling with MSVC, because it doesn't support some libc functions (`strsep`, etc.)

## How to run?
These are instructions for UNIX systems, if you use windows, go figure it out yourself  
### Requirements
- GCC / Clang or any compatible C compiler
- `cmake`
- `Make` or `Ninja` or any other `cmake` generator

```shell
mkdir build
cd build
cmake ..
cmake --build .
```