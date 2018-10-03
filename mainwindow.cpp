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
        //updateTimer.start();





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

    QString str = coordinate.toString();

    ui->statusBar->showMessage(coordinate.toString());

    QString lat = QString().setNum(coordinate.latitude(),'f',6);
    QString lon = QString().setNum(coordinate.longitude(),'f',6);


    QNetworkRequest request;
    QString url  = "https://api.weather.yandex.ru/v1/forecast?lat=%1&lon=%2&extra=true";
    url = url.arg(lat).arg(lon);

    request.setUrl(url);
    request.setRawHeader("X-Yandex-API-Key", "23b11d43-a334-48e0-b86e-1bc7ca0e76f0");
    //request.setRawHeader("Content-Type", "text/html");
    //request.setRawHeader("Accept", "*/*");

    reply = manager.get(request);//Получаем данные с сервера
    connect(reply, SIGNAL(finished()),this,SLOT(getReplyFinished()));




}


void MainWindow::getReplyFinished(){



    QByteArray responce = reply->readAll();

    ui->textBrowser->clear();
    ui->textBrowser->append(responce);

    reply->deleteLater();

}
