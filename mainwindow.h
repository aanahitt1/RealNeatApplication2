#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenu>
#include <QString>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QGridLayout>
#include <QLabel>
#include <QSharedPointer>
#include "parsexml.h"
#include "RNALIB/rnalib.h"
#include "IAlgorithm.h"
#include "optionchooser.h"
#include "graphicswindow.h"
#include "pluginmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_listItem_changed(QListWidgetItem* list, QLabel* description, QString* names, QString* desc);

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    void createFileBar();
    QDialog* createAlgoList();
    bool checkSequence(QString* sequence);
    IAlgorithm* loader(QString* filepath, QString name);
    void showTab(QMap<QString, double>, IAlgorithm*);
    void populateList(QString*);

    QString file_path;
    QString directory_path;
    QListWidget* algoList;
    int FASTAcount;
    GraphicsWindow* tabWindow;
    PluginManager* manager;

};
#endif // MAINWINDOW_H
