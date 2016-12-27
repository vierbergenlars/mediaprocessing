#ifndef STRATEGY_H
#define STRATEGY_H

#include <matrix.h>
#include<pathfinder.h>
#include<worldmodel.h>
#include<vector>
#include<pathfinder.h>


class Strategy
{
public:
    Strategy(WorldModel *worldModel);


    bool doNextStep();

private:
    WorldModel* _worldModel;
    std::vector<std::shared_ptr<Enemy>> enemys;
    std::vector<std::shared_ptr<WorldTile>> healtTiles;
    std::deque<Node> stepQue;


    std::vector<std::shared_ptr<WorldTile> > findDefeatableEnemies();
    std::deque<Node> findClosestPath(std::vector<std::shared_ptr<WorldTile> > tileList);

};

#endif // STRATEGY_H
