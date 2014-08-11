#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "homography_image.h"
#include "openni_image.h"
#include "level_analyzed_image.h"

#if defined _WIN32 || defined _WIN64
#include "pcsdk_image.h"
#endif

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<MontBlanc::OpenCVImage>("MontBlanc", 1, 0, "OpenCVImage");
    qmlRegisterType<MontBlanc::HomographyImage>("MontBlanc", 1, 0, "HomographyImage");
    qmlRegisterType<MontBlanc::OpenNIImage>("MontBlanc", 1, 0, "OpenNIImage");
    qmlRegisterType<MontBlanc::LevelAnalyzedImage>("MontBlanc", 1, 0, "LevelAnalyzedImage");

#if defined _WIN32 || defined _WIN64
    qmlRegisterType<MontBlanc::PCSDKImage>("MontBlanc", 1, 0, "PCSDKImage");
#endif

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    return app.exec();
}
