file(GLOB_RECURSE TO_JSON_LIB_SOURCES *.cpp)

add_library(tojsonlib ${TO_JSON_LIB_SOURCES})

target_link_libraries(tojsonlib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
