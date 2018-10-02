#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLocation>
#include <QtPositioning>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:


    void geoUpdated(QGeoPositionInfo update);
    void geoTimeout();
    void geoError(QGeoPositionInfoSource::Error positioningError);
    void requestUpdate();

private:
    Ui::MainWindow *ui;
    QGeoPositionInfoSource* geoSource = 0;
    QTimer updateTimer;
};

#endif // MAINWINDOW_H
