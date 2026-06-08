# Raster Blaster is a an old-school software rasterizer

This is a software-based rasterizer with minimal dependencies. It's original purpose is for my PS1 emulator called Hakuya. Like that project, this project is also written in strictly conformat C99 with as few libraries as possible. The flags `-std=c99 -pedantic-errors` are used to ensure that no GCC/Clang extensions are used. At the moment, SDL3 is used displaying for displaying the final result, but it's not used at any point during the rasterization process.

