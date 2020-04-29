#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QFileDialog>

#include <QPalette>
#include <QSvgWidget>
#include <QScrollArea>
#include <QScrollBar>


#include "graphicswindow.h"
#include "rnamodelgen.h"

GraphicsWindow::GraphicsWindow()
{
   tabWindow = new QTabWidget();
}

void GraphicsWindow::show(QString filepath){



    //This generates the 2D plot
    RNAModelGen *rnaGen = new RNAModelGen();
    rnaGen->generate2DModel(filepath);

    QFileInfo getName(filepath);

   QString fileName = ".\\";
   fileName.append(getName.baseName());
   fileName.append("_ss.svg");
   svgImage = new QSvgWidget(fileName);

   svgImage->resize(500,500);
   svgImage->setStyleSheet("background-color:white");

   QPalette pal(QPalette::Background, Qt::white);


   //Sets the area for our image and makes it scrollable
   QScrollArea *area = new QScrollArea();
   area->setWidget(svgImage);
   area->setAlignment(Qt::AlignCenter);
   area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   area->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
   area->verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");
   area->setPalette(pal);

   //This gets the name of the file for the new tab
   QFileInfo* temp = new QFileInfo(filepath);
   QString name = temp->baseName();

   //Adds the tab to our base widget
   tabWindow->addTab(area, QString(name));
   tabWindow->show();
}

void GraphicsWindow::processDOne(QProcess::ProcessError error)
{
    qInfo() << error;
}


