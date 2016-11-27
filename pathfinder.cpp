#include "pathfinder.h"
#include <algorithm>

PathFinder::PathFinder(int xstart, int ystart, int xend, int yend, std::unique_ptr<Matrix<PStruct> > &matrix):
    _xstart(xstart), _ystart(ystart), _xend(xend), _yend(yend),_matrix(matrix)
{

}

std::deque<Node> PathFinder::Run()
{
    //step 1 Breadth-First
    Node startNode;
    startNode.parent = nullptr;
    startNode.score = 0;
    startNode.tile = _matrix->get(_xstart, _ystart).tile;

    Node currentNode;
    //step 2
    openList.push_back(startNode);

    //step 3
    while(!openList.empty()){
        //3.1 +3.2
        currentNode = openList.back();
        if (currentNode.tile->getXPos() == _xend && currentNode.tile->getYPos()== _yend){ // the current node is the solution node
            break;
        }

        // 3.3

        std::shared_ptr<Node> parent_ptr = std::make_shared<Node>(currentNode);
        for(int i= 0; i<=2;i++){ // ga alle posities rond current af
            for(int j=0; j <= 2; j++){
                int x = currentNode.tile->getXPos() +i-1;
                int y = currentNode.tile->getYPos() +j - 1;
                if(_matrix->contains(x,y)){ //positie in range of matrix
                    Node nearbyNode;
                    nearbyNode.tile = _matrix->get(x,y).tile;

                    bool found = std::find(openList.begin(), openList.end(), nearbyNode) != openList.end(); // if true, dan al in lijst
                    if(!found) found = std::find(closedList.begin(), closedList.end(), nearbyNode) != closedList.end();
                    if(!std::isinf(nearbyNode.tile->getValue()) && !found){ //  if not inpasssable and not already in Lists
                        nearbyNode.parent= parent_ptr; // set parrent pointer to current node
                        openList.push_front(nearbyNode); //add to open list
                    }
                }
            }

        }
        openList.pop_back();//delete currentNode from open
        closedList.push_back(currentNode); //add current node to closed
    }

// sollution should be found now
    if (currentNode.tile->getXPos() == _xend && currentNode.tile->getYPos()== _yend){ // oplossing gevonden
        while(currentNode.parent != nullptr){
            resultList.push_front(currentNode);
            currentNode = *(currentNode.parent);
        }
    } //else geen oplossing, resultList is leeg
return resultList;
}
