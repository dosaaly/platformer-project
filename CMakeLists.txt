cmake_minimum_required(VERSION 3.27)
project(platformer)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib CONFIG REQUIRED)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fsanitize=address -fsanitize=undefined")
endif()

add_executable(platformer platformer.cpp globals.h graphics.h level.h player.h enemy.h assets.h utilities.h
        level.cpp
        level_adapter.h
        level_legacy.h
        level_legacy.cpp
        enemies_controller.cpp
        enemies_controller.h
        player_manager.cpp
        player_manager.h
)
target_link_libraries(platformer PRIVATE raylib)
