#include "graphicswindow.h"

GraphicsWindow::GraphicsWindow()
{
   tabWindow = new QTabWidget();
}

void GraphicsWindow::show(QString filepath){



   //This generates the 2D plot
   RNAModelGen *rnaGen = new RNAModelGen();
   QFile* svg_image = rnaGen->generate2DModel(filepath);
   svgImage = new QSvgWidget( svg_image->fileName() );

    displayTab(rnaGen);

   //remove file that is loaded so directory isn't over filled
   // svg_image->remove();
}

void GraphicsWindow::showHighlighted(QString struc1, QString struc2){
    //This generates the 2D plot
    RNAModelGen *rnaGen = new RNAModelGen();
    QFile* svg_image = rnaGen->generate2DModel(struc1);

    //add highlights to RNA SVG picture
    rnaGen->highlight2DModel(svg_image->fileName(), struc1, struc2);

    displayTab(rnaGen);
}

void GraphicsWindow::displayTab(RNAModelGen* RNAModel){

    svgImage = new QSvgWidget( RNAModel->get2DModelFile() );
    svgImage->resize(500,500);
    svgImage->setStyleSheet("background-color:white");

    QPalette pal(QPalette::Background, Qt::white);

    //Sets the area for our image and makes it scrollable
    QScrollArea *area = new QScrollArea();
    area->setWidget(svgImage);
    area->setAlignment(Qt::AlignCenter);
    area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    area->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    area->verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");
    area->setPalette(pal);

    //Adds the tab to our base widget
    tabWindow->addTab(area, RNAModel->get2DModelName());
    tabWindow->show();
}

void GraphicsWindow::processDOne(QProcess::ProcessError error)
{
    qInfo() << error;
}


