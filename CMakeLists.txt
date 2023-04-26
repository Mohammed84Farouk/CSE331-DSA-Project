cmake_minimum_required(VERSION 3.5)

project(xmlparser VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON) # for .qrc resource file to be added to excutable using add_excutable() cmake command

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Edit the below paths to match your Qt installation correct path and version.
# Using these paths, you can compile and build this Qt project using CMake from any IDE you like.
set(QT_DIR "C:/Qt/6.2.4/mingw_64/lib/cmake/Qt6")
set(Qt6_DIR "C:/Qt/6.2.4/mingw_64/lib/cmake/Qt6")
set(Qt6WidgetsTools_DIR "C:/Qt/6.2.4/mingw_64/lib/cmake/Qt6WidgetsTools")
set(Qt6CoreTools_DIR "C:/Qt/6.2.4/mingw_64/lib/cmake/Qt6CoreTools")
set(Qt6GuiTools_DIR "C:/Qt/6.2.4/mingw_64/lib/cmake/Qt6GuiTools")

# Make sure to edit Qt version.
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        xmlparser.qrc
)

add_subdirectory(${CMAKE_SOURCE_DIR}/compression_decompression)
add_subdirectory(${CMAKE_SOURCE_DIR}/identation)
add_subdirectory(${CMAKE_SOURCE_DIR}/xml_to_json)
add_subdirectory(${CMAKE_SOURCE_DIR}/SNA_graph)
add_subdirectory(${CMAKE_SOURCE_DIR}/graphviz_dot_generator)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(xmlparser
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET xmlparser APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(xmlparser SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(xmlparser
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(xmlparser PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(xmlparser PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(xmlparser)
endif()
