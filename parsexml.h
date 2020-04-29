#ifndef PARSEXML_H
#define PARSEXML_H


// Library needed for processing XML documents
#include <QtXml>
// Library needed for processing files
#include <QFile>
#include <QString>

class ParseXML
{
public:

    //This function will return a string of names from an XML document
    QString* getInfo(QString FileName, QString tag) {
        //The QDomDocument class represents an XML document.
        QDomDocument xmlBOM;
        // Load xml file as raw data
        QFile f(FileName);
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

        //Find the names and store them
        QDomElement root = xmlBOM.documentElement();
        QDomElement program = root.firstChild().toElement();
        QString* names = new QString[10];
        QString type = program.tagName();
        int i = 0;
        while(!program.isNull()) {
            if(program.tagName() == "Program") {
                QString name = program.attribute(tag);
                names[i] = name;
                i++;
            }
            program = program.nextSibling().toElement();
        }

        names[i] = "NON";

        return names;

    }

    QString* getPath(QString fileName, int i);
    void removeByName(QString fileName, QString algoName);
    void addItem(QString, QString, QString, QString);
};

#endif // PARSEXML_H
