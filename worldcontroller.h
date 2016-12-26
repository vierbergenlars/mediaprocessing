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
    bool doPathfinderStep();
    void doPathfinder();

    float getProtagonistEnergy();
    float getProtagonistHealth();
private:
    QGraphicsPixmapItem *backgroundImage;
    PathFinder *path;
    QGraphicsScene *scene;
    WorldModel *worldModel;
    std::vector<GraphicsPosition*> positions;
    GraphicsProtagonist* gprotagonist;
    Strategy *strategy;
};

#endif // WORLDCONTROLLER_H
