#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
class QPushButton;
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

private:
    const int statusBarMessageLifetime = 2000;

    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void compressButtonClicked();
    void decompressButtonClicked();
    void minifyingButtonClicked();
    void jsonButtonClicked();
    QWidget *central;
    QString strippedName(const QString &fullFileName);
    QPlainTextEdit *textEdit;
    QString curFile;
    QMenuBar *menuBar;
    QPushButton *checkButton;
    QPushButton *fixButton;
    QPushButton *compressButton;
    QPushButton *decompressButton;
    QPushButton *prettifyingButton;
    QPushButton *minifyingButton;
    QPushButton *toJSONButton;
    QPushButton *toGRAPHButton;
    QPlainTextEdit *outputConsole;
};


#endif // MAINWINDOW_H
