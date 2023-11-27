cmake_minimum_required(VERSION 3.14)

project(tetrisky-qt LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core LinguistTools)

set(TS_FILES tetrisky-qt_en_US.ts)

add_executable(tetrisky-qt
  main.cpp
  ${TS_FILES}
)
target_link_libraries(tetrisky-qt Qt${QT_VERSION_MAJOR}::Core ncurses)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

install(TARGETS tetrisky-qt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
