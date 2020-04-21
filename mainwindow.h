#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <screengrubber.h>

//#include <tesseract/baseapi.h>

//#include "opencv2/core.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/highgui.hpp"

//#include <opencv2/core/types_c.h>
//#include <opencv2/core/mat.hpp>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void initUi();
private:
 //   Q_SLOT void getRecognitionText(const QImage &img);
    QLabel *sourceImage;
    QLabel *textLbl;
    ScreenGrubber *sg;
 //   tesseract::TessBaseAPI *ocr;
 //  void qimage_to_mat(const QImage &image, cv::OutputArray out);
 //   void mat_to_qimage(cv::InputArray image, QImage &out);
};

#endif // MAINWINDOW_H
