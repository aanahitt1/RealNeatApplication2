#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QListWidget>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QStandardPaths>
#include <QMessageBox>
#include <QFileDialog>
#include "parsexml.h"

class PluginManager : public QObject
{
    Q_OBJECT

public:
    PluginManager(QListWidget* algoList);

    void show();

private:
    void removeAlgo();
    void addAlgo();
    QJsonDocument findJson(QString*);

    QDialog* main;
    QListWidget* algo;

    QString XML_CONFIG_FILE;



signals:
    void listChanged();

};

#endif // PLUGINMANAGER_H
