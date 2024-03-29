#ifndef WORLDCONTROLLER_H
#define WORLDCONTROLLER_H

#include "matrix.h"
#include <memory>
#include <world.h>
#include <QString>
#include <QGraphicsView>
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
    WorldController(QGraphicsView *view);
    void createWorld(QString world, int enemies, int healthpacks);
    void render();
    void moveProtagonist(int rows, int cols);
    void doPathfinderSteps(int xTarget, int yTarget, float heuristicsWeight);

    std::shared_ptr<WorldModel> getWorldModel();
    void playStrategy(float heuristicsWeight);
    void stopTimer();
    void updateAnimationSpeed(int speed) { animationSpeed = speed; }

    ActionTimer &getTimer();
private:
    int animationSpeed = 1;
    int range;
    QGraphicsView *view;
    std::shared_ptr<WorldModel> worldModel;
    ActionTimer actionTimer;
};

#endif // WORLDCONTROLLER_H
