file(GLOB_RECURSE GRAPHVIZ_DOT_GENERATOR_LIB_SOURCES *.cpp)

add_library(graphvizdotgeneratorlib ${GRAPHVIZ_DOT_GENERATOR_LIB_SOURCES})

target_link_libraries(graphvizdotgeneratorlib PRIVATE snalib)
