#include "rnamodelgen.h"

#include <QTemporaryFile>
#include <QTextStream>
#include <QDebug>
#include <QSaveFile>
#include <QDir>

RNAModelGen::RNAModelGen()
{
    // configure the processes (RNAfold and RNAplot)
    rnaPlotCmd = "C:\\Program Files (x86)\\ViennaRNA Package\\RNAplot.exe";

    QDir dir;
    qInfo() << dir.absolutePath();


}

void RNAModelGen::generate2DModel(QString filepath){

    //initialize the QProcess
    rnaPlot = new QProcess(this);

    // set process argumemts
    rnaPlotArgs << "-o" << "svg";

    //Convert file to QByteArray
    QFile* temp = new QFile(filepath);
    QFileInfo info(filepath);
    temp->open(QIODevice::ReadWrite);
    if(!temp->isOpen()) {
        qInfo() << "File not open.";
        qInfo() << temp->error();
    }
    QByteArray array = temp->readAll();

    // start RNAplot
    rnaPlot->setProgram(rnaPlotCmd);
    rnaPlot->setArguments(rnaPlotArgs);
    rnaPlot->start(QIODevice::ReadWrite);
    rnaPlot->write(array);

    rnaPlot->waitForFinished();
    rnaPlot->closeWriteChannel();
}

void RNAModelGen::processError(QProcess::ProcessError error){
    qInfo() << error;
}


