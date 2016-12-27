#include <iostream>
#include <world.h>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QMainWindow>
#include <cmath>
#include "graphicstile.h"
#include "matrix.h"
#include "worldcontroller.h"
#include "mainwindow.h"

using namespace std;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}
