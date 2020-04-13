cmake_minimum_required(VERSION 3.10)

project(MADEVIL)

add_compile_options(-std=c++17 -Wall -O3)


add_executable(madevil
    src/main.cpp
    src/game.cpp
    src/fx.cpp
    )

target_link_libraries(madevil
    -lSDL2
    -lSDL2_image
    )
