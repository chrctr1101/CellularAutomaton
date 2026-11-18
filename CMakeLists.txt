cmake_minimum_required(VERSION 3.16)
project(CellularAutomaton LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- Основное приложение ---
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

# --- Настройка Google Test ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# --- Настройка Google Test ---
find_package(GTest REQUIRED)

# --- Тесты ---
enable_testing()
add_executable(unit_tests tests.cpp)

# Явно линкуем и gtest, и gtest_main
target_link_libraries(unit_tests PRIVATE GTest::gtest GTest::gtest_main)