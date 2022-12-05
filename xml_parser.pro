QT += widgets

HEADERS += \
    xmlparser.h
SOURCES       += \
                main.cpp \
                xmlparser.cpp

QMAKE_PROJECT_NAME = widgets_xmlparser

# install
target.path = D:\Work\ASU_FOE\CSE331s Data Structures and Algorithms/xml_parser
INSTALLS += target

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
