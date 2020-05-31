#ifndef RNAPKPLEX_H
#define RNAPKPLEX_H

#include "IAlgorithm.h"
#include <QMap>
#include <QVariant>
#include <QProcess>
#include <QFile>
#include <QRegularExpression>

class RNAPKplex : public IAlgorithm
{
public:

    Q_INTERFACES(IAlgorithm)

    RNAPKplex();

    QStringList getInfo() override;
    QString getStructure() override;
    QMap<QString, double> getOptions() override;
    QString initiate(QString filepath, QMap<QString, double> opt) override;
    virtual ~RNAPKplex() {};

private:
    QStringList getOptionList(QMap<QString, double>);

    QStringList info;
    QMap<QString, double> options;
    QString structure;

    QString lib_path;

};

#endif // RNAPKPLEX_H
