#include "rnafold.h"

#ifdef Q_OS_WINDOWS
#define MYEXPORT __declspec(dllexport)
#else
#define MYEXPORT
#endif

Q_PLUGIN_METADATA(IAlgorithm_IID FILE RNAFold.json)
Q_INTERFACES(IAlgorithm)

/*RNAFold::RNAFold(){
    //placeholder, generalization needed
    //initiate("RNAfold.exe", );
}
*/
/*RNAFold::RNAFold(QString _sequence){
    QTextStream cout(stdout);
    cout << "Beginning construction";

}
*/

//From a filepath with the .exe location and a list of options, construct and pass a command line argument.
QString RNAFold::initiate(QString filepath, QMap<QString, double> opt){
    file_path = filepath;

    runProc("AGUAGUAGUAUC");

    return structure;
}



QString RNAFold::getStructure(){
    return structure;
}
QStringList RNAFold::getInfo(){
    return info;
}
QMap<QString,double> RNAFold::getOptions(){
    return map;
}
/*
void RNAFold::findDir(QString _path){
    this->path = &_path;
    QString lib = "..\\lib\\";
    QString defPath = lib + path;
    directory = new QDir(*path);

    if(!directory->exists()){
        //We haven't been passed a complete path
        //Attempt to find it
        directory = new QDir(defPath);
    }else if(!directory->exists()){
        qWarning("Cannot find directory");
    }
}
*/
/*
void RNAFold::findJson(){
    QString fileName = *path + "\\RNAFold.json";
    QStringList filters;
    filters << fileName;
    directory->setNameFilters(filters);

    QStringList file;
    if((file = directory->entryList(filters)).isEmpty()){
        qWarning("Cannot find plugin");
    }
    doc = new QJsonDocument(QJsonDocument::fromJson(file.first().toLocal8Bit()));

}
*/
//Extracts the plugin details and creates the objects to send to the user interface.
/*void RNAFold::extractJsonData(){
    pluginMap = new QVariantMap(doc->object().toVariantMap());

    QVariant var;
    var = pluginMap->value("options");

    QSequentialIterable itrVar = var.value<QSequentialIterable>();

    //Assemble a map of Keys:Commands and a map of Keys:default values.
    for(const QVariant &v : itrVar){
       QMap<QString,QVariant> tempMap;
       tempMap = v.toMap();
       QString tempVal = tempMap.value("command").toString();
       QMap<QString,double> tempKV = tempMap.value("pass").value<QMap<QString,double>>();
       commandMap.insert(tempKV.firstKey(),tempVal);
       map.unite(tempMap.value("pass").value<QMap<QString,double>>());
    }

    //Alternate method, needs testing
    //Using text as key is problematic for general parsing.
    //QJsonArray testArr = doc->object()["options"].toArray();

    //for(const QJsonValue &val: testArr){
    //    QJsonObject o = val.toObject();
        //commandMap.insert()
    //}

}
*/
/*
void RNAFold::TESTMETHOD(QString libpath){
    QFile rFile;
    QString content;

    QMap<QString, double> passingParameters;
    QStringList commands;
    QJsonValue values;
    QVariantMap passMap;

    const QString fPath = libpath + "\\RNAFold.json";

    //From the plugin library path, find .json file.
    rFile.setFileName(fPath);
    rFile.open(QIODevice::ReadOnly | QIODevice::Text);
    content = rFile.readAll();
    rFile.close();

    //From the .json file, create a QJsonDocument and extract the contents
    //into a QJsonObject.
        QJsonDocument fDoc = QJsonDocument::fromJson(content.toUtf8());
        QJsonObject sett = fDoc.object();
        QJsonValue options = sett.value(QString("options"));
        QJsonArray arr = options.toArray();

    for(int i = 0; i < arr.size(); i++){
        values = arr.at(i);
        commands.append(values["command"].toString());
        passMap.unite(values["pass"].toObject().toVariantMap());
    }
}
*/

//Creates the QStringList of arguments which will be used to run the process.
/*
QStringList RNAFold::uniteMaps(QMap<QString,double> usrOpts){
    QStringList args;
    QString comm;
    double val;
    QString argument;
    for(auto k : usrOpts.keys()){
        comm = commandMap.value(k);
        if(usrOpts.value(k) != 0.0){
            val = usrOpts.value(k);
            args << comm + " " + val;
        }else{
            args << comm;
        }
    }
    return args;
}
*/
void RNAFold::runProc(QString rnaSequence){

    //Replace hard coded value with var. when stable.
    QString rnaFoldCmd = "..//";
    QStringList rnaFoldArgs;

    proc = new QProcess();

    // generate temp file for RNAFold input
    QTemporaryFile *seq = new QTemporaryFile();
    seq->setAutoRemove(false);

    if(!seq->open()){
        printf("error opening temp file.");
    }

    QTextStream out(seq);
    out << rnaSequence;
    out.flush(); // write sequence immediately to file
    seq->waitForReadyRead(-1);

    if(!seq->isReadable())
        printf("not reay to read file");

    // set process argumemts
    //TODO:Remember how these are going to be returned to this class
    rnaFoldArgs << "--noPS" << seq->fileName();

    //start RNAfold;
    proc->setProgram(rnaFoldCmd);
    proc->setArguments(rnaFoldArgs);
    proc->setStandardOutputFile(structure);
    proc->start(QIODevice::ReadWrite);
    proc->waitForFinished();
}

RNAFold::~RNAFold(){}

//extern "C" MYEXPORT IAlgorithm* create_RNAFold() {
//    return new RNAFold("C://Users//jjsta//OneDrive//Documents//QT_Projects//RNAFold.json");
//}
