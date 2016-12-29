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
    Strategy(std::shared_ptr<WorldModel> worldModel, float heuristicsWeight);


    StepType doNextStep();

private:
    float heuristicsWeight;
    std::shared_ptr<WorldModel> _worldModel;
    std::deque<Node> stepQue;


    std::vector<std::shared_ptr<WorldTile> > findDefeatableEnemies();
    std::deque<Node> findClosestPath(std::vector<std::shared_ptr<WorldTile> > tileList);

};

#endif // STRATEGY_H
