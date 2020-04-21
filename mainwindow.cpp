#include "mainwindow.h"
#include <QLayout>
#include <QTimer>
#include <QScreen>
#include <QImage>
#include <QTextCodec>


//#include <leptonica/allheaders.h>



#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    initUi();
    sg = new ScreenGrubber();


  //  ocr = new tesseract::TessBaseAPI();
  //  ocr->Init("/home/rmi/Projects/pitometr/tessdata","rus",tesseract::OEM_LSTM_ONLY);
 //   ocr->SetPageSegMode(tesseract::PSM_AUTO);

  //  connect(sg,SIGNAL(newScreenPix(QPixmap)),sourceImage,SLOT(setPixmap(QPixmap)));
    connect(sg,SIGNAL(newScreenImg(QImage)),this,SLOT(getRecognitionText(QImage)));
    sg->startGrub();
}

void MainWindow::initUi()
{
    this->resize(1200,800);
    this->setWindowTitle("Питометр 0.0.1");

    QVBoxLayout *layout = new QVBoxLayout(this);

    sourceImage = new QLabel(this);
    sourceImage->setGeometry(10,10,1100,500);
   // sourceImage->setScaledContents(true);

    textLbl = new QLabel(this);
    textLbl->setGeometry(10,sourceImage->geometry().bottomLeft().y(),1100,250);

//    sourceImage->setLayout(layout);

}





//void MainWindow::qimage_to_mat(const QImage& image, cv::OutputArray out) {
//    using namespace cv;

//    switch(image.format()) {
//        case QImage::Format_Invalid:
//        {
//            Mat empty;
//            empty.copyTo(out);
//            break;
//        }
//        case QImage::Format_RGB32:
//        {
//            Mat view(image.height(),image.width(),CV_8UC4,(void *)image.constBits(),image.bytesPerLine());
//            view.copyTo(out);
//            break;
//        }
//        case QImage::Format_RGB888:
//        {
//            Mat view(image.height(),image.width(),CV_8UC3,(void *)image.constBits(),image.bytesPerLine());
//            cvtColor(view, out, COLOR_RGB2BGR);
//            break;
//        }
//        default:
//        {
//            QImage conv = image.convertToFormat(QImage::Format_ARGB32);
//            Mat view(conv.height(),conv.width(),CV_8UC4,(void *)conv.constBits(),conv.bytesPerLine());
//            view.copyTo(out);
//            break;
//        }
//    }
//}



//void MainWindow::mat_to_qimage(cv::InputArray image, QImage& out)
//{
//    using namespace cv;
//    switch(image.type())
//    {
//        case CV_8UC4:
//        {
//            Mat view(image.getMat());
//            QImage view2(view.data, view.cols, view.rows, view.step[0], QImage::Format_ARGB32);
//            out = view2.copy();
//            break;
//        }
//        case CV_8UC3:
//        {
//            Mat mat;
//            cvtColor(image, mat, COLOR_BGR2BGRA); //COLOR_BGR2RGB doesn't behave so use RGBA
//            QImage view(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_ARGB32);
//            out = view.copy();
//            break;
//        }
//        case CV_8UC1:
//        {
//            Mat mat;
//            cvtColor(image, mat, COLOR_GRAY2BGRA);
//            QImage view(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_ARGB32);
//            out = view.copy();
//            break;
//        }
//        default:
//        {
//           // throw invalid_argument("Image format not supported");
//            break;
//        }
//    }
//}









//void MainWindow::getRecognitionText(const QImage &img)
//{
//    using namespace cv;
//   // cv::Mat testImg = cv::imread("/home/rmi/Projects/pitometr/cvtext.jpg",cv::IMREAD_COLOR);

//    QImage image = img;
//    cv::Mat tmp(image.height(),image.width(),CV_8UC4,(uchar*)image.bits(),image.bytesPerLine());
//    cv::Mat resultcv; // deep copy just in case (my lack of knowledge with open cv)
//    cvtColor(tmp, resultcv,COLOR_BGRA2GRAY);

//    if (img.size().width() < 30)
//        return;
//  //  cv::Mat testImg;
//  //  qimage_to_mat(img,testImg);
//    QImage result;
//    mat_to_qimage(resultcv,result);
//    sourceImage->setPixmap(QPixmap::fromImage(result));
//   //  QImage tempImage = img;
//   //  testImg.data = tempImage.bits();
//    ocr->SetImage(resultcv.data,resultcv.cols,resultcv.rows,1,resultcv.step);

//    char* recognizedText = ocr->GetUTF8Text(); // recognizes text with tesseract
//    QTextCodec* codec = QTextCodec::codecForName("UTF-8"); // creates UTF-8 codec
//    QString decodedText = codec->toUnicode(recognizedText); // Converts to UNICODE
//    textLbl->setText(decodedText);
//    qDebug() << decodedText;



//}

