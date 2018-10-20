#include "weathermodel.h"
#include <QJsonObject>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QApplication>
#include <QJsonArray>
#include <QDate>
#include <QDateTime>
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



    if(images.contains(iconCode)){


        return images.take(iconCode);


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

            images.insert(iconCode,imageData);
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



    } else {

        QJsonValue forecast = json.object()["forecasts"];

        if(!forecast.isArray()){


            return QByteArray();
        }


        QJsonArray forec = forecast.toArray();

        for(int n=0;n<forec.size();n++){


            QJsonValue dayForecast = forec.at(n);

            if(dayForecast.isObject()){


                QJsonObject dayForec = dayForecast.toObject();

                QDate date = QDate::fromString(dayForec["date"].toString(),"yyyy-MM-dd");
                QDate currentDate = QDate::currentDate();

                if(currentDate.daysTo(date)!=day) continue;

                // dayForec/parts/day/icon


                QString iconCode = dayForec["parts"].toObject()["day"].toObject()["icon"].toString();

                return getImage(iconCode);


            }

        }


    }


    return QByteArray();


}
