#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>
#include "worldcontroller.h"
#include <qprogressbar.h>
#include <QToolBar>
#include <QDialog>
#include <QLabel>
#include <QSpinBox>

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
    QToolBar *toolBar;
};

class CoordinateInputDialog: public QDialog
{
public:
    CoordinateInputDialog(int maxX, int maxY, QWidget *parent = nullptr);
    virtual ~CoordinateInputDialog() {};
    int getXPos() { return xPos->value(); }
    int getYPos() { return yPos->value(); }
    int getAnimationSpeed() { return animationSpeed->value(); }
private:
    QSpinBox *xPos;
    QSpinBox *yPos;
    QSpinBox *animationSpeed;
};

#endif // MAINWINDOW_H
