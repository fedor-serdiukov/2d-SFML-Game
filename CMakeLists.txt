cmake_minimum_required(VERSION 3.29)
project(oop_project_game)

set(CMAKE_CXX_STANDARD 20)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(oop_project_game main.cpp
        Enemy.cpp
        Player.cpp
        Cell.cpp
        EnemyBuilding.cpp
        Hand.cpp
        AreaDamageSpell.cpp
        DirectDamageSpell.cpp
        Field.cpp
#        Trap.cpp
#        TrapSpell.cpp
#        TrapSpell.h)
)

target_link_libraries(oop_project_game sfml-graphics sfml-window sfml-system)

