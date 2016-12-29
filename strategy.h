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
    enum StepType { none, pathfind, follow };
    Strategy(WorldModel *worldModel);


    StepType doNextStep();

private:
    WorldModel* _worldModel;
    std::deque<Node> stepQue;


    std::vector<std::shared_ptr<WorldTile> > findDefeatableEnemies();
    std::deque<Node> findClosestPath(std::vector<std::shared_ptr<WorldTile> > tileList);

};

#endif // STRATEGY_H
