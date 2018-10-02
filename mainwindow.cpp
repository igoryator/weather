#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    geoSource = QGeoPositionInfoSource::createDefaultSource(this);

    if(geoSource){

        connect(geoSource,SIGNAL(error(QGeoPositionInfoSource::Error)),this,SLOT(geoError(QGeoPositionInfoSource::Error)));
        connect(geoSource,SIGNAL(updateTimeout()),this,SLOT(geoTimeout()));
        connect(geoSource,SIGNAL(positionUpdated(QGeoPositionInfo)),this,SLOT(geoUpdated(QGeoPositionInfo)));

        updateTimer.setInterval(20000);
        connect(&updateTimer,SIGNAL(timeout()),this,SLOT(requestUpdate()));
        requestUpdate();
        updateTimer.start();





    } else {

        ui->statusBar->showMessage("No coordinate source");

    }


    int z = 0;


}

MainWindow::~MainWindow()
{

    if(geoSource)
        delete geoSource;
    delete ui;
}



void MainWindow::requestUpdate(){


    geoSource->requestUpdate(10000);

}

void MainWindow::geoError(QGeoPositionInfoSource::Error positioningError){



    ui->statusBar->showMessage("Error occured");


}

void MainWindow::geoTimeout(){


    ui->statusBar->showMessage("Get coordinates timeout");

}


void MainWindow::geoUpdated(QGeoPositionInfo update){


    QGeoCoordinate coordinate =  update.coordinate();

    ui->statusBar->showMessage(coordinate.toString());



}
