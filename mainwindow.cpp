#include "mainwindow.h"
#include <QGraphicsScene>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), mainView(this)
{
    this->setCentralWidget(&mainView);
    mainView.show();
    QGraphicsScene* scene = new QGraphicsScene(&mainView);
    controller = new WorldController(scene);
    mainView.setScene(scene);
}

void MainWindow::createWorld(QString file)
{
    controller->createWorld(file);
    controller->render();
    //mainView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //mainView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    default:
        return;
        break;
    case Qt::Key_Left:
    case Qt::Key_4:
        controller->moveProtagonist(-1, 0);
        break;
    case Qt::Key_Right:
    case Qt::Key_6:
        controller->moveProtagonist(1, 0);
        break;
    case Qt::Key_Up:
    case Qt::Key_8:
        controller->moveProtagonist(0, -1);
        break;
    case Qt::Key_Down:
    case Qt::Key_2:
        controller->moveProtagonist(0, 1);
        break;
    case Qt::Key_Plus:
        controller->scale/=2;
        controller->range*=4;
        break;
    case Qt::Key_Minus:
        controller->scale*=2;
        controller->range/=4;
        break;
    case Qt::Key_Enter:
        controller->doPathfinderStep();
        break;
    case Qt::Key_A:
        for(int i=0; i<40; i++)
            if(controller->doPathfinderStep())
                break;
        break;
    case Qt::Key_B:
        for(int i=0; i<400; i++)
            if(controller->doPathfinderStep())
                break;
        break;
    case Qt::Key_C:
        for(int i=0; i<4000; i++)
            if(controller->doPathfinderStep())
                break;
        break;
    case Qt::Key_D:
        for(int i=0; i<40000; i++)
            if(controller->doPathfinderStep())
                break;
        break;
    case Qt::Key_E:
        controller->doPathfinder();
        break;
    case Qt::Key_F:
        controller->debugMode=!controller->debugMode;
    }

    controller->render();
}
