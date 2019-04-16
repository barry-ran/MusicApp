#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "musicApp.h"
#include "util.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<MusicApp>("MusicAppCore", 1, 0, "MusicApp");
    qmlRegisterType<Util>("MusicAppCore", 1, 0, "Util");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    auto window = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
    window->setIcon(QIcon(":/image/logo.png"));

    return app.exec();
}
