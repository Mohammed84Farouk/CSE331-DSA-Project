#include <QtWidgets>

#include "xmlparser.h"

XMLParser::XMLParser(QWidget *parent)
    : QMainWindow(parent)
{
    createMenu();


    setWindowTitle(tr("XML Parser"));
}

void XMLParser::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    aboutMenu = new QMenu(tr("&About"), this);
    openFileAction = fileMenu->addAction(tr("O&pen"));
    exitAction = fileMenu->addAction(tr("E&xit"));
    aboutQtAction = aboutMenu->addAction(tr("A&bout Qt"));

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(aboutMenu);

    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(openFileAction, &QAction::triggered, this, &XMLParser::openFileClicked);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

    setMenuBar(menuBar);
}

void XMLParser::openFileClicked()
{
    QString fileContent;

    QString filename = QFileDialog::getOpenFileName(this, "Choose XML File");


   if(filename.isEmpty())
       return;

   QFile file(filename);

   if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
       return;

   QTextStream in(&file);

   fileContent = in.readAll();

   file.close();

   QMessageBox::about(this, tr("Done"), tr("File loaded successfully."));

//   ui->textEdit->clear();
//   ui->textEdit->setPlainText(fileContent);
}

void XMLParser::about()
{
    QMessageBox::about(this, tr("About"), tr("This example demonstrates the "
        "different features of the QCompleter class."));
}

XMLParser::~XMLParser()
{
    delete ui;
}
