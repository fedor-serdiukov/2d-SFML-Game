cmake_minimum_required(VERSION 3.29)
project(oop_project_game)

set(CMAKE_CXX_STANDARD 20)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(oop_project_game main.cpp
        Enemy.cpp
        Player.cpp
        Field.cpp
        Cell.cpp
        EnemyBuilding.cpp)

target_link_libraries(oop_project_game sfml-graphics sfml-window sfml-system)

