#ifndef WEATHERVIEW_H
#define WEATHERVIEW_H

#include <QWidget>
#include "weathermodel.h"

class weatherView : public QWidget
{
    Q_OBJECT
public:
    explicit weatherView( weatherModel *m,QWidget *parent = nullptr);
    void renderIcons();


signals:

    void needRender();

public slots:

    void renderPix();

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent* event);


private:

    QVector<QRectF> days;
    QVector<QRectF> hourly;
    QVector<QRectF> daysTemp;
    QVector<QRectF> hourlyTemp;
    QVector<QImage> dayIcons;
    QVector<QImage> hourIcons;
    weatherModel* model;
    int daySelected = 0;
    bool isPaintInProgress = false;
    bool isResizeInProgress = false;
    void calcRects();
    QImage pix;
};

#endif // WEATHERVIEW_H
