#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
}



void MainWindow::loadPlugins(){

    //Starting with static
    //auto infers type by value
    const auto instances = QPluginLoader::staticInstances();

    for(QObject* plug : instances){
    }
        //plug->getoptions()
        //send plugin list/populate list
    MainWindow::plugDir = QDir(QCoreApplication::applicationDirPath());

    //simplifies moving from dev to release
    //Note: this is only valid for WIN OS
    if(plugDir.dirName().toLower() == "debug" || plugDir.dirName().toLower() == "release")
        plugDir.cdUp();

    plugDir.cd("plugins");


    //Now Dynamic

    const auto dynInstances = plugDir.entryList(QDir::Files);
    for(const QString &fileName : dynInstances) {
        QPluginLoader loader(plugDir.absoluteFilePath(fileName));
        //This will be null if the plugin is invalid. This could be the case if it was compiled by an incompatible compiler.
        QObject* plugin = loader.instance();
        if(plugin){
            //send plugin list/populate list
            pluginFileNames += fileName;
        }
    }
    //IAlgorithm->setEnabled
}

void MainWindow::findPlugins(const QString &path, const QStringList &fileNames)
{
    /*label->setText(tr("Plug & Paint found the following plugins\n"
                      "(looked in %1):")
                   .arg(QDir::toNativeSeparators(path)));
*/
    const QDir dir(path);

    const auto staticInstances = QPluginLoader::staticInstances();
    for (QObject *plugin : staticInstances)
    //    populateTreeWidget(plugin, tr("%1 (Static Plugin)")
    //                               .arg(plugin->metaObject()->className()));

    for (const QString &fileName : fileNames) {
        QPluginLoader loader(dir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
    //   if (plugin)
    //        populateTreeWidget(plugin, fileName);

    }
}
/*
void MainWindow::populateTreeWidget(QObject *plugin, const QString &text)
{
    auto pluginItem = new QTreeWidgetItem(treeWidget);
    pluginItem->setText(0, text);
    pluginItem->setExpanded(true);

    QFont boldFont = pluginItem->font(0);
    boldFont.setBold(true);
    pluginItem->setFont(0, boldFont);

    if (plugin) {
        auto iAlg = qobject_cast<IAlgorithm *>(plugin);
        if (iAlg){
            //do something
        }
    }
}
*/
//This method removes the extension of the file put into it. Very useful.
QString MainWindow::stripFileExt(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}

MainWindow::~MainWindow(){
    delete ui;
}
