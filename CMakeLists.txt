cmake_minimum_required(VERSION 3.20)
project(MyKeylight)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)


set(CMAKE_CXX_STANDARD 14)
add_executable(MyKeylight gui/mainwindow.ui gui/mainwindow.cpp MyKeylightCLI.cpp Keylight.cpp Keylight.h KeylightDiscover.cpp KeylightCommunication.cpp MyKeylightGUI.cpp MyKeylightMain.cpp)
target_link_libraries(MyKeylight PRIVATE curl)
target_link_libraries(MyKeylight PRIVATE Qt6::Widgets)
