cmake_minimum_required(VERSION 3.0.0)
project(tetrix LANGUAGES C VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(tetrix src/main.c)
file(GLOB SOURCES "*.c")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(tetrix ${CURSES_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set_property(TARGET tetrix PROPERTY C_STANDARD 90)

include(CPack)
