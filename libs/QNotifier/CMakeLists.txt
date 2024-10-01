cmake_minimum_required(VERSION 3.10)
project(QNotifier)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(QNOTIFIER_SOURCES
    QNotifier.h QNotifier.cpp
    QChoiceNotifier.h QChoiceNotifier.cpp
    styles.h
)

add_library(QNotifier SHARED ${QNOTIFIER_SOURCES})
target_link_libraries(QNotifier Qt6::Widgets)

target_include_directories(QNotifier PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
