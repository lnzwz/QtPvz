cmake_minimum_required(VERSION 3.5)

if(LINUX)
    set(CMAKE_CXX_COMPLIER "clang++")
    set(CMAKE_C_COMPLIER "clang")
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pvz VERSION 0.1 LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(TS_FILES test_zh_CN.ts)

set(PROJECT_SOURCES
        stdafx.h
        icons.qrc
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        choose.cpp
        choose.h
        choose.ui
        game.h
        game_ai.cpp
        config.cpp
        draw.cpp
        game.cpp
        action.cpp
        ico.rc
        logo.icns
        mythread.h
        mythread.cpp
        choogq.h choogq.cpp choogq.ui
        other.h other.cpp other.ui
        myglwidget.h myglwidget.cpp
        ${TS_FILES}
)

qt_add_big_resources(PROJECT_SOURCES icons.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pvz
        MANUAL_FINALIZATION
        WIN32
        ${PROJECT_SOURCES}
        README.md
        trash.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET pvz APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(pvz SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(pvz
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(ANDROID)
    set_property(TARGET pvz APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/android)
endif()

target_link_libraries(pvz PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

#if(WIN32)
#    target_link_libraries(pvz PRIVATE winmm)
#endif()

if(WIN32)
    target_compile_options(pvz PRIVATE /Ox)
else()
    target_compile_options(pvz PRIVATE -O3)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
    target_link_libraries(pvz PRIVATE Qt6::OpenGLWidgets)
    find_package(OpenGL REQUIRED)
    target_link_libraries(pvz PRIVATE OpenGL::GL)
endif()

# target_precompile_headers(pvz PRIVATE stdafx.h)

set_target_properties(pvz PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER lnzwz.pvz.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set(MACOSX_BUNDLE_ICON_FILE logo.icns)

set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/logo.icns)
set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
       MACOSX_PACKAGE_LOCATION "Resources")

install(TARGETS pvz
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pvz)
endif()
