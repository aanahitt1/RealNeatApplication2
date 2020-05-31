#include "rnapkplex.h"
#ifdef Q_OS_WIN
#define MYEXPORT __declspec(dllexport)
#else
#define MYEXPORT
#endif

RNAPKplex::RNAPKplex()
{
    //Creates the options.
    options.insert("Enter energy cutoff (default -8.10):", -8.1);
    options.insert("No lonely pairs", 0);
    options.insert("No GU pairs", 0);
    options.insert("No closing GU pairs:", 0);
    options.insert("Enter temp (default 37C): ", 37);

    lib_path = "C:\\Program Files (x86)\\ViennaRNA Package\\RNAPKplex.exe";
}

QMap<QString, double> RNAPKplex::getOptions() {
    return options;
}

QStringList RNAPKplex::getInfo() {
    return info;
}

QString RNAPKplex::getStructure() {
    return structure;
}

QString RNAPKplex::initiate(QString filepath, QMap<QString, double> opt) {


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
    rnaFoldArgs << getOptionList(opt);

    //start RNAfold;
    proc->setProgram(lib_path);
    proc->setStandardInputFile(seq->fileName());
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
QStringList RNAPKplex::getOptionList(QMap<QString, double> opt) {

    QStringList options;

    if(opt.value("No closing GU pairs:") == 1) {
        options << "--noClosingGU";
    }
    if(opt.value("No lonely pairs")==1) {
        options << "--noLP";
    }
    if(opt.value("No GU pairs")==1) {
        options << "--noGU";
    }
    QString temp = "-T";
    temp.append(QString::number(opt.value("Enter temp (default 37C): ")));
    options << temp;

    temp = "-e";
    temp.append(QString::number(opt.value("Enter energy cutoff (default -8.10):")));
    options << temp;

    return options;
}


extern "C" MYEXPORT IAlgorithm* create_RNAPKplex() {
    return new RNAPKplex();
}
