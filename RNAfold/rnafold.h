#ifndef RNAfold_H
#define RNAfold_H

#include "IAlgorithm.h"
#include <QMap>
#include <QVariant>
#include <QProcess>
#include <QFile>
#include <QRegularExpression>

class RNAfold : public IAlgorithm
{
public:

    Q_INTERFACES(IAlgorithm)

    RNAfold();

    QStringList getInfo() override;
    QString getStructure() override;
    QMap<QString, double> getOptions() override;
    QString initiate(QString filepath, QMap<QString, double> opt) override;
    virtual ~RNAfold() {};

private:
    QStringList getOptionList(QMap<QString, double>);

    QStringList info;
    QMap<QString, double> options;
    QString structure;

    QString lib_path;

};

#endif // RNAfold_H
