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
    void createWorld(QString file, int enemies, int healthpacks);
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual ~MainWindow() {}
signals:
    void worldLoaded(bool);
    void actionRunning(bool);
private:
    std::shared_ptr<WorldController> controller;
    QProgressBar *energyBar;
    QProgressBar *healthBar;
};

class CoordinateInputDialog: public QDialog
{
public:
    CoordinateInputDialog(int maxX, int maxY, QWidget *parent = nullptr);
    virtual ~CoordinateInputDialog() {}
    void setMaxX(int x) { xPos->setMaximum(x);}
    void setMaxY(int y) { yPos->setMaximum(y); }
    int getXPos() { return xPos->value(); }
    int getYPos() { return yPos->value(); }
    int getAnimationSpeed() { return animationSpeed->value(); }
private:
    QSpinBox *xPos;
    QSpinBox *yPos;
    QSpinBox *animationSpeed;
};

class MapConfigInputDialog: public QDialog
{
public:
    MapConfigInputDialog(QWidget *parent = nullptr);
    virtual ~MapConfigInputDialog() {}
    int getEnemies() { return enemies->value(); }
    int getHealthpacks() { return healthpacks->value(); }
private:
    QSpinBox *enemies;
    QSpinBox *healthpacks;
};

#endif // MAINWINDOW_H
