#include "strategy.h"
#include <QDebug>

Strategy::Strategy(WorldModel* worldModel)
    :_worldModel(worldModel)
{


}

void Strategy::getNextStep()
{
    if(stepQue.empty()){ //que empty = zoek nieuwe destinatie

     std::deque<Node> tempQue =findClosestEnemyPath();
     if(!tempQue.empty()){
         stepQue = tempQue;
     }
     else{ // als geen enemies, spel gewonnen, niet genoeg health om enemies te verslaan?, of geen energy?
        stepQue =findClosestHealtpackPath();
     }

     if(stepQue.empty()){
          qDebug() << "No moves left";
          return;
     }
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
        float minDistance= std::numeric_limits<float>::infinity();
        std::deque<Node> tempQue;
        std::deque<Node> resultQue;
        if(healtTiles.empty()){
            tempQue.clear();
            return tempQue; // geen healthpacks meer over?
        }
        for(std::shared_ptr<WorldTile> tile :healtTiles){
            PathFinder path = PathFinder(x,y,tile->getX(), tile->getY(), _worldModel->tiles());
            path.AStarInit();
            tempQue = path.RunAStar();
            float cost = tempQue.back().finalCost;

            if(minDistance >cost){
                minDistance = cost;
                resultQue = tempQue;
            }
        }
        if(minDistance > _worldModel->protagonist()->getEnergy()){
            // geen packs te bereiken
            tempQue.clear();
            return tempQue;
        }
        return resultQue;

}

    std::deque<Node> Strategy::findClosestEnemyPath(){
        std::vector<std::shared_ptr<WorldTile> > enemies = findDefeatableEnemies();
        int x = _worldModel->protagonist()->getXPos();
        int y= _worldModel->protagonist()->getYPos();

        float minDistance= std::numeric_limits<float>::infinity();

        std::deque<Node> tempQue;
        std::deque<Node> resultQue;
        std::shared_ptr<WorldTile> closestEnemy;
        if(enemies.empty()){ // spel gewonnen? niet genoeg health om enemies te verslaan?
            tempQue.clear();
            return tempQue;
        }
        for(std::shared_ptr<WorldTile> tile :enemies){
            PathFinder path = PathFinder(x,y,tile->getX(), tile->getY(), _worldModel->tiles());
            path.AStarInit();
            tempQue = path.RunAStar();
            float cost = tempQue.back().finalCost;

            if(minDistance >cost) {
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
