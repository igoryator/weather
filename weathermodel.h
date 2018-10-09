#ifndef WEATHERMODEL_H
#define WEATHERMODEL_H

#include <QObject>
#include <QImage>
#include <QJsonDocument>

class weatherModel : public QObject
{
    Q_OBJECT
public:
    explicit weatherModel(QObject *parent = nullptr);

    void setWeatherData(QByteArray& jsonData);

    unsigned int daysCount();
    QImage iconForDay(unsigned int day);

    unsigned int hourlyCount(unsigned int day);

signals:

public slots:

private:

    QByteArray modelData;
    QJsonDocument json;
    bool isDataLoaded = false;
};

#endif // WEATHERMODEL_H
