cmake_minimum_required(VERSION 3.5)

project(qt-mandelbrot VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mandelbrot.cpp
        mandelbrot.h
        renderer.cpp
        renderer.h
        image_info.cpp
        image_info.h
        image_package.cpp
        image_package.h
        mainwindow.h
        mainwindow.cpp
        position_asker.cpp
        position_asker.h
)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)
#add_compile_options(-fsanitize=undefined)
#add_link_options(-fsanitize=undefined)
#add_compile_options(-fsanitize=thread)
#add_link_options(-fsanitize=thread)

##target_compile_options(qt-mandelbrot PRIVATE -fsanitize=address)
#target_compile_options(qt-mandelbrot PRIVATE -fsanitize=undefined)
#target_compile_options(qt-mandelbrot PRIVATE -fsanitize=thread)
##target_link_options(qt-mandelbrot PRIVATE -fsanitize=address)
#target_link_options(qt-mandelbrot PRIVATE -fsanitize=undefined)
#target_link_options(qt-mandelbrot PRIVATE -fsanitize=thread)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt-mandelbrot
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qt-mandelbrot APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qt-mandelbrot SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qt-mandelbrot
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(qt-mandelbrot PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(qt-mandelbrot PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS qt-mandelbrot
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt-mandelbrot)
endif()
