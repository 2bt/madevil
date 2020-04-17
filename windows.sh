#!/bin/sh

set -e

rm -rf win
mkdir -p win

x86_64-w64-mingw32-g++ \
    -I /home/dlangner/stuff/SDL-windows/SDL2-2.0.12/x86_64-w64-mingw32/include/SDL2 \
    -I /home/dlangner/stuff/SDL-windows/SDL2_image-2.0.5/x86_64-w64-mingw32/include/SDL2 \
    -Os -s \
    src/*cpp \
    -static-libgcc -static-libstdc++ \
    -L /home/dlangner/stuff/SDL-windows/SDL2_image-2.0.5/x86_64-w64-mingw32/bin \
    -L /home/dlangner/stuff/SDL-windows/SDL2-2.0.12/x86_64-w64-mingw32/bin \
    -lSDL2 -lSDL2_image \
    -o win/madevil.exe

cp -r assets win
cp /home/dlangner/stuff/SDL-windows/SDL2-2.0.12/x86_64-w64-mingw32/bin/SDL2.dll win
cp /home/dlangner/stuff/SDL-windows/SDL2_image-2.0.5/x86_64-w64-mingw32/bin/SDL2_image.dll win
cp /home/dlangner/stuff/SDL-windows/SDL2_image-2.0.5/x86_64-w64-mingw32/bin/libpng16-16.dll win
cp /home/dlangner/stuff/SDL-windows/SDL2_image-2.0.5/x86_64-w64-mingw32/bin/zlib1.dll win

zip -r win.zip win
rsync -aviz win.zip twobit@chaossource.net:~/www/madevil/
rm -r win.zip win
