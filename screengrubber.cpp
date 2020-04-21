#include "screengrubber.h"
#include <QScreen>
#include <QImage>
#include <QPixmap>
#include <QGuiApplication>

ScreenGrubber::ScreenGrubber(QObject *parent) : QObject(parent)
{
    refreshTimer = new QTimer();
    refreshTimer->setInterval(10);
    connect(refreshTimer,SIGNAL(timeout()),this,SLOT(grubScreen()));
}

void ScreenGrubber::startGrub(int updateInterval)
{
    refreshTimer->start(1000/updateInterval);
}


void ScreenGrubber::grubScreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QImage img = screen->grabWindow(0,300,100,400,300).toImage();
    img = img.scaled(1600,1200,Qt::KeepAspectRatio);
    emit newScreenPix(QPixmap::fromImage(img));
    emit newScreenImg(img);
}
