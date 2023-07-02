cmake_minimum_required(VERSION 3.0.0)
project(cpu_usage_tracker VERSION 0.1.0 LANGUAGES C)

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set (CMAKE_C_FLAGS "-Weverything")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_C_FLAGS "-Wall -Wextra")
endif()

include(CTest)

enable_testing()
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_executable(cpu_usage_tracker main.c source/reader.c source/analyzer.c source/printer.c source/watchdog.c source/logger.c)
target_link_libraries(cpu_usage_tracker ${CURSES_LIBRARIES})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
