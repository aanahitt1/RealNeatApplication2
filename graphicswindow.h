#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QSvgWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QPalette>
#include <QSvgWidget>
#include <QScrollArea>
#include <QScrollBar>

#include "rnamodelgen.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GraphicsWindow; }
QT_END_NAMESPACE

class GraphicsWindow: public QMainWindow
{
    Q_OBJECT
public:
    GraphicsWindow();
    void show(QString filepath);
    void showHighlighted(QString, QString);

public slots:
void processDOne(QProcess::ProcessError);


private:
    QTabWidget* tabWindow;
    QSvgWidget *svgImage;
    Ui::GraphicsWindow *ui;
    void displayTab(RNAModelGen*);
};

#endif // GRAPHICSWINDOW_H
