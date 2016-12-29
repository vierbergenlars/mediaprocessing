#include "strategy.h"
#include <QDebug>

Strategy::Strategy(std::shared_ptr<WorldModel> worldModel, float heuristicsWeight)
    :_worldModel(worldModel), heuristicsWeight(heuristicsWeight)
{


}

Strategy::StepType Strategy::doNextStep()
{
    StepType stepType = follow;
    if(_worldModel->protagonist()->getHealth() <= 0)
        return none;
    if(stepQue.empty()){ //que empty = zoek nieuwe destinatie
        stepType = pathfind;
     std::vector<std::shared_ptr<WorldTile> > enemies = findDefeatableEnemies();
     std::deque<Node> tempQue =findClosestPath(enemies);
     if(!tempQue.empty()){
         stepQue = tempQue;
     }
     else{ // als geen enemies, spel gewonnen, niet genoeg health om enemies te verslaan?, of geen energy?
         std::vector<std::shared_ptr<WorldTile> > healtTiles = _worldModel->getHealtPackTiles();
         stepQue =findClosestPath(healtTiles);
     }


        if(stepQue.empty()){
            qDebug() << "No moves left";
            return none;
        }
    }

    Node nextPos = stepQue.front();
    stepQue.pop_front(); // remove
    int dX= nextPos.tile->getX() - _worldModel->protagonist()->getXPos();
    int dY = nextPos.tile->getY() - _worldModel->protagonist()->getYPos();
    _worldModel->moveProtagonist(dX,dY);
    return stepType;
}


    std::deque<Node> Strategy::findClosestPath(std::vector<std::shared_ptr<WorldTile> > tileList){
        std::shared_ptr<PathFinder> bestPath;

        int x = _worldModel->protagonist()->getXPos();
        int y= _worldModel->protagonist()->getYPos();

        float minDistance= std::numeric_limits<float>::infinity();

        std::deque<Node> tempQue;
        std::deque<Node> resultQue;

        if(tileList.empty()){ // spel gewonnen? niet genoeg health om enemies te verslaan?
            tempQue.clear();
            return tempQue;
        }
        for(std::shared_ptr<WorldTile> tile :tileList){

            auto path = std::make_shared<PathFinder>(x,y,tile->getX(), tile->getY(), _worldModel->tiles(), heuristicsWeight);
            tempQue = path->RunAStar();
            if(tempQue.empty())
                continue;
            float cost = tempQue.back().finalCost;

            if(minDistance >cost) {
                minDistance = cost;

                resultQue = tempQue;
                bestPath = path;
            }
        }
        if(minDistance > _worldModel->protagonist()->getEnergy()){
            // geen enemies te bereiken
            tempQue.clear();
            return tempQue;
        }
        bestPath->showVisuals();
        return resultQue;

    }

std::vector<std::shared_ptr<WorldTile> > Strategy::findDefeatableEnemies(){
    float health = _worldModel->protagonist()->getHealth();
    auto enemies = _worldModel->getEnemyTiles();
    std::vector<std::shared_ptr<WorldTile> > resultarray;
    for(std::shared_ptr<WorldTile> tile : enemies){
        if(!tile->enemy()->getDefeated() && -tile->getHealthEffect()< health){
            resultarray.push_back(tile);
        }
    }
    return resultarray;

}
