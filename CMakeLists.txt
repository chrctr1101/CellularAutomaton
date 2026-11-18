cmake_minimum_required(VERSION 3.16)
project(CellularAutomaton LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(CellularAutomaton
    main.cpp
    mainwindow.cpp mainwindow.h
    gamewindow.cpp gamewindow.h
    mapswindow.cpp mapswindow.h
    automaton.h
    graph.h
    myvector.h
    mylist.h
    myqueue.h
    mydeque.h
)

target_link_libraries(CellularAutomaton PRIVATE Qt6::Widgets)
if(WIN32)
    target_link_options(CellularAutomaton PRIVATE -mwindows)
endif()