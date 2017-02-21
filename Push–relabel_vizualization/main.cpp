#include "mainwindow.h"
#include <QApplication>
#include <QPainter>
#include <QPen>
#include <QPoint>

#include <iostream>

static const size_t V = 10;
static const size_t E = 16;
static const long long CAP_MAX = 10;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Network net(V, 0, V - 1);
    net.generateNetwork(E, CAP_MAX);
    MainWindow w(net);
    w.show();
    return a.exec();
}
