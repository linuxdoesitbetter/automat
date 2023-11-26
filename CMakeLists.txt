cmake_minimum_required(VERSION 3.25)

project(
    automat
    LANGUAGES C
    VERSION 1.0.0
    DESCRIPTION "Bibliothek zum Erstellen von Zustandsautomaten." )

add_library(
    automat SHARED
    src/automat.h
    src/automat.c )

add_executable(
    test-automat
    src/main.c )

target_link_libraries(
    test-automat
    automat )
