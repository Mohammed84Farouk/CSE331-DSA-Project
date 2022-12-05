#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QMainWindow>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QPushButton;
class QMenu;
class QMenuBar;
QT_END_NAMESPACE

class XMLParser : public QMainWindow
{
    Q_OBJECT

public:
    XMLParser(QWidget *parent = nullptr);
    ~XMLParser();

private:
    XMLParser *ui;
    void createMenu();
    void openFileClicked();
    void about();
    void saveToFile();
    void loadFromFile();

    QPushButton *loadButton;
    QPushButton *saveButton;

    QMenuBar *menuBar;

    QMenu *fileMenu;
    QMenu *aboutMenu;
    QAction *exitAction;
    QAction *openFileAction;
    QAction *aboutQtAction;
};
#endif // XMLPARSER_H
