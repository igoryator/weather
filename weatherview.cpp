#include "weatherview.h"
#include <QPainter>
#include <QResizeEvent>
#include <QSvgRenderer>
weatherView::weatherView(weatherModel* m, QWidget *parent) : QWidget(parent)
{

    setGeometry(parent->rect());
    model = m;

}







void weatherView::resizeEvent(QResizeEvent *event){

    setGeometry(0,0,event->size().width(),event->size().height());

    QWidget::resizeEvent(event);

}

void weatherView::paintEvent(QPaintEvent *event){


    QPainter painter(this);

    QPen pen = painter.pen();
    pen.setWidth(3);
    painter.setPen(pen);

    unsigned int daysCount = model->daysCount();
    days.clear();


    float dayWidth = width()/daysCount;
    float dayX = 0;
    for(unsigned int n=0;n<daysCount;n++){

        days.append(QRectF(dayX,0,dayWidth,height()/2));
        dayX += dayWidth;

    }
    painter.drawRects(days);

    hourly.clear();
    unsigned int hourlyCount = model->hourlyCount(daySelected);
    float hourlyWidth = width()/hourlyCount;
    float hourlyX = 0;

    for(unsigned int n=0;n<hourlyCount;n++){


        hourly.append(QRectF(hourlyX,height()/2,hourlyWidth,height()/2));
        hourlyX += hourlyWidth;

    }

    painter.drawRects(hourly);


    for(unsigned int n=0;n<daysCount;n++){

        QSvgRenderer renderer;
        renderer.load(model->iconForDay(n));
        renderer.render(&painter,days.at(n));

        //painter.drawImage(days.at(n), model->iconForDay(n));

    }




}
