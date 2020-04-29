#ifndef IALGORITHM_H
#define IALGORITHM_H

#include <QtPlugin>

class IAlgorithm {

private:
    QString file_path;

public: 
    //getStructure() - returns a dot-bracket notation of the secondary structure as a QString
    virtual QString getStructure() = 0;

    //returns any additional information the program gives in a QStringList as visible by viewer, eg. "Free energy is x". Can be blank.
    virtual QStringList getInfo() = 0;

    //Options should be created in header, so getOptions() can be called without doing anything other than creating the object.
    //Returns a QStringList of options as visible by user
        //If a number is to be entered, the QString should start with the word "Enter"
    virtual QMap<QString, double> getOptions() = 0;

    //Initiate with filepath to FASTA file and a QStringList of options, should run the program with given info and store everything until called
    //"Opt" is given as a QStringList of the same length as the one given when getOptions() is called, filled with 1 (chosen), 0 (not chosen),
        //or the number asked for. The plug in must convert this to the right commands
    virtual QString initiate(QString filepath, QMap<QString, double> opt);
    virtual ~IAlgorithm(){};

};

#define IAlgorithm_iid "IAlgorithm"

Q_DECLARE_INTERFACE(IAlgorithm, IAlgorithm_iid);

#endif // IALGORITHM_H
