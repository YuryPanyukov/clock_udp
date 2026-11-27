#include <QApplication>
#include "serverwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("YuryOrg");
    QCoreApplication::setApplicationName("ClockServer");

    ServerWindow w;
    w.show();
    return app.exec();
}