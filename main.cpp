#include <iostream>
#include <world.h>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QMainWindow>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow mainWindow;

    QGraphicsScene graphicsScene;
    QGraphicsView graphicsView(&graphicsScene, &mainWindow);

    mainWindow.setCentralWidget(&graphicsView);

    if(argc < 2) {
        std::cerr << "Pass the world file to use as first parameter" << std::endl;
        return 1;
    }
    World *w = new World();

    try {
        std::vector<std::unique_ptr<Tile>> tiles = w->createWorld(argv[1]);

        for(std::unique_ptr<Tile> &tile: tiles) {
            QColor color;
            if(isinf(tile->getValue())) {
                // Use red for impassable areas
                color = QColor(0xff, 0, 0);
            } else {
                int colorValue(tile->getValue()*0xff);
                color = QColor(colorValue, colorValue, colorValue);
            }
            //QPen pen(Qt::black, 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
            QPen pen(Qt::NoPen);
            graphicsScene.addRect(tile->getXPos(), tile->getYPos(), 1, 1, pen, QBrush(color));
        }

        graphicsView.scale(20, 20);

    } catch(const QString &err) {
        std::cerr << err.toStdString() << std::endl;
        return 2;
    }


    mainWindow.show();
    return a.exec();
}
