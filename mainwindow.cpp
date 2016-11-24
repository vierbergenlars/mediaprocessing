#include "mainwindow.h"
#include <QGraphicsScene>
#include <QKeyEvent>

MainWindow::MainWindow(WorldController &controller, QWidget *parent) :
    QMainWindow(parent), mainView(this), controller(controller)
{
    this->setCentralWidget(&mainView);
    mainView.show();
}

void MainWindow::createWorld(QString file)
{
    controller.createWorld(file);
    QGraphicsScene* scene = new QGraphicsScene(&mainView);
    controller.render(*scene);
    mainView.setScene(scene);
    mainView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    default:
        return;
        break;
    case Qt::Key_Left:
    case Qt::Key_4:
        controller.moveProtagonist(-1, 0);
        break;
    case Qt::Key_Right:
    case Qt::Key_6:
        controller.moveProtagonist(1, 0);
        break;
    case Qt::Key_Up:
    case Qt::Key_8:
        controller.moveProtagonist(0, -1);
        break;
    case Qt::Key_Down:
    case Qt::Key_2:
        controller.moveProtagonist(0, 1);
        break;
    case Qt::Key_Plus:
        controller.scale/=2;
        controller.range*=4;
        break;
    case Qt::Key_Minus:
        controller.scale*=2;
        controller.range/=4;
    }

    QGraphicsScene* prevScene = mainView.scene();
    QGraphicsScene* scene = new QGraphicsScene(&mainView);
    controller.render(*scene);
    mainView.setScene(scene);
    delete prevScene;
}
