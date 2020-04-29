#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //This sets up the main window and sets all bases as they should be.
    ui->setupUi(this);
    file_path = "";
    directory_path = "..\\Documents";
    FASTAcount = 0;
    algoList = NULL;
    tabWindow = new GraphicsWindow();
    createFileBar();
    setWindowTitle("Real Neat Application");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createFileBar()
{
      QAction *quit = new QAction("&Quit", this);
      QAction* Algorithms = new QAction("&Algorithms", this);
      QAction* selectDest = new QAction("&Select FASTA destination");
      QAction* plugin = new QAction("&Plug in Manager");

      //Creating menu
      QMenu *FileBar;
      FileBar = menuBar()->addMenu("&File");
      FileBar->addAction(Algorithms);
      FileBar->addAction(selectDest);
      FileBar->addAction(plugin);
      FileBar->addAction(quit);

      //This makes the quit action
      connect(quit, &QAction::triggered, qApp, QApplication::quit);

      QDialog* listDialog = createAlgoList();
      QFileDialog* selection = new QFileDialog();
      selection->setFileMode(QFileDialog::DirectoryOnly);

      //This shows the algorithm chooser
      connect(Algorithms, &QAction::triggered, listDialog, &QDialog::show);

      //This shows the plugin Manager
      manager = new PluginManager(algoList);
      connect(plugin, &QAction::triggered, [=]() {manager->show();});
      //This refreshes the algoList after a change
      connect(manager, &PluginManager::listChanged, [=](){
          ParseXML* gopher = new ParseXML();
          QString* names = gopher->getInfo("C:/Users/Anahit/Documents/RealNeatApplication/Config.xml", "name");

          populateList(names);
      });

      //This is about choosing a directory.
      connect(selectDest, &QAction::triggered, selection, &QDialog::show);
      connect(selection, &QFileDialog::fileSelected, [=](QString dir){
          directory_path = dir;
      });

}

//This method creates the algorithm chooser dialog box.
QDialog* MainWindow::createAlgoList() {

    //Create the dialog window so it looks good.
    QDialog* list = new QDialog();
    list->resize(500, 300);
    QGridLayout* back = new QGridLayout(list);
    algoList = new QListWidget();
    QPushButton* ok = new QPushButton("&OK");
    QLabel* description = new QLabel();
    description->setWordWrap(true);
    back->cellRect(3, 2);
    back->setColumnMinimumWidth(2, 250);
    back->setRowMinimumHeight(1, 25);
    back->setRowMinimumHeight(2, 150);
    back->setRowMinimumHeight(3, 25);
    back->addWidget(new QLabel("Algorithms"), 1, 1);
    back->addWidget(algoList, 2, 1);
    back->addWidget(ok, 3, 1);
    back->addWidget(description, 2, 2);

    connect(ok, &QPushButton::clicked, list, &QDialog::hide);

    ParseXML* gopher = new ParseXML();
    QString* names = gopher->getInfo("C:/Users/Anahit/Documents/RealNeatApplication/Config.xml", "name");
    QString* desc = gopher->getInfo("C:/Users/Anahit/Documents/RealNeatApplication/Config.xml", "desc");

    populateList(names);

    connect(algoList, &QListWidget::itemPressed, [=](QListWidgetItem* item) {on_listItem_changed(item, description, names, desc);});

    return list;
}

//This function populates the algolist.
void MainWindow::populateList(QString* names) {

    //First we clear the list
    while(algoList->count()>0) {
        algoList->takeItem(0);
    }

    int i =0;
    while(names[i] != "NON") {
        algoList->addItem(names[i]);
        QListWidgetItem* item = algoList->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        i++;
    }
}

void MainWindow::on_listItem_changed(QListWidgetItem* list, QLabel* description, QString* names, QString* desc) {

    int i = 0;
    while(names[i] != "NON") {
        if(names[i] == list->text()) {
            description->setText(desc[i]);
        }
        i++;
    }
}

//This method takes in a String from an input box and saves it as a FASTA file if it is valid, and sets the file path as file_path.
void MainWindow::on_pushButton_clicked()
{
    QString sequence = QInputDialog::getText(this, "Sequence", "Insert RNA sequencce: ");
    bool valid = checkSequence(&sequence);
    if(!valid) {
        QMessageBox::critical(this, "ERROR", "This is not a valid RNA sequence");
    }
    FASTAcount++;
    QString name = "Sequence";
    name.append(QString::number(FASTAcount));
    file_path = RNALIB::stringToFasta(sequence, directory_path, name);
}

//This method takes in a .txt or .FASTA file. It turns a .txt file into a .FASTA file and sets the .FASTA file's path as file_path.
void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Sequence", "..\\documents", "Text files (*.txt);;FASTA files (*.FASTA)");
    if(filename.contains(".txt")) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "ERROR", "File not readable.");
        }

        QString sequence;

        QTextStream s1(&file);

        sequence.append(s1.readAll());
        bool valid = checkSequence(&sequence);
        if(!valid) {
            QMessageBox::critical(this, "ERROR", "This is not a valid RNA sequence");
        }

        //This creates the FASTA file with the name of the original file.
        FASTAcount++;
        QFileInfo* temp = new QFileInfo(file);
        QString name = temp->baseName();
        name.append(QString::number(FASTAcount));
        file_path = RNALIB::stringToFasta(sequence, directory_path, name);
    } else {
        file_path = filename;
    }
}

//This method checks to make sure a sequence has been selected and an algorithm has been selected, and sends the file to the selected
//plug ins to generate a 2D structure.
void MainWindow::on_pushButton_3_clicked()
{
    if(file_path == "") {
        QMessageBox::critical(this, "ERROR", "No sequence chosen.");
        return;
    }

    //Here we create a list of the selected algorithms.
    QList<QSharedPointer<IAlgorithm>>* selectedAlgo = new QList<QSharedPointer<IAlgorithm>>();
    ParseXML* gopher = new ParseXML();
    for (int i = 0; i < algoList->count(); i++) {
        if(algoList->item(i)->checkState() == Qt::Checked) {
            //Checks to see if we can load the algorithms.
            IAlgorithm* algoObject = loader(gopher->getPath(":/res/Config.xml", i), algoList->item(i)->text());
            if(algoObject == NULL) {
                QMessageBox::critical(this, "ERROR", "Algorithm not available");
                break;
            }

            //Here we get the option list so we can offer it to the user
            QMap<QString, double> options = algoObject->getOptions();

            OptionChooser* opt = new OptionChooser(options);
            opt->show();

            //The connect needs to bring everything together - the graphics window only shows when the options are chosen and
            //OptionChooser is closed.
            connect(opt, &OptionChooser::boxClosed, [=](QMap<QString, double> options) {
                showTab(options, algoObject);
            });

            QSharedPointer<IAlgorithm> ptr(algoObject);
            selectedAlgo->push_back(ptr);
        }
    }
    if(selectedAlgo->isEmpty()) {
        QMessageBox::critical(this, "ERROR", "No algorithms chosen.");
        return;
    }

}

//This algorithm takes the options, runs it to the IAlgorithm, and puts the structure into the FASTA, then opens the tab
void MainWindow::showTab(QMap<QString, double> options, IAlgorithm* algo) {
    QString structure = algo->initiate(file_path, options);

    //Add structure to FASTA
    QFile fasta(file_path);
    if(fasta.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&fasta);

        stream << "\n" << structure;
        stream.flush();

        fasta.close();
    }

    tabWindow->show(file_path);

}

//This method loads the dll from the given filepath and returns it in type IAlgorithm
IAlgorithm* MainWindow::loader(QString* libpath, QString name) {

    if(!QLibrary::isLibrary(*libpath)) {
        QMessageBox::information(this, "Library", "This is not a library");
        return NULL;
    }

    QLibrary customDLL(*libpath);
    if(customDLL.load()) {
        typedef IAlgorithm* (*customAlgo)();
        name = "create_" + name;
        customAlgo temp = (customAlgo)customDLL.resolve(name.toStdString().c_str());
        if(temp) {
            IAlgorithm* success = temp();
            return success;
        }
    }
    return NULL;
}

//This method checks to make sure the sequence is a valid RNA sequence.
bool MainWindow::checkSequence(QString* seq) {
    QString sequence = seq->toUpper();
    bool valid = true;
    int i = 0;
    while(sequence[i]!=NULL) {
        if(sequence[i]!='A' && sequence[i]!='C' && sequence[i]!='U' && sequence[i]!='G') {
            valid = false;
        }
        i++;
    }

    return valid;
}
