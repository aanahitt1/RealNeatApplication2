#ifndef RNAMODELGEN_H
#define RNAMODELGEN_H

#include <QString>
#include <QProcess>


class RNAModelGen : public QObject
{
 Q_OBJECT
public:
    RNAModelGen();
    void generate2DModel(QString);

public slots:
    void processError(QProcess::ProcessError);

private:
    QProcess *rnaFold, *rnaPlot;
    QString rnaFoldCmd, rnaPlotCmd;
    QStringList rnaFoldArgs, rnaPlotArgs;


};

#endif // RNAMODELGEN_H


