#ifndef WEATHERVIEW_H
#define WEATHERVIEW_H

#include <QWidget>
#include "weathermodel.h"

class weatherView : public QWidget
{
    Q_OBJECT
public:
    explicit weatherView( weatherModel *m,QWidget *parent = nullptr);


signals:

public slots:


protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);


private:

    QVector<QRectF> days;
    QVector<QRectF> hourly;
    weatherModel* model;
    int daySelected = 0;
};

#endif // WEATHERVIEW_H
