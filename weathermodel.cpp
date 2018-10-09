#include "weathermodel.h"
#include <QJsonObject>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QApplication>

weatherModel::weatherModel(QObject *parent) : QObject(parent)
{

}



unsigned int weatherModel::daysCount(){


    return 3;

}



unsigned int weatherModel::hourlyCount(unsigned int day){


    if(day>daysCount()){

        return 0;

    }

    return 10;

}



void weatherModel::setWeatherData(QByteArray &jsonData){


    QJsonParseError err;
    json = QJsonDocument::fromJson(jsonData, &err);
    if(err.error==QJsonParseError::NoError){

        isDataLoaded = true;

    }



}



QByteArray weatherModel::getImage(QString iconCode){




        QString fileName = QString("%1.svg").arg(iconCode);


        if(QFile::exists(fileName)){

            QFile file(fileName);

            QByteArray imageData;

            if(file.open(QIODevice::ReadOnly)){

                imageData = file.readAll();
                file.close();
            }

            return imageData;

        } else {

            QString url = QString("https://yastatic.net/weather/i/icons/blueye/color/svg/%1.svg").arg(iconCode);
            QNetworkRequest request;
            request.setUrl(url);
            QNetworkAccessManager manager;
            QNetworkReply* reply = manager.get(request);

            while (!reply->isFinished()) {

                QApplication::processEvents();
            }


            if(reply->isFinished()){


                QByteArray imageData = reply->readAll();
                reply->deleteLater();

                QFile file(fileName);
                if(file.open(QIODevice::WriteOnly)){


                    file.write(imageData);
                    file.close();

                }


                return imageData;


            } else {

                reply->deleteLater();
                return QByteArray();
            }



        }









}



QByteArray weatherModel::iconForDay(unsigned int day){

    if(day>daysCount()){

        return QByteArray();

    }

    if(!isDataLoaded){

        return QByteArray();
    }


    if(day==0){

        // show current icon

        QJsonValue fact = json.object()["fact"];
        if(fact.isObject()){

            QJsonObject factObject = fact.toObject();

            QJsonValue currentCondition = factObject["condition"];
            QString daytime = factObject["daytime"].toString();
            QString conditionString = currentCondition.toString();
            QString iconCode = factObject["icon"].toString();

            return getImage(iconCode);












        }



    }


    return QByteArray();


}
