#ifndef RNAMODELGEN_H
#define RNAMODELGEN_H

#include <QString>
#include <QProcess>
#include <QFile>


class RNAModelGen : public QObject
{
 Q_OBJECT
public:
    RNAModelGen();
    QFile* generate2DModel(QString);

public slots:
    void processError(QProcess::ProcessError);

private:
    QString validateFasta(QString);
    QProcess *rnaFold, *rnaPlot;
    QString rnaFoldCmd, rnaPlotCmd;
    QStringList rnaFoldArgs, rnaPlotArgs;


};

#endif // RNAMODELGEN_H


