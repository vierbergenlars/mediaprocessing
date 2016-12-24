#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>
#include "worldcontroller.h"
#include <qprogressbar.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void createWorld(QString file);
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual ~MainWindow();
private:
    WorldController *controller;
    QGraphicsView mainView;
    QProgressBar *energyBar;
    QProgressBar *healthBar;
};

#endif // MAINWINDOW_H
