#ifndef WORLDCONTROLLER_H
#define WORLDCONTROLLER_H

#include "matrix.h"
#include <memory>
#include <world.h>
#include <QString>
#include <QGraphicsScene>

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
    typedef Matrix<std::shared_ptr<Tile>> MTiles;
    typedef Matrix<std::shared_ptr<Enemy>> MEnemies;
    std::unique_ptr<MTiles> tiles;
    std::unique_ptr<MEnemies> enemies;
    std::unique_ptr<MTiles> healthpacks;
    std::shared_ptr<Protagonist> protagonist;
    template<class T>
    std::unique_ptr<T> getTilesAroundProtagonist(std::unique_ptr<T> &matrix);
};

#endif // WORLDCONTROLLER_H
