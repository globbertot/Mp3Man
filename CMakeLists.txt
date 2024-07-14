cmake_minimum_required(VERSION 3.5)

project(Mp3Man VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Network Widgets)

set(PROJECT_SOURCES
    window.ui main.cpp
    window.cpp window.h
    updater.cpp updater.h
    youtube.h youtube.cpp
)

qt_add_executable(Mp3Man
    ${PROJECT_SOURCES}
)

target_link_libraries(Mp3Man
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Network
)

include(GNUInstallDirs)
install(TARGETS Mp3Man
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Mp3Man)
endif()
