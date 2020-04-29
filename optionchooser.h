#ifndef OPTIONCHOOSER_H
#define OPTIONCHOOSER_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include <QMapIterator>
#include <QInputDialog>
#include <QMessageBox>

class OptionChooser : public QObject
{
    Q_OBJECT

private:
    QMap<QString, double> options;
    QListWidget* optList;
    QDialog* main;

    void emitBoxClosed();

public:
    OptionChooser(QMap<QString, double> opt);

    void getNumber(QListWidgetItem* item);
    void show();

signals:
   void boxClosed(QMap<QString, double>);
};

#endif // OPTIONCHOOSER_H
