#ifndef RNAFOLD_H
#define RNAFOLD_H

#include "IAlgorithm.h"
#include <QDir>
#include <QProcess>
#include <QMap>
#include <QJsonDocument>
#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QVariant>
#include <QTemporaryFile>
#include <QTextStream>
#include <QJsonValue>

class RNAFold : public QObject, public IAlgorithm
{
public:

    Q_OBJECT

    Q_PLUGIN_METADATA(IID "IAlgorithm" FILE "rnafold.json")

    Q_INTERFACES(IAlgorithm)

    //RNAFold();
    //RNAFold(QString);
    ~RNAFold() override;
    QStringList getInfo() override;
    QString getStructure() override;
    QMap<QString,double> getOptions() override;
    QString initiate(QString filepath, QMap<QString, double> opt) override;

//private:
//    void findJson();
//    void findDir(QString);
//    void extractJsonData();
//    QStringList uniteMaps(QMap<QString,double>);
    void runProc(QString rnaSequence);
    void buildOptions(QJsonObject);

    QDir* directory;
    QString* path;
    QJsonDocument* doc;
    QProcess* proc;

    QMap<QString,QString> commandMap;

    QStringList info;
    QMap<QString, double> map;
    QString algName;
    QString algDesc;
    QMap<QString, QVariant>* pluginMap;
    QVariantList* algOptions;
    QString structure;

};

#endif // RNAFold_H
