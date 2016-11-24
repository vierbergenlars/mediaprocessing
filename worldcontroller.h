#ifndef WORLDCONTROLLER_H
#define WORLDCONTROLLER_H

#include "matrix.h"
#include <memory>
#include <world.h>
#include <QString>
#include <QGraphicsScene>

typedef struct PStruct {
    std::shared_ptr<Tile> tile;
    std::shared_ptr<Enemy> enemy;
    std::shared_ptr<Tile> healthpack;
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
private:
    Matrix<PStruct> *tiles;
    std::shared_ptr<Protagonist> protagonist;
    std::unique_ptr<Matrix<PStruct>> getTilesAroundProtagonist();
};

#endif // WORLDCONTROLLER_H
