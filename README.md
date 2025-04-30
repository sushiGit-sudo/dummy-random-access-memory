# dummy-random-access-memory
Use this in C++ as a safe and easy way to dynamically allocate memory.

To include this in your project use `#include "random-access.h"`.

To create a dram object use `dram _ram;` or `dram _ram(__size__)` or `dram _ram(__dram_obj__)`.

To use malloc you use `__type__* string = (__type__*)_ram.dmalloc(__size__)`.

To compile this with your project on linux use `gcc -shared -fPIC -c random-access.c -o random-access.so` then use `gcc __your_project__ random-access.so -o __your_exe__`.

> This module was created by (c)bitwise-studio.
