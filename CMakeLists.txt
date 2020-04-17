cmake_minimum_required(VERSION 3.10)

project(MADEVIL)

add_compile_options(-std=c++17 -Wall -O3)

find_package(SDL2 REQUIRED)

add_executable(madevil
    src/main.cpp
    src/game.cpp
    src/enemy.cpp
    src/hero.cpp
    src/fx.cpp
    )

target_include_directories(madevil PRIVATE
    ${SDL2_INCLUDE_DIRS}
    )

target_link_libraries(madevil
    -lSDL2
    -lSDL2_image
    )
