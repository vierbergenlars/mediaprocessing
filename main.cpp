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
    WorldController controller;
    MainWindow mainWindow(controller);

    if(argc < 2) {
        std::cerr << "Pass the world file to use as first parameter" << std::endl;
        return 1;
    }

    try {
        mainWindow.createWorld(argv[1]);
    } catch(const QString &err) {
        std::cerr << err.toStdString() << std::endl;
        return 2;
    }
    controller.findPath();
    mainWindow.show();


    return a.exec();
}
