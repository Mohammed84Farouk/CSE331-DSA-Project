#include <QApplication>

#include "xmlparser.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    XMLParser xmlParser;
    xmlParser.show();
    return app.exec();
}
