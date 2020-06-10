#ifndef RNAMODELGEN_H
#define RNAMODELGEN_H

#include <QString>
#include <QProcess>
#include <QFile>
#include <QTemporaryFile>
#include <QTextStream>
#include <QDebug>
#include <QSaveFile>
#include <QDir>
#include <QDomDocument>


class RNAModelGen : public QObject
{
 Q_OBJECT
public:
    RNAModelGen();
    QFile* generate2DModel(const QString);
    QFile* highlight2DModel(const QString&, const QString&, const QString&);

public slots:
    void processError(QProcess::ProcessError);
    QString get2DModelFile();
    QString get2DModelName();

private:
    QString validateFasta(const QString);
    QHash<int,bool>* getHighlights(const QString&, const QString&);
    bool advanceToStrucLine(QFile&);
    QProcess *rnaFold, *rnaPlot;
    QString rnaFoldCmd, rnaPlotCmd;
    QString modelFile, modelName;
    QStringList rnaFoldArgs, rnaPlotArgs;


};

#endif // RNAMODELGEN_H


