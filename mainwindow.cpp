#include <QtWidgets>

#include "mainwindow.h"
#include "compression_decompression/huffman.cpp"
#include "xml_to_json/xmlToJson.cpp"
#include "identation/identation.cpp"

MainWindow::MainWindow() : textEdit(new QPlainTextEdit) {
    createActions();

    createStatusBar();

    central = new QWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *horizontalGroupBox = new QHBoxLayout;

    horizontalGroupBox-> addWidget(textEdit);

    QVBoxLayout *verticalGroupBox = new QVBoxLayout;

    checkButton = new QPushButton(tr("Validate"));
    fixButton = new QPushButton(tr("Fix"));
    compressButton = new QPushButton(tr("Compress"));
    decompressButton = new QPushButton(tr("Decompress"));
    prettifyingButton = new QPushButton(tr("Prettify"));
    minifyingButton = new QPushButton(tr("Minify"));
    toJSONButton = new QPushButton(tr("Json"));
    toGRAPHButton = new QPushButton(tr("Graph"));

    verticalGroupBox-> addWidget(checkButton);
    verticalGroupBox-> addWidget(fixButton);
    verticalGroupBox-> addWidget(compressButton);
    verticalGroupBox-> addWidget(decompressButton);
    verticalGroupBox-> addWidget(prettifyingButton);
    verticalGroupBox-> addWidget(minifyingButton);
    verticalGroupBox-> addWidget(toJSONButton);
    verticalGroupBox-> addWidget(toGRAPHButton);

    connect(compressButton, &QPushButton::released, this, &MainWindow::compressButtonClicked);
    connect(decompressButton, &QPushButton::released, this, &MainWindow::decompressButtonClicked);
    connect(checkButton, &QPushButton::released, this, &MainWindow::validateButtonClicked);
    connect(prettifyingButton, &QPushButton::released, this, &MainWindow::prettifyingButtonClicked);
    connect(minifyingButton, &QPushButton::released, this, &MainWindow::minifyingButtonClicked);
    connect(toJSONButton, &QPushButton::released, this, &MainWindow::jsonButtonClicked);

    horizontalGroupBox->addLayout(verticalGroupBox);

    mainLayout->addLayout(horizontalGroupBox);
    mainLayout->addWidget(textEdit, 2.0);

    outputConsole = new QPlainTextEdit();
    outputConsole -> setReadOnly(true);

    mainLayout -> addWidget(outputConsole);

    setMenuBar(menuBar);

    central->setLayout(mainLayout);

    setCentralWidget(central);

    readSettings();

    connect(textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::documentWasModified);

#ifndef QT_NO_SESSIONMANAGER
    connect(qApp, &QGuiApplication::commitDataRequest, this, &MainWindow::commitData);
#endif

    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::compressButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this);
//    qInfo() << fileName;
    if (!fileName.isEmpty()) {
        try {
            Huffman huffman = Huffman(fileName.toStdString(), fileName.toStdString().append(".huff"));
            huffman.compress();
            statusBar()->showMessage(tr("File compressed to %1").arg(fileName.toStdString().append(".huff").data()));
            QMessageBox::about(this, tr("Success"),
                     tr("File compressed successfully."));
        }
        catch (exception) {
            QMessageBox::about(this, tr("Error"),
                     tr("Something went wrong."));
        }
    }
}

void MainWindow::decompressButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this);
// minimum size is 8 it doesn't make sense if the filename length is less than or equal to 8 --> "D:/.huff"
    if (fileName.length() <= 8 || fileName.toStdString().substr(fileName.length() - 5, fileName.length()) != ".huff") {
        QMessageBox::about(this, tr("Error"),
                 tr("Please choose a .huff file to decompress."));
        return;
    }
    if (!fileName.isEmpty()) {
        try {
            Huffman huffman = Huffman(fileName.toStdString(), fileName.toStdString().substr(0, fileName.length() - 5));
            huffman.decompress();
            statusBar()->showMessage(tr("File decompressed to %1").arg(fileName.toStdString().substr(0, fileName.length() - 5).data()));
            QMessageBox::about(this, tr("Success"),
                     tr("File decompressed successfully."));
        }
        catch (exception) {
            QMessageBox::about(this, tr("Error"),
                     tr("Something went wrong."));
        }
    }
}

void MainWindow::validateButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        try {
            consistent(fileName.toStdString(), fileName.toStdString().substr(0, fileName.length() - 4).append("_out.xml"));
            statusBar()->showMessage(tr("File validated to %1").arg(fileName.toStdString().substr(0, fileName.length() - 4).append("_out.xml").data()));
            QMessageBox::about(this, tr("Success"),
                     tr("File validated successfully."));
        }
        catch (exception) {
            QMessageBox::about(this, tr("Error"),
                     tr("Something went wrong."));
        }
    }
}

void MainWindow::prettifyingButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        try {
            pretify(fileName.toStdString(), fileName.toStdString().substr(0, fileName.length() - 4).append("_out.xml"));
            statusBar()->showMessage(tr("File prettified to %1").arg(fileName.toStdString().substr(0, fileName.length() - 4).append("_out.xml").data()));
            QMessageBox::about(this, tr("Success"),
                     tr("File pretified successfully."));
        }
        catch (exception) {
            QMessageBox::about(this, tr("Error"),
                     tr("Something went wrong."));
        }
    }
}

void MainWindow::minifyingButtonClicked() {
//        QMessageBox::about(this, tr("About XML Parser"), );
//        textEdit->setPlainText(minify(textEdit->toPlainText().toStdString()).data());
}

void MainWindow::jsonButtonClicked() {
    QPlainTextEdit *jsonViewer = new QPlainTextEdit();
    jsonViewer -> setReadOnly(true);
    jsonViewer->setFixedHeight(800);
    jsonViewer->setFixedWidth(1600);
    try {
        Xml *xml = new Xml(textEdit->toPlainText().toStdString());
        string minifiedXml = xml->minifying(textEdit->toPlainText().toStdString());
        jsonViewer->setPlainText(xml->xmlToJSON(minifiedXml).data());
    }
    catch (exception) {
        QMessageBox::about(this, tr("Error"),
                 tr("Something went wrong."));
    }
    QVBoxLayout *verticalBoxLayout = new QVBoxLayout();
    verticalBoxLayout -> addWidget(jsonViewer);
    QWidget *central = new QWidget();
    central -> setLayout(verticalBoxLayout);
    central -> show();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile() {
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile(QString());
    }
}

void MainWindow::open() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
//        qInfo() << fileName;
        if (!fileName.isEmpty())
            loadFile(fileName);
        else {
            QMessageBox::about(this, tr("Error"),
                     tr("This XML file is empty."));
        }
    }
}

bool MainWindow::save() {
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs() {
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().at(0)); // .first(), [0]
}

void MainWindow::about() {
   QMessageBox::about(this, tr("About XML Parser"),
            tr("The <b>XML Parser</b> used to convert xml files to gui based understandable by anyone."));
}

void MainWindow::documentWasModified() {
    setWindowModified(textEdit->document()->isModified());
}

void MainWindow::createActions() {
    menuBar = new QMenuBar;

    QMenu *fileMenu = menuBar->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);


    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);


    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("XML Parser-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);

    exitAct->setStatusTip(tr("Exit the XML Parser"));

    QMenu *editMenu = menuBar->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));

#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
    QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);

    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, &QAction::triggered, textEdit, &QPlainTextEdit::cut);
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, &QAction::triggered, textEdit, &QPlainTextEdit::copy);
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    menuBar->addSeparator();

#endif // !QT_NO_CLIPBOARD

    QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the XML Parser's About box"));
    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

#ifndef QT_NO_CLIPBOARD
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
    connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = screen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

bool MainWindow::maybeSave() {
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("XML Parser"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("XML Parser"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), statusBarMessageLifetime);
}

bool MainWindow::saveFile(const QString &fileName) {
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        if (!file.commit()) {
            errorMessage = tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("XML Parser"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), statusBarMessageLifetime);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName) {
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager) {
    if (manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
        // Non-interactive: save without asking
        if (textEdit->document()->isModified())
            save();
    }
}
#endif

MainWindow::~MainWindow() {
    delete textEdit;
    delete central;
}
