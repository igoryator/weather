#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


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


    gridLayout = new QGridLayout();
    gridLayout->setGeometry(this->geometry());

    weather = new weatherView(&wModel, this);

    gridLayout->addWidget(weather);


    ui->centralWidget->setLayout(gridLayout);

}

MainWindow::~MainWindow()
{

    delete weather;
    delete gridLayout;
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
    QString url  = "https://api.weather.yandex.ru/v1/forecast?lat=%1&lon=%2&extra=true&limit=3";
    url = url.arg(lat).arg(lon);

    request.setUrl(url);
    request.setRawHeader("X-Yandex-API-Key", "7392ad59-8cdb-4693-9a95-66913c1949ec");
    //request.setRawHeader("Content-Type", "text/html");
    //request.setRawHeader("Accept", "*/*");

    reply = manager.get(request);//Получаем данные с сервера
    connect(reply, SIGNAL(finished()),this,SLOT(getReplyFinished()));





}


void MainWindow::getReplyFinished(){


    if(reply->isReadable()){


        QByteArray responce = reply->readAll();

        wModel.setWeatherData(responce);
        weather->renderIcons();
        weather->renderPix();
        weather->update();
    } else {


        QMessageBox::warning(this,"Warning", "Can't get weather data");

    }


    reply->deleteLater();

}
