cmake_minimum_required(VERSION 3.5)

project(TPics3 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport  REQUIRED)

if(ANDROID)
  add_library(TPics3 SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

  )
else()
  add_executable(TPics3
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    pixel_table/pixel_table.cpp
    pixel_table/pixel_table.h
    control_panel.cpp
    control_panel.h
    control_panel.ui
    qcustomplot.cpp
    qcustomplot.h
    displaywindow.cpp
    displaywindow.h
    displaywindow.ui
    animation.cpp
    animation.h
    animationwindow.cpp
    animationwindow.h
    animationwindow.ui
    pixel_table/tot_avg_window.cpp
    pixel_table/tot_avg_window.h
    pixel_table/tot_avg_window.ui
  )
endif()

target_link_libraries(TPics3 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport )
