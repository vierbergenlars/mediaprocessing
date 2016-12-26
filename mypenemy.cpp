#include "mypenemy.h"

MyPEnemy::MyPEnemy(int x, int y, float value)
    :PEnemy(x, y, value), prevPoisonLevel{value}
{
    timer.setInterval(100); // One second timer interval
    connect(&timer, SIGNAL(timeout()), this, SLOT(updatePoisonLevel()));
    connect(this, SIGNAL(dead()), &timer, SLOT(stop()));
}

bool MyPEnemy::poison()
{
    timer.start();
    emit poisoned(prevPoisonLevel);
    return PEnemy::poison();
}

void MyPEnemy::updatePoisonLevel()
{
    if(prevPoisonLevel != getPoisonLevel()) {
        emit poisoned(prevPoisonLevel);
        prevPoisonLevel = getPoisonLevel();
    }
}

