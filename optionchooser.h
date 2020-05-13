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
    QTabWidget* main;

    void emitBoxClosed();

public:
    OptionChooser(QList<QMap<QString, double>>* opt, QStringList* names);

    void getNumber(QListWidgetItem* item);
    void show();

signals:
   void boxClosed(QMap<QString, double>, int);
};

#endif // OPTIONCHOOSER_H
