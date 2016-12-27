#ifndef WORLDCONTROLLER_H
#define WORLDCONTROLLER_H

#include "matrix.h"
#include <memory>
#include <world.h>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <deque>
#include "worldtile.h"
#include "graphicstile.h"
#include "worldmodel.h"
#include "strategy.h"
#include <QTimer>
#include "actiontimer.h"
class PathFinder;

class WorldController
{
public:
    WorldController(QGraphicsScene*scene);
    ~WorldController();
    void createWorld(QString world);
    void render();
    void moveProtagonist(int rows, int cols);
    int range;
    int scale;
    bool debugMode = false;
    void doPathfinderSteps(int xTarget, int yTarget, int timerLength = 100);
    bool doPathfinderStep();
    void doPathfinder();

    float getProtagonistEnergy();
    float getProtagonistHealth();
    void playStrategy();
    void stopTimer();
private:
    QGraphicsPixmapItem *backgroundImage;
    PathFinder *path;
    QGraphicsScene *scene;
    WorldModel *worldModel;
    std::vector<GraphicsPosition*> positions;
    GraphicsProtagonist* gprotagonist;
    ActionTimer actionTimer;
};

#endif // WORLDCONTROLLER_H
