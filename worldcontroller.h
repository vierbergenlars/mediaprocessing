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
class PathFinder;

class WorldController
{
public:
    WorldController(QGraphicsScene*scene);
    void createWorld(QString world);
    void render();
    void moveProtagonist(int rows, int cols);
    int range;
    int scale;
    bool debugMode = false;
    bool doPathfinderStep();
    void doPathfinder();
private:
    QGraphicsPixmapItem *backgroundImage;
    PathFinder *path;
    QGraphicsScene *scene;
    Matrix<std::shared_ptr<WorldTile>> *tiles;
    std::vector<GraphicsPosition*> positions;
    GraphicsProtagonist* gprotagonist;
    std::shared_ptr<Protagonist> protagonist;
    std::unique_ptr<Matrix<std::shared_ptr<WorldTile>>> getTilesAroundProtagonist();
};

#endif // WORLDCONTROLLER_H
