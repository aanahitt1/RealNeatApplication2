#include "RNAfold.h"
#ifdef Q_OS_WIN
#define MYEXPORT __declspec(dllexport)
#else
#define MYEXPORT
#endif

RNAfold::RNAfold()
{
    //Creates the options.
    options.insert("Circular RNA", 0);
    options.insert("Include G-Quadruplex structures", 0);
    options.insert("No closing GU pairs", 0);
    options.insert("Enter temp (default 37C): ", 37);

    lib_path = "C:\\Program Files (x86)\\ViennaRNA Package\\RNAfold.exe";
}

QMap<QString, double> RNAfold::getOptions() {
    return options;
}

QStringList RNAfold::getInfo() {
    return info;
}

QString RNAfold::getStructure() {
    return structure;
}

QString RNAfold::initiate(QString filepath, QMap<QString, double> opt) {


    QString rnaFoldCmd = "..//";
    QStringList rnaFoldArgs;

    QProcess* proc = new QProcess();

    // generate temp file for RNAFold input
    QFile *seq = new QFile(filepath);

    if(!seq->open(QIODevice::ReadOnly)){
        printf("error opening file.");
    }

    seq->waitForReadyRead(-1);

    if(!seq->isReadable())
        printf("not ready to read file");

    // set process argumemts
    //TODO:Remember how these are going to be returned to this class
    rnaFoldArgs << "--noPS";
    rnaFoldArgs << getOptionList(opt);
    rnaFoldArgs << seq->fileName();

    //start RNAfold;
    proc->setProgram(lib_path);
    proc->setArguments(rnaFoldArgs);
    proc->start(QIODevice::ReadWrite);
    if(!proc->waitForStarted()) {

    }
    proc->waitForReadyRead();
    proc->waitForFinished();
    structure = proc->readAllStandardOutput();

    int last = structure.lastIndexOf("(");
    info << structure.right(structure.length() - last);

    structure = structure.remove(QRegularExpression("[a-zA-Z>\\d\\s]"));
    int length = structure.length();
    structure = structure.left(length-4);


    return structure;
}

//Returns the command line arguments needed for the structure.
QStringList RNAfold::getOptionList(QMap<QString, double> opt) {

    QStringList options;

    if(opt.value("Circular RNA") == 1) {
        options << "-c";
    }
    if(opt.value("Include G-Quadruplex structures")==1) {
        options << "-g";
    }
    if(opt.value("No closing GU pairs")==1) {
        options << "-noGU";
    }
    QString temp = "-T";
    temp.append(QString::number(opt.value("Enter temp (default 37C): ")));

    return options;
}


extern "C" MYEXPORT IAlgorithm* create_RNAfold() {
    return new RNAfold();
}
