#ifndef RNALIB_H
#define RNALIB_H

#include "RNALIB_global.h"
#include <QString>
#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <QFileDialog>

class RNALIB
{
public:
    RNALIB();

    static QString stringToFasta(QString, QString, QString);
};

#endif // RNALIB_H
