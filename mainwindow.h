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
#include <QMouseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow() {}
};

class GraphicsView: public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr): QGraphicsView(parent) {}
    virtual ~GraphicsView() = default;
signals:
    void tilePressed(int x, int y);
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

class MainWindowCentralWidget: public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowCentralWidget(QWidget *parent = 0);
    void createWorld(QString file, int enemies, int healthpacks);
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual ~MainWindowCentralWidget() = default;
public slots:
    void runStrategy();
    void stopAction();
    void runPathfinder(int targetX, int targetY);
private slots:
    void setHeuristicsWeight(double weight) { heuristicsWeight = weight; }
signals:
    void worldLoaded(int rows, int cols);
    void healthUpdated(int health);
    void energyUpdated(int energy);
private:
    GraphicsView *graphicsView;
    std::shared_ptr<WorldController> controller;
    float heuristicsWeight = 1.f;
};

class CoordinateInputDialog: public QDialog
{
    Q_OBJECT
public:
    CoordinateInputDialog(int maxX, int maxY, QWidget *parent = nullptr);
    virtual ~CoordinateInputDialog() {}
    void setMaxX(int x) { xPos->setMaximum(x);}
    void setMaxY(int y) { yPos->setMaximum(y); }
    int getXPos() { return xPos->value(); }
    int getYPos() { return yPos->value(); }
public slots:
    void setMaxDims(int rows, int cols) { this->setMaxX(cols-1); this->setMaxY(rows-1); }
private:
    QSpinBox *xPos;
    QSpinBox *yPos;
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
