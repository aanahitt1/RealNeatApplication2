#ifndef IALGORITHM_H
#define IALGORITHM_H

#include <QtPlugin>

class IAlgorithm {

private:
    QString file_path;

public:
    virtual QString getStructure() = 0;
    virtual QStringList getInfo() = 0;
    virtual QMap<QString, double> getOptions() = 0;
    virtual QString initiate(QString filepath, QMap<QString, double> opt) = 0;
    virtual ~IAlgorithm() {};

};

#define IAlgorithm_iid "IAlgorithm"

Q_DECLARE_INTERFACE(IAlgorithm, IAlgorithm_iid);

#endif // IALGORITHM_H
