#include "strategy.h"
#include <QDebug>

Strategy::Strategy(std::shared_ptr<WorldModel> worldModel)
    :_worldModel(worldModel)
{


}

Strategy::Direction Strategy::getNextStep()
{
    if(stepQue.empty()){

     std::deque<Node> tempQue =findClosestEnemyPath();
     if(!tempQue.empty()){
         stepQue = tempQue;
     }
     else{
        stepQue =findClosestHealtpackPath();
     }

     if(stepQue.empty())
          qDebug() << "No moves left";
     else
         getNextStep();



    }

    Node nextPos = stepQue.front();
    stepQue.pop_front(); // remove
    int dX= nextPos.tile->getX() - _worldModel->protagonist()->getXPos();
    int dY = nextPos.tile->getY() - _worldModel->protagonist()->getYPos();
    _worldModel->moveProtagonist(dX,dY);




}
    std::deque<Node> Strategy::findClosestHealtpackPath(){

        std::vector<std::shared_ptr<WorldTile> > healtTiles = _worldModel->getHealtPackTiles();
        int x = _worldModel->protagonist()->getXPos();
        int y= _worldModel->protagonist()->getYPos();
        float minDistance= -1;
        std::deque<Node> tempQue;
        std::deque<Node> resultQue;
        if(healtTiles.empty()){
            tempQue.clear();
            return tempQue; // spel gewonnen? niet genoeg health om enemies te verslaan?
        }
        for(std::shared_ptr<WorldTile> tile :healtTiles){
            PathFinder path = PathFinder(x,y,tile->getX(), tile->getY(), _worldModel->tiles());
            path.AStarInit();
            tempQue = path.RunAStar();
            float cost = tempQue.front().finalCost;
            if(minDistance = -1){
                minDistance = cost;
                resultQue = tempQue;

            }
            if(minDistance >tempQue.front().finalCost){
                minDistance = cost;
                resultQue = tempQue;
            }
        }
        if(minDistance > _worldModel->protagonist()->getEnergy()){
            // geen enemies te bereiken
            tempQue.clear();
            return tempQue;
        }
        return resultQue;

}

    std::deque<Node> Strategy::findClosestEnemyPath(){
        std::vector<std::shared_ptr<WorldTile> > enemies = findDefeatableEnemies();
        int x = _worldModel->protagonist()->getXPos();
        int y= _worldModel->protagonist()->getYPos();

        float minDistance= -1;
        std::deque<Node> tempQue;
        std::deque<Node> resultQue;
        std::shared_ptr<WorldTile> closestEnemy;
        if(enemies.empty()){
            tempQue.clear();
            return tempQue; // spel gewonnen? niet genoeg health om enemies te verslaan?
        }
        for(std::shared_ptr<WorldTile> tile :enemies){
            PathFinder path = PathFinder(x,y,tile->getX(), tile->getY(), _worldModel->tiles());
            path.AStarInit();
            tempQue = path.RunAStar();
            float cost = tempQue.front().finalCost;
            if(minDistance = -1){
                minDistance = cost;
                closestEnemy = tile;
                resultQue = tempQue;

            }
            if(minDistance >tempQue.front().finalCost){
                minDistance = cost;
                closestEnemy = tile;
                resultQue = tempQue;
            }
        }
        if(minDistance > _worldModel->protagonist()->getEnergy()){
            // geen enemies te bereiken
            tempQue.clear();
            return tempQue;
        }
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
