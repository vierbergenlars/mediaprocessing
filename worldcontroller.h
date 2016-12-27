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
class WorldController
{
public:
    WorldController(QGraphicsScene*scene);
    void createWorld(QString world, int enemies, int healthpacks);
    void render();
    void moveProtagonist(int rows, int cols);
    bool debugMode = false;
    void doPathfinderSteps(int xTarget, int yTarget, int timerLength = 100);

    std::shared_ptr<WorldModel> getWorldModel();
    float getProtagonistEnergy();
    float getProtagonistHealth();
    void playStrategy();
    void stopTimer();
    void updateScale(float scaleDiff);

private:
    int scale;
    int range;
    QGraphicsPixmapItem *backgroundImage;
    QGraphicsScene *scene;
    std::shared_ptr<WorldModel> worldModel;
    std::vector<GraphicsPosition*> positions;
    GraphicsProtagonist* gprotagonist;
    ActionTimer actionTimer;
};

#endif // WORLDCONTROLLER_H
