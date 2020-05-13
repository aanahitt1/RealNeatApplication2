#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QFile>
#include <QMainWindow>
#include <QPluginLoader>
#include <QTreeWidget>
#include <QPlainTextEdit>
#include "IAlgorithm.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString stripFileExt(const QString &fullFileName);

private:
    Ui::MainWindow *ui;

    void loadPlugins();
    void findPlugins(const QString &path, const QStringList &pluginNames);
//    void populateTreeWidget(QObject *plugin, const QString &text);

    QDir plugDir;
    QStringList pluginFileNames;
    QPlainTextEdit* textEdit;
    QTreeWidget *treeWidget = nullptr;

};
#endif // MAINWINDOW_H
