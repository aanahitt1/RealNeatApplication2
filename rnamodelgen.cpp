#include "rnamodelgen.h"

#include <QTemporaryFile>
#include <QTextStream>
#include <QDebug>
#include <QSaveFile>
#include <QDir>

RNAModelGen::RNAModelGen()
{

}

QFile* RNAModelGen::generate2DModel(QString fasta_file){

    //check to make sure file is a valid file
    if(!QFileInfo::exists(fasta_file) && QFileInfo(fasta_file).isFile())
        qCritical() << "RNAModelError: " << "File " << fasta_file << "is not a valid file.";

    rnaPlotCmd = "C:\\Program Files (x86)\\ViennaRNA Package\\RNAplot.exe";
    rnaPlotArgs << "-o" << "svg" << fasta_file;

    // start RNAplot
    rnaPlot = new QProcess(this);
    rnaPlot->setProgram(rnaPlotCmd);
    rnaPlot->setArguments(rnaPlotArgs);
    rnaPlot->setProcessChannelMode(QProcess::ForwardedChannels);
    rnaPlot->start(QIODevice::ReadWrite);

    if(!rnaPlot->waitForFinished()){
        qCritical() << "RNAPlot failed to finish";
        qCritical() << rnaPlot->errorString();
    }


    //open svg file saved (output of RNAPlot)
    QString svg_file = QDir().absolutePath() + "/" + validateFasta(fasta_file).append("_ss.svg");
    QFile rnaplot_output(svg_file);
    qDebug() << "SVG File: " << svg_file;



    //create unique file to write structure (picture) to
    QTemporaryFile* unique_svg = new QTemporaryFile("rnaPictXXXXXX.svg");
      if(!unique_svg->open()){
           qCritical() << "Did not open temp file!";
       }else
           qCritical() << "Temp File name: " << unique_svg->fileName();

     QString rnaPictFile = unique_svg->fileName();


    if(!rnaplot_output.open(QIODevice::ReadWrite)){
        qCritical() << "Can't open RNAPlot output file";
    }

    //copy contents of RNAPlot output to unique file
    unique_svg->write(rnaplot_output.readAll());

    if(!unique_svg->waitForReadyRead(-1))
        qCritical() << "Did not copy file";


    rnaplot_output.close();
    unique_svg->close();

    if(!QFile::remove(svg_file))
        qWarning() << "Could not remove RNAPlot SVG file";


    return unique_svg;

}

// checks the given structure file to ensure it has a fasta header
// and if it does the header is returned
QString RNAModelGen::validateFasta(QString filepath){
    QFile sec_strc(filepath);

    if(!sec_strc.open(QIODevice::ReadWrite)){
        qCritical() << sec_strc.errorString();
        return "";
    }

    QTextStream input(&sec_strc);
    while(!input.atEnd()){

        QString line = input.readLine();

        if(!line.trimmed().isEmpty()){ //look at first line that has text

            // for a fasta file with a header line that looks like this:
            // >myseq ...potentially other text...
            //return "myseq"
            if(line.trimmed().startsWith(">")){
                return line.simplified().section(" ", 0, 0).remove(">");
            }else
              break;
        }

    }

    qCritical() << "Structure file invalid";
    return "";
}

void RNAModelGen::processError(QProcess::ProcessError error){
    qInfo() << error;
}


