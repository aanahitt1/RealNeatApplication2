#include "pluginmanager.h"

PluginManager::PluginManager(QListWidget* algoList)
{
    //Create the Dialog
    main = new QDialog();
    main->resize(400, 300);

    algo = new QListWidget();
    for(int i =0; i<algoList->count(); i++) {
        QListWidgetItem* temp = new QListWidgetItem(*algoList->item(i));
        algo->addItem(temp);
    }

    //build it up
    QGridLayout* back = new QGridLayout(main);
    back->cellRect(3, 1);
    QGridLayout* bottom = new QGridLayout();
    bottom->cellRect(1,3);
    QPushButton* remove = new QPushButton("Remove Selected");
    QPushButton* add = new QPushButton("Add Plug-in");
    QLabel* label = new QLabel("Available plug-ins");
    back->addWidget(algo, 2,1);
    back->addWidget(label, 1, 1);
    back->addLayout(bottom, 3, 1);
    bottom->addWidget(add, 1, 1);
    bottom->addWidget(remove, 1, 3);

    //Add the connections
    connect(remove, &QPushButton::clicked, [=]() {removeAlgo();});
    connect(add, &QPushButton::clicked, [=]() {addAlgo();});

    QString XML_CONFIG_FILE = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0)
            + "/RealNeatApplication2/Config.xml";


}

//This lets you choose a library and add it to the application
void PluginManager::addAlgo() {

    QFileDialog* selection = new QFileDialog();
    selection->show();
    QString* filepath = new QString();
    connect(selection, &QFileDialog::fileSelected, [=] (QString file) {
        *filepath = file;

        //Make sure you selected a .dll
        if(!QLibrary::isLibrary(*filepath)) {
            QMessageBox::information(main, "Library", "This is not a library");
        } else {
            //Find the json and retrieve data
            QJsonDocument doc = findJson(filepath);
            QJsonObject jsonObj = doc.object();
            QJsonObject data = jsonObj["plugin_data"].toObject();
            QString name = data["plugin_name"].toString();
            QString desc = data["plugin_desc"].toString();

            ParseXML* gopher = new ParseXML();
            gopher->addItem(XML_CONFIG_FILE, name, desc, *filepath);
        }


        emit listChanged();
    });

    main->hide();
}

//This removes libraries from your application
void PluginManager::removeAlgo() {

    for(int i =0; i<algo->count(); i++) {
        if(algo->item(i)->checkState() == Qt::Checked) {
            ParseXML* gopher = new ParseXML();
            gopher->removeByName(XML_CONFIG_FILE, algo->item(i)->text());
            algo->takeItem(i);
            delete gopher;
        }
    }

    emit listChanged();

    main->hide();
}

//This subfunction finds a json file next to the dll chosen
QJsonDocument PluginManager::findJson(QString* path){
    QString fileName = path->remove(".dll");
    fileName = fileName.append(".json");
    if(!QFile(fileName).exists()) {
        qInfo() << "No json";
    } else {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QString val = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        return doc;
    }

}

void PluginManager::show() {
    main->show();
}
