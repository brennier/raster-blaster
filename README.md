# Raster Blaster: Old-School Software Rasterizer

A lightweight, software-based rasterizer with minimal dependencies. Originally built for Hakuya, my PS1 emulator. This project aims to be strictly conformant to the C99 standard with as few dependencies as possible. This project is compiled with the flags `-std=c99 -pedantic-errors` to enforce this policy, ensuring no GCC/Clang extensions creep in. SDL3 is used solely for displaying the final framebuffer output. It plays no part in the rasterization process itself.
