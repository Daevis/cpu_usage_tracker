cmake_minimum_required(VERSION 3.0.0)
project(cpu_usage_tracker VERSION 0.1.0 LANGUAGES C)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set (CMAKE_C_FLAGS "-Weverything")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_C_FLAGS "-Wall -Wextra")
endif()

include(CTest)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED On)

enable_testing()
add_subdirectory(tests)
include_directories(header)
add_subdirectory(source)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_executable(cpu_usage_tracker cpu_usage_tracker.c)
target_link_libraries(cpu_usage_tracker ${CURSES_LIBRARIES} CUT_lib)

