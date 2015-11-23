#include <QApplication>
#include <QTranslator>
#include "Kugou.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("InternationalLanguage.qm");
    app.installTranslator(&qtTranslator);

    Kugou w;
    w.show();

    return app.exec();
}
