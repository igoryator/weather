#include "weathermodel.h"

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


    modelData = jsonData;


}


QImage weatherModel::iconForDay(unsigned int day){

    if(day>daysCount()){

        return QImage();

    }


    return QImage(":/icons/15.png");


}
