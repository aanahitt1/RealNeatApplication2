#include "parsexml.h"

QString* ParseXML::getPath(QString fileName, int i) {
    //The QDomDocument class represents an XML document.
    QDomDocument xmlBOM;
    // Load xml file as raw data
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly ))
    {
        // Error while loading file
        QString *error = new QString();
        QString message = "Error while loading file.";
        error[0] = message;
        return error;
    }
    // Set data into the QDomDocument before processing
    xmlBOM.setContent(&f);
    f.close();

    QDomElement root = xmlBOM.documentElement();
    QDomElement program = root.firstChild().toElement();

    int j = 0;

    //This find the proper program in the list.
    while(!program.isNull()) {
        if(program.tagName()=="Program" && j == i) {
            QString* path = new QString();
            path[0] = program.text();
            return path;
        }
        program = program.nextSibling().toElement();
        j++;
    }

    QString* error = new QString("Element not found");
    return error;
}

void ParseXML::removeByName(QString fileName, QString algoName) {

    //The QDomDocument class represents an XML document.
    QDomDocument xmlBOM;
    // Load xml file as raw data
    QFile f(fileName);
    if (!f.open(QIODevice::ReadWrite))
    {
        qInfo() << "Not open";
        return;
    }
    // Set data into the QDomDocument before processing
    xmlBOM.setContent(&f);

    QDomElement root = xmlBOM.documentElement();
    QDomElement program = root.firstChild().toElement();

    //This find the proper program in the list and remove it.
    while(!program.isNull()) {
        if(program.tagName()=="Program") {
            if(program.attribute("name") == algoName) {
                QDomNode parent = program.parentNode();
                parent.removeChild(program);
                qInfo() << algoName;
            }
        }
        program = program.nextSibling().toElement();
    }

    f.resize(0);
    QTextStream out(&f);
    out << xmlBOM.toString();
    out.flush();
    f.close();

}

//Adds an item to the xml
void ParseXML::addItem(QString path, QString name, QString desc, QString filepath) {

    //The QDomDocument class represents an XML document.
    QDomDocument xmlBOM;
    // Load xml file as raw data
    QFile f(path);
    if (!f.open(QIODevice::ReadWrite))
    {
        qInfo() << "Not open";
        return;
    }
    // Set data into the QDomDocument before processing
    xmlBOM.setContent(&f);

    QDomElement root = xmlBOM.documentElement();

    //Create the new element
    QDomElement newalgo = xmlBOM.createElement("Program");
    newalgo.setAttribute("name", name);
    newalgo.setAttribute("desc", desc);
    QDomText newText = xmlBOM.createTextNode(filepath);
    newalgo.appendChild(newText);

    //Add node
    root.appendChild(newalgo);

    //Rewrite
    f.resize(0);
    QTextStream out(&f);
    out << xmlBOM.toString();
    out.flush();
    f.close();
}

