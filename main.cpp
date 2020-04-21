#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQuickView>
#include <QQmlContext>

#include <QWindow>
#include <mainwindow.h>

#include <pitsmodel.h>




// TODO
// sort после добавления цели и обновления времени
// client server logic
// update gui


int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
//    MainWindow *window = new MainWindow();
//    window->show();
    QQmlApplicationEngine engine;
    ALLTargetModel *allTargetmodel = new ALLTargetModel();
    PitsModel *model = new PitsModel(allTargetmodel->getAllTargets());
    engine.rootContext()->setContextProperty("pitsModel", model);
    engine.rootContext()->setContextProperty("allTargetsModel", allTargetmodel);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
