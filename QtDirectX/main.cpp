#include <QApplication>
#include "TriangleApp.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget* mainWidget = new TriangleApp();
    mainWidget->show();
    mainWidget->resize(960,640);

    int result = app.exec();
    delete mainWidget;
    return result;
}