#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(xmlparser);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("CSE331-Project");
    QCoreApplication::setApplicationName("XML Parser");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow mainWin;
    if (!parser.positionalArguments().isEmpty())
        mainWin.loadFile(parser.positionalArguments().at(0)); // .first(), [0]
    mainWin.show();
    return app.exec();
}
