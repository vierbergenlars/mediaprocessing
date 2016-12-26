#ifndef MYPENEMY_H
#define MYPENEMY_H

#include <QObject>
#include <QTimer>
#include "world.h"

class MyPEnemy : public PEnemy
{
    Q_OBJECT
public:
    MyPEnemy(std::shared_ptr<PEnemy> penemy): MyPEnemy(penemy->getXPos(), penemy->getYPos(), penemy->getValue()) {}
    MyPEnemy(const PEnemy& penemy): MyPEnemy(penemy.getXPos(), penemy.getYPos(), penemy.getValue()) {}
    MyPEnemy(int x, int y, float value);
    virtual ~MyPEnemy() {
        timer.stop();
    }

public slots:
    bool poison();
signals:
    void poisoned(float level);
private slots:
    void updatePoisonLevel();
private:
    QTimer timer;
    float prevPoisonLevel;
};

#endif // MYPENEMY_H
