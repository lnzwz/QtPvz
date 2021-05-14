cmake_minimum_required(VERSION 3.5)

project(test LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_COMPLIER clang++)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -std=c++20")


# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

if(ANDROID)
     set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
endif()

find_package(QT NAMES Qt5 COMPONENTS Widgets LinguistTools opengl REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES test_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        stdafx.h
        icons.qrc
        choose.cpp
        choose.h
        choose.ui
        ${TS_FILES}
)

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 5)
#    add_executable(test
#        ${PROJECT_SOURCES}
#    )

#    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
#else()
    if(ANDROID)
        add_library(test SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(test
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
# endif()

target_link_libraries(test PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# find_package(Qt6 COMPONENTS OpenGLWidgets REQUIRED)
# target_link_libraries(test PRIVATE Qt6::OpenGLWidgets)

