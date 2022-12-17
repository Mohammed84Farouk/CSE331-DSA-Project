QT += widgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h
SOURCES       = main.cpp \
                mainwindow.cpp

RESOURCES     = xmlparser.qrc

# install
target.path = D:\Work\ASU_FOE\CSE331s Data Structures and Algorithms
INSTALLS += target

