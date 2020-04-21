#ifndef SCREENGRUBBER_H
#define SCREENGRUBBER_H

#include <QObject>
#include <QTimer>

class ScreenGrubber : public QObject
{
    Q_OBJECT
public:
    explicit ScreenGrubber(QObject *parent = nullptr);
    void startGrub(int updateInterval = 25);
    Q_SIGNAL void newScreenPix(const QPixmap &pix);
    Q_SIGNAL void newScreenImg(const QImage &img);
public slots:


private:
    QTimer *refreshTimer;
    Q_SLOT void grubScreen();
};

#endif // SCREENGRUBBER_H
