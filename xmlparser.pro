QT += widgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                compression_decompression/huffman.cpp \
                xml_to_json/xmlToJson.cpp \
                identation/identation.cpp

RESOURCES     = xmlparser.qrc

# install
target.path = D:\Work\ASU_FOE\CSE331s Data Structures and Algorithms\CSE331-DSA-Project
INSTALLS += target

