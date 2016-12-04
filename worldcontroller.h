#ifndef WORLDCONTROLLER_H
#define WORLDCONTROLLER_H

#include "matrix.h"
#include <memory>
#include <world.h>
#include <QString>
#include <QGraphicsScene>
#include <deque>
class PathFinder;

enum Status {none, openlist, closedlist, solution};
typedef struct PStruct {
    std::shared_ptr<Tile> tile;
    std::shared_ptr<Enemy> enemy;
    std::shared_ptr<Tile> healthpack;

    Status pathStatus;
} PStruct;

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
    void doPathfinderStep();
    void doPathfinder();
private:
    PathFinder *path;
    Matrix<std::shared_ptr<PStruct>> *tiles;
    std::shared_ptr<Protagonist> protagonist;
    std::unique_ptr<Matrix<std::shared_ptr<PStruct>>> getTilesAroundProtagonist();
};

#endif // WORLDCONTROLLER_H
