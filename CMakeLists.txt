cmake_minimum_required(VERSION 3.0.0)
project(tetrix LANGUAGES C VERSION 0.1.0)

include(CTest)
enable_testing()

# Crea il modulo tetrix e aggiunge l'eseguibile main.c
add_executable(tetrix src/main.c)
add_executable(tester src/tester.c)
# Crea il modulo libtetrix e aggiunge i file come librerie
add_library(libtetrix
    src/gamefield.c
    src/player.c
    src/singleplayer.c
    src/multiplayer.c
    src/tetrimino.c
    src/functions.c
    src/constants.c
    src/bot.c
)

# Collega il modulo libetetrix a tetrix
target_link_libraries(tetrix libtetrix)
target_link_libraries(tester libtetrix)
# Permette di includere nel progetto la cartella "include"
include_directories(include)

# Specifica la versione ANSI C per i moduli
set_property(TARGET tetrix PROPERTY C_STANDARD 90)
set_property(TARGET libtetrix PROPERTY C_STANDARD 90)

# Importa la libreria ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(tetrix ${CURSES_LIBRARIES})
target_link_libraries(tester ${CURSES_LIBRARIES})

# Test valgrind
add_test(NAME mem_check COMMAND valgrind ./tetrix)

file(GLOB SOURCES "*.c")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
