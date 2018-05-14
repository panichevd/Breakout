#include "Breakout.h"
#include "Geometry.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Breakout w;
    w.setWindowTitle("Breakout");
    w.show();

    return a.exec();

    return 0;
}
