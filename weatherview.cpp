#include "weatherview.h"
#include <QPainter>
#include <QResizeEvent>
#include <QSvgRenderer>
weatherView::weatherView(weatherModel* m, QWidget *parent) : QWidget(parent)
{

    if(parent)setGeometry(parent->geometry());
    else setGeometry(200,200, 320, 240);
    model = m;
    renderIcons();
}



void weatherView::renderPix(){



    QImage locpix = QImage(width(),height(),QImage::Format_ARGB32);
    locpix.fill(0);
    calcRects();


    QPainter painter(&locpix);


    QPen pen = painter.pen();
    pen.setWidth(3);
    pen.setColor(QColor(128,128,128,128));
    painter.setPen(pen);

    painter.drawRects(days);
    painter.drawRects(hourly);
    painter.drawRects(daysTemp);
    painter.drawRects(hourlyTemp);

    QFont font("Helvetica");
    font.setPointSize(height()/12);

    painter.setFont(font);
    unsigned int daysCount = model->daysCount();
    unsigned int hourlyCount = model->hourlyCount(daySelected);

    pen = painter.pen();
    pen.setWidth(3);
    pen.setColor(QColor(255,128,128,128));
    painter.setPen(pen);
    for(unsigned int n=0;n<daysCount;n++){

        painter.drawImage(days.at(n),dayIcons.at(n));
        painter.drawText(daysTemp.at(n),model->tempForDay(n),QTextOption(Qt::AlignCenter));

    }

    for(unsigned int n=0;n<hourlyCount;n++){

        painter.drawImage(hourly.at(n),hourIcons.at(n));
        painter.drawText(hourlyTemp.at(n),model->tempForHour(daySelected,n),QTextOption(Qt::AlignCenter));
    }


    pix = locpix;
}



void weatherView::resizeEvent(QResizeEvent *event){

    if(isResizeInProgress) return;

    isResizeInProgress = true;
    setGeometry(0,0,event->size().width(),event->size().height());
    renderPix();
    event->accept();
    isResizeInProgress = false;

}

void weatherView::calcRects(){


    qreal w,h;

    if(width()>=height()*1.5){

        w = height()*1.5;
        h = height();

    } else {

        h = width()/1.5;
        w = width();

    }

    unsigned int daysCount = model->daysCount();



    float dayWidth = w/daysCount;
    float dayX = 0;
    for(unsigned int n=0;n<daysCount;n++){

        if(n<days.size()) days.replace(n,QRectF(dayX,0,dayWidth,h*0.5));
        else days.append(QRectF(dayX,0,dayWidth,h*0.5));

        if(n<daysTemp.size()) daysTemp.replace(n, QRectF(dayX,h*0.5,dayWidth,h*0.15));
        else daysTemp.append(QRectF(dayX,h*0.5,dayWidth,h*0.15));

        dayX += dayWidth;

    }



    unsigned int hourlyCount = model->hourlyCount(daySelected);
    float hourlyWidth = w/hourlyCount;
    float hourlyX = 0;

    for(unsigned int n=0;n<hourlyCount;n++){


        if(n<hourly.size()) hourly.replace(n,QRectF(hourlyX,h*0.65,hourlyWidth,h*0.25));
        else hourly.append(QRectF(hourlyX,h*0.65,hourlyWidth,h*0.25));

        if(n<hourlyTemp.size()) hourlyTemp.replace(n, QRectF(hourlyX, h*0.9,hourlyWidth,h*0.1));
        else hourlyTemp.append(QRectF(hourlyX, h*0.9,hourlyWidth,h*0.1));

        hourlyX += hourlyWidth;

    }


}

void weatherView::renderIcons(){

    calcRects();
    QSvgRenderer renderer;
    unsigned int daysCount = model->daysCount();
    unsigned int hourlyCount = model->hourlyCount(daySelected);

    for(unsigned int n=0;n<daysCount;n++){

        QImage ic(640, 640, QImage::Format_ARGB32);
        ic.fill(0);
        QPainter p(&ic);
        renderer.load(model->iconForDay(n));
        renderer.render(&p);


        if(n<dayIcons.size())dayIcons.replace(n,ic);
        else dayIcons.append(ic);

    }

    for(unsigned int n=0;n<hourlyCount;n++){

        QImage ic(640, 640, QImage::Format_ARGB32);
        ic.fill(0);
        QPainter p(&ic);
        renderer.load(model->iconForHour(daySelected,n));
        renderer.render(&p);

        if(n<hourIcons.size()) hourIcons.replace(n,ic);
        else hourIcons.append(ic);

    }

}


void weatherView::paintEvent(QPaintEvent *event){

    if(isResizeInProgress) {
        event->accept();
        return;
    }
    if(isPaintInProgress) return;

    isPaintInProgress = true;

    QPainter painter(this);
    painter.drawImage(0,0,pix);

    painter.end();
    while(painter.isActive());

    event->accept();
    isPaintInProgress = false;
}

void weatherView::mousePressEvent(QMouseEvent *event){



    QPoint position = event->pos();

    for(int n=0;n<days.count();n++){

        if(position.x()<=days.at(n).left()) continue;
        if(position.x()>=days.at(n).right()) continue;
        if(position.y()<=days.at(n).top()) continue;
        if(position.y()>=days.at(n).bottom()) continue;

        daySelected = n;
        renderIcons();
        renderPix();
        update();

    }




    QWidget::mousePressEvent(event);
}

