#include <QtWidgets>

#include "mainwindow.h"
#include "compression_decompression/huffman.cpp"
#include "identation/identation.cpp"
#include "SNA_graph/main.cpp"

SNA *sna = nullptr;
vector<user> users;
vector<vector<int>> adj;
pair<unordered_map<string,int>, vector<string>> p;
unordered_map<string, int> ID;
vector<string> antiID;

MainWindow::MainWindow() : textEdit(new QPlainTextEdit) {
    createActions();

    createStatusBar();

    central = new QWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *mainButtons = new QHBoxLayout;

    checkButton = new QPushButton(tr("Validate"));
    compressButton = new QPushButton(tr("Compress"));
    decompressButton = new QPushButton(tr("Decompress"));
    prettifyingButton = new QPushButton(tr("Prettify"));
    minifyingButton = new QPushButton(tr("Minify"));
    toJSONButton = new QPushButton(tr("Json"));
    toGRAPHButton = new QPushButton(tr("Graph"));

    mainButtons-> addWidget(checkButton);
    mainButtons-> addWidget(compressButton);
    mainButtons-> addWidget(decompressButton);
    mainButtons-> addWidget(prettifyingButton);
    mainButtons-> addWidget(minifyingButton);
    mainButtons-> addWidget(toJSONButton);
    mainButtons-> addWidget(toGRAPHButton);

    connect(compressButton, &QPushButton::released, this, &MainWindow::compressButtonClicked);
    connect(decompressButton, &QPushButton::released, this, &MainWindow::decompressButtonClicked);
    connect(checkButton, &QPushButton::released, this, &MainWindow::validateButtonClicked);
    connect(prettifyingButton, &QPushButton::released, this, &MainWindow::prettifyingButtonClicked);
    connect(minifyingButton, &QPushButton::released, this, &MainWindow::minifyingButtonClicked);
    connect(toJSONButton, &QPushButton::released, this, &MainWindow::jsonButtonClicked);
    connect(toGRAPHButton, &QPushButton::released, this, &MainWindow::graphButtonClicked);

    QHBoxLayout *graphButtons = new QHBoxLayout;

    mostInfluencerButton = new QPushButton(tr("Most Influencer"));
    mostActiveButton = new QPushButton(tr("Most Active"));
    mutualFollowersButton = new QPushButton(tr("Mutual Followers"));
    suggestFollowersButton = new QPushButton(tr("Suggest Followers"));
    postsSearchButton = new QPushButton(tr("Search post"));
    betweennessButton = new QPushButton(tr("Betweenness (Bonus)"));

    graphButtons-> addWidget(mostInfluencerButton);
    graphButtons-> addWidget(mostActiveButton);
    graphButtons-> addWidget(mutualFollowersButton);
    graphButtons-> addWidget(suggestFollowersButton);
    graphButtons-> addWidget(postsSearchButton);
    graphButtons-> addWidget(betweennessButton);

    connect(mostInfluencerButton, &QPushButton::released, this, &MainWindow::mostInfluencerButtonClicked);
    connect(mostActiveButton, &QPushButton::released, this, &MainWindow::mostActiveButtonClicked);
    connect(mutualFollowersButton, &QPushButton::released, this, &MainWindow::mutualFollowersButtonClicked);
    connect(suggestFollowersButton, &QPushButton::released, this, &MainWindow::suggestFollowersButtonClicked);
    connect(postsSearchButton, &QPushButton::released, this, &MainWindow::postsSearchButtonClicked);
    connect(betweennessButton, &QPushButton::released, this, &MainWindow::betweennessButtonClicked);

    QHBoxLayout *graphSettings = new QHBoxLayout;

    topicSearchLabel = new QLabel();
    topicSearchLabel -> setText("Topic Name for Search");

    topicSearchLineEdit = new QLineEdit();

    bodySearchLabel = new QLabel();
    bodySearchLabel -> setText("Body word for Search");

    bodySearchLineEdit = new QLineEdit();

    firstUserLabel = new QLabel();
    firstUserLabel -> setText("First user name");

    firstUserLineEdit = new QLineEdit();

    secondUserLabel = new QLabel();
    secondUserLabel -> setText("Second user name");

    secondUserLineEdit = new QLineEdit();

    graphSettings -> addWidget(topicSearchLabel);
    graphSettings -> addWidget(topicSearchLineEdit);
    graphSettings -> addWidget(bodySearchLabel);
    graphSettings -> addWidget(bodySearchLineEdit);
    graphSettings -> addWidget(firstUserLabel);
    graphSettings -> addWidget(firstUserLineEdit);
    graphSettings -> addWidget(secondUserLabel);
    graphSettings -> addWidget(secondUserLineEdit);

    mainLayout -> addLayout(mainButtons);

    mainLayout -> addLayout(graphButtons);

    mainLayout -> addLayout(graphSettings);

    mainLayout -> addWidget(textEdit);

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
            outputConsole -> clear();
            Huffman huffman = Huffman(fileName.toStdString(), fileName.toStdString().append(".huff"));
            outputConsole -> appendPlainText("compressing...");
            huffman.compress();
            statusBar()->showMessage(tr("File compressed to %1").arg(fileName.toStdString().append(".huff").data()));
            outputConsole -> appendPlainText("File compressed successfully.");
        }
        catch (exception &e) {
            outputConsole -> appendPlainText("ERROR: compressing failed for some reason.");
            outputConsole -> appendPlainText(e.what());
        }
    }
}

void MainWindow::decompressButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this);
// minimum size is 8 it doesn't make sense if the filename length is less than or equal to 8 --> "D:/.huff"
    if (fileName.length() <= 8 || fileName.toStdString().substr(fileName.length() - 5, fileName.length()) != ".huff") {
        return outputConsole -> appendPlainText("ERROR: Please choose a .huff file to decompress.");
    }
    if (!fileName.isEmpty()) {
        try {
            outputConsole -> clear();
            Huffman huffman = Huffman(fileName.toStdString(), fileName.toStdString().substr(0, fileName.length() - 5));
            outputConsole -> appendPlainText("decompressing...");
            huffman.decompress();
            statusBar()->showMessage(tr("File decompressed to %1").arg(fileName.toStdString().substr(0, fileName.length() - 5).data()));
            outputConsole -> appendPlainText("file decompressed successfully.");
            outputConsole -> appendPlainText(tr("you can find it in %1.").arg(fileName.toStdString().substr(0, fileName.length() - 5).data()));
        }
        catch (exception &e) {
            outputConsole -> appendPlainText("ERROR: decompressing failed for some reason.");
            outputConsole -> appendPlainText(e.what());
        }
    }
}

void MainWindow::validateButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        try {
            outputConsole -> clear();
            consistent(fileName.toStdString(), fileName.toStdString().substr(0, fileName.length() - 4).append("_out.xml"));
            statusBar()->showMessage(tr("File validated to %1").arg(fileName.toStdString().substr(0, fileName.length() - 4).append("_out.xml").data()));
            outputConsole -> appendPlainText("file validated successfully.");
            outputConsole -> appendPlainText(tr("you can find it in %1.").arg(fileName.toStdString().substr(0, fileName.length() - 4).append("_out.xml").data()));
        }
        catch (exception &e) {
            outputConsole -> appendPlainText("ERROR: validating failed for some reason.");
            outputConsole -> appendPlainText(e.what());
        }
    }
}

void MainWindow::prettifyingButtonClicked() {
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        try {
            outputConsole -> clear();
            pretify(fileName.toStdString(), fileName.toStdString().substr(0, fileName.length() - 4).append("_out.xml"));
            statusBar()->showMessage(tr("File prettified to %1").arg(fileName.toStdString().substr(0, fileName.length() - 4).append("_out.xml").data()));
            outputConsole -> appendPlainText("file pretified successfully.");
            outputConsole -> appendPlainText(tr("you can find it in %1.").arg(fileName.toStdString().substr(0, fileName.length() - 4).append("_out.xml").data()));
        }
        catch (exception &e) {
            outputConsole -> appendPlainText("ERROR: pretifying failed for some reason.");
            outputConsole -> appendPlainText(e.what());
        }
    }
}

void MainWindow::minifyingButtonClicked() {
    if (textEdit -> toPlainText().length() == 0) {
        outputConsole -> appendPlainText("ERROR: no xml text found to start minifying.");
        return;
    }
    try {
        outputConsole -> clear();
        outputConsole -> appendPlainText("xml object is created automatically when you open any file so it's ready for minifing.");
        outputConsole -> appendPlainText("minifying...");
        minifiedXml = xml->minifying(textEdit->toPlainText().toStdString());
        isMinifiled = true;
        outputConsole -> appendPlainText("file minified successfully.");
        outputConsole -> appendPlainText("================= Start Minified Xml Text =================\n");
        outputConsole -> appendPlainText(minifiedXml.data());
        outputConsole -> appendPlainText("\n");
        outputConsole -> appendPlainText("================= End Minified Xml Text =================\n");
    }
    catch (exception &e) {
        outputConsole -> appendPlainText("ERROR: minifying failed for some reason.");
        outputConsole -> currentCharFormat().colorProperty(0xFFFF0000);
        outputConsole -> appendPlainText(e.what());
    }
}

void MainWindow::jsonButtonClicked() {
    QPlainTextEdit *jsonViewer = new QPlainTextEdit();
    jsonViewer -> setReadOnly(true);
    jsonViewer->setFixedHeight(800);
    jsonViewer->setFixedWidth(1600);
    if (xml == nullptr || !isMinifiled) {
        outputConsole -> appendPlainText("ERROR: no minified text found, please run Minify button first.");
        return;
    }
    try {
        string minifiedXml = textEdit->toPlainText().toStdString();
        jsonViewer->setPlainText(xml->xmlToJSON(minifiedXml).data());
        QVBoxLayout *verticalBoxLayout = new QVBoxLayout();
        verticalBoxLayout -> addWidget(jsonViewer);
        QWidget *central = new QWidget();
        central -> setLayout(verticalBoxLayout);
        central -> show();
    }
    catch (exception &e) {
        outputConsole -> appendPlainText("ERROR: converting to json failed for some reason.");
        outputConsole -> appendPlainText(e.what());
    }
}

//string formatPath(string path) {
//    string formattedPath;
//    for (char x : path) {
//        if (x == '/') formattedPath.push_back('\\');
//        else formattedPath.push_back(x);
//    }
//    return formattedPath;
//}

void MainWindow::graphButtonClicked() {
    if (xml == nullptr || !isMinifiled) {
        outputConsole -> appendPlainText("ERROR: no minified text found, please run Minify button first.");
        return;
    }
    QString fileName = QFileDialog::getExistingDirectory(this);
    qInfo() << fileName;
    if (!fileName.isEmpty()) {
        outputConsole -> clear();
//        string formattedPath = formatPath(fileName.toStdString()); // for formatting paths correctly
//        qInfo() << formattedPath.data();
        QPlainTextEdit *graphViewer = new QPlainTextEdit();
        graphViewer -> setReadOnly(true);
        graphViewer->setFixedHeight(800);
        graphViewer->setFixedWidth(1600);

        try {
//            qInfo() << minifiedXml.data();
//            qInfo() << minifiedXml.length();
            if (sna != nullptr) {
                outputConsole -> appendPlainText("optimizing memory...");
                delete sna;
                outputConsole -> currentCharFormat().colorProperty(0xFF00FF00);
                outputConsole -> appendPlainText("memory optimized successfully.");
                outputConsole -> currentCharFormat().colorProperty(0xFFFFFFFF);
            }
            else {
                outputConsole -> appendPlainText("creating SNA object...");
                sna = new SNA();
                outputConsole -> currentCharFormat().colorProperty(0xFF00FF00);
                outputConsole -> appendPlainText("SNA object created successfully.");
                outputConsole -> currentCharFormat().colorProperty(0xFFFFFFFF);
            }

            users = sna -> extractInputFromMinifiedXML(minifiedXml, *sna);
            qInfo() << "users: " << users.size();
            adj = sna -> getGraph(users);
            qInfo() << "adj: " << adj.size();
            p = sna -> GetAntiID(users);
            ID = p.first;

            antiID = p.second;

            outputConsole -> appendPlainText("creating input.txt file..., this is the file where we will read input from.");
            sna -> createFile(users, fileName.toStdString().append("input.txt"));
            outputConsole -> appendPlainText("input.txt file create successfully.");
            outputConsole -> appendPlainText(tr("you can find it here %1").arg(fileName));
//            qInfo() << fileName.toStdString().data();
            outputConsole -> appendPlainText(tr("reading input.txt in %1 to create the graph...").arg(fileName));
            sna -> takeInput(*sna, fileName.toStdString());
            isGraphCreated = true;
            outputConsole -> appendPlainText("graph created successfully.");
        }
        catch (exception &e) {
            outputConsole -> appendPlainText("ERROR: buidling graph failed for some reason.");
            outputConsole -> appendPlainText(e.what());
        }
    }
}

void MainWindow::mostInfluencerButtonClicked() {
    if (xml == nullptr || !isMinifiled) {
        outputConsole -> appendPlainText("ERROR: no minified text found, please run Minify button first.");
        return;
    }
    if (!isGraphCreated) {
        outputConsole -> appendPlainText("ERROR: no graph found, please run Graph button first.");
        return;
    }
    try {
        outputConsole -> appendPlainText("=========================== Most Inflencer User Output ========================");
        outputConsole -> appendPlainText(getMostInfluencer(adj, antiID).data());
        outputConsole -> appendPlainText("=====================================================================");

    }
    catch (exception &e) {
        outputConsole -> appendPlainText("ERROR: getting most influencer user failed for some reason.");
outputConsole -> appendPlainText(e.what());
    }
}

void MainWindow::mostActiveButtonClicked() {
    if (xml == nullptr || !isMinifiled) {
        outputConsole -> appendPlainText("ERROR: no minified text found, please run Minify button first.");
        return;
    }
    if (!isGraphCreated) {
        outputConsole -> appendPlainText("ERROR: no graph found, please run Graph button first.");
        return;
    }
    try {
        outputConsole -> appendPlainText("=========================== Most Active User Output ========================");
        outputConsole -> appendPlainText(getMostActive(adj, antiID).data());
        outputConsole -> appendPlainText("=====================================================================");
    }
    catch (exception &e) {
        outputConsole -> appendPlainText("ERROR: getting most active user failed for some reason.");
outputConsole -> appendPlainText(e.what());
    }
}

void MainWindow::mutualFollowersButtonClicked() {
    if (xml == nullptr || !isMinifiled) {
        outputConsole -> appendPlainText("ERROR: no minified text found, please run Minify button first.");
        return;
    }
    if (!isGraphCreated) {
        outputConsole -> appendPlainText("ERROR: no graph found, please run Graph button first.");
        return;
    }
    string user1;
    string user2;
    try {
        if (firstUserLineEdit -> text().length() != 0 && secondUserLineEdit -> text().length() != 0) {
            outputConsole -> appendPlainText("=========================== Mutual Followers Output ========================");
            user1 = firstUserLineEdit -> text().toStdString();
            user2 = secondUserLineEdit -> text().toStdString();
            vector<string> mutual = getMutualFollowers(user1, user2, adj, antiID, ID);
            for (auto &x : mutual) {
                outputConsole -> appendPlainText(x.data());
            }
            outputConsole -> appendPlainText("=====================================================================");
        }
        else {
            outputConsole -> appendPlainText("ERROR: please fill in \"First User Name\" and \"Second User Name\" above in order to find mutual followers between them.");
        }
    }
    catch (exception &e) {
        outputConsole -> appendPlainText(tr("ERROR: suggesting followers for user %1 and user %2 failed for some reason.").arg(user1.data(), user2.data()));
outputConsole -> appendPlainText(e.what());
    }
}

void MainWindow::suggestFollowersButtonClicked() {
    if (xml == nullptr || !isMinifiled) {
        outputConsole -> appendPlainText("ERROR: no minified text found, please run Minify button first.");
        return;
    }
    if (!isGraphCreated) {
        outputConsole -> appendPlainText("ERROR: no graph found, please run Graph button first.");
        return;
    }
    string user1;
    try {
        if (firstUserLineEdit -> text().length() != 0) {
            outputConsole -> appendPlainText("=========================== Suggest Followers Output ========================");
            user1 = firstUserLineEdit -> text().toStdString();
            vector <string> suggest = suggestFollowers(user1 , adj, antiID, ID);
            for (auto &x : suggest) {
                outputConsole -> appendPlainText(x.data());
            }
            outputConsole -> appendPlainText("=====================================================================");
        }
        else {
            outputConsole -> appendPlainText("ERROR: please fill in \"First User Name\" above in order to find suggest followers.");
        }

    }
    catch (exception &e) {
        outputConsole -> appendPlainText(tr("ERROR: suggesting followers for user %1 failed for some reason.").arg(user1.data()));
        outputConsole -> appendPlainText(e.what());
    }
}

void MainWindow::postsSearchButtonClicked() {
    if (xml == nullptr || !isMinifiled) {
        outputConsole -> appendPlainText("ERROR: no minified text found, please run Minify button first.");
        return;
    }
    if (!isGraphCreated) {
        outputConsole -> appendPlainText("ERROR: no graph found, please run Graph button first.");
        return;
    }
    string topic;
    string word;
    try {

        if ((topicSearchLineEdit -> text().length()) != 0)
        {
            outputConsole -> appendPlainText("=========================== Search Topic Result Output ========================");
            topic = topicSearchLineEdit -> text().toStdString();
            // false means search in topics
            vector<pair<int, string>> x=postSearch(topic, false, users);
            for(auto&xx:x) {
                outputConsole -> appendPlainText(tr("%1 %2").arg(xx.first).arg(xx.second.data()));
            }
            outputConsole -> appendPlainText("=====================================================================");
        }
        else {
            outputConsole -> appendPlainText("ERROR: please fill in \"Topic Name for Search\" above in order to search in topics.");
        }

        if ((bodySearchLineEdit -> text().length()) != 0)
        {
            outputConsole -> appendPlainText("=========================== Search Body Result Output ========================");
            word = bodySearchLineEdit -> text().toStdString();
            // true means search in bodies
            vector<pair<int, string>> y=postSearch(word, true, users);
            for(auto&yy:y) {
                outputConsole -> appendPlainText(tr("%1 %2").arg(yy.first).arg(yy.second.data()));
            }
            outputConsole -> appendPlainText("=====================================================================");
        }
        else {
            outputConsole -> appendPlainText("ERROR: please fill in \"Body Word for Search\" above in order to search in topics.");
        }

    }
    catch (exception &e) {
        outputConsole -> appendPlainText(tr("ERROR: searching for topic %1 and word %2 failed for some reason.").arg(topic.data(), word.data()));
outputConsole -> appendPlainText(e.what());
    }
}

void MainWindow::betweennessButtonClicked() {
    if (xml == nullptr || !isMinifiled) {
        outputConsole -> appendPlainText("ERROR: no minified text found, please run Minify button first.");
        return;
    }
    if (!isGraphCreated) {
        outputConsole -> appendPlainText("ERROR: no graph found, please run Graph button first.");
        return;
    }
    try {
        outputConsole -> appendPlainText("=========================== Highest Betweenness Output ========================");
        string betweenness = highestBetweenness(adj, antiID);
        outputConsole -> appendPlainText(betweenness.data());
        outputConsole -> appendPlainText("=====================================================================");
    }
    catch (exception &e) {
        outputConsole -> appendPlainText("ERROR: calculating betweenness failed for some reason.");
outputConsole -> appendPlainText(e.what());
    }
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
            outputConsole -> appendPlainText("xml is empty.");
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

    if (xml != nullptr) {
        outputConsole -> appendPlainText("optimizing memory...");
        delete xml;
        outputConsole -> currentCharFormat().colorProperty(0xFF00FF00);
        outputConsole -> appendPlainText("memory optimized successfully.");
        outputConsole -> currentCharFormat().colorProperty(0xFFFFFFFF);
    }
    try {
        outputConsole -> appendPlainText("creating new xml object...");
        xml = new Xml(textEdit->toPlainText().toStdString());
        outputConsole -> appendPlainText("xml object created successfully.");
        isMinifiled = false;
        isGraphCreated = false;
        outputConsole -> appendPlainText("reset minify done.");
    }
    catch (exception &e) {
        outputConsole -> appendPlainText("something went wrong while creating xml object.");
        outputConsole -> appendPlainText(e.what());
    }
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
