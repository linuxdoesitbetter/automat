cmake_minimum_required(VERSION 3.25)

project(
    automat
    LANGUAGES C
    VERSION 1.0.0
    DESCRIPTION "Bibliothek zum Erstellen von Zustandsautomaten." )

set(
    AUTOMAT_VERSION
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

add_library(
    automat SHARED
    src/automat.h
    src/automat.c )

set_target_properties(
    automat PROPERTIES
    VERSION ${AUTOMAT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

add_executable(
    test-automat
    src/main.c )

target_link_libraries(
    test-automat
    automat )
