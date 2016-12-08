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
class PathFinder;

class WorldController
{
public:
    WorldController();
    void createWorld(QString world);
    void render(QGraphicsScene &scene);
    void moveProtagonist(int rows, int cols);
    int range;
    int scale;
    bool debugMode = false;
    bool doPathfinderStep();
    void doPathfinder();
private:
    QGraphicsPixmapItem *backgroundImage;
    PathFinder *path;
    Matrix<WorldTile> *tiles;
    std::shared_ptr<Protagonist> protagonist;
    std::unique_ptr<Matrix<WorldTile>> getTilesAroundProtagonist();
};

#endif // WORLDCONTROLLER_H
