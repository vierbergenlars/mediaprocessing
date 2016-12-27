#include "graphicstile.h"

#include <cmath>
#include <iostream>
#include "mypenemy.h"

GraphicsProtagonist::GraphicsProtagonist(QGraphicsItem *parent):
    QGraphicsItem(parent), boundingBox(this)
{
    boundingBox.setRect(0, 0, 1, 1);
    boundingBox.setPen(QPen(Qt::magenta, 0));
    boundingBox.setBrush(Qt::NoBrush);
}

QRectF GraphicsProtagonist::boundingRect() const
{
    return boundingBox.boundingRect();
}

void GraphicsProtagonist::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}


GraphicsPosition::GraphicsPosition(const std::shared_ptr<WorldTile> tile, QGraphicsItem *parent)
    :QGraphicsItem(parent), tile(tile)
{
    statusRect = new QGraphicsRectItem(this);
    statusRect->setRect(0,0,1,1);
    statusRect->setZValue(3);
    statusRect->setPen(Qt::NoPen);

    poisonRect = new QGraphicsRectItem(this);
    poisonRect->setRect(0,0,1,1);
    poisonRect->setPen(Qt::NoPen);
    poisonRect->setZValue(2);


    itemEllipse = new QGraphicsEllipseItem(this);
    itemEllipse->setRect(0, 0, 1, 1);
    itemEllipse->setPen(Qt::NoPen);
    itemEllipse->setZValue(4);

    itemText = new QGraphicsSimpleTextItem(this);
    itemText->setBrush(Qt::white);
    itemText->setPen(QPen(Qt::black, 0));
    itemText->setScale(0.1);
    itemText->setZValue(5);

    connect(&*tile, &WorldTile::changed, this, &GraphicsPosition::update);

    std::shared_ptr<PEnemy> penemy = std::dynamic_pointer_cast<PEnemy>(tile->enemy());

    if(penemy != nullptr) {
        connect(&*penemy, &PEnemy::dead, this, &GraphicsPosition::update);
        connect(&*penemy, SIGNAL(poisonLevelUpdated(int)), this, SLOT(update()));
    }
    update();
}

QRectF GraphicsPosition::boundingRect() const
{
    return QRectF(0,0,1,1);

}

void GraphicsPosition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void GraphicsPosition::update()
{
    switch(tile->status()) {
    case WorldTile::Status::openlist:
        statusRect->setOpacity(0.2);
        statusRect->setBrush(QBrush(Qt::darkYellow));
        break;
    case WorldTile::Status::closedlist:
        statusRect->setOpacity(0.2);
        statusRect->setBrush(QBrush(Qt::blue));
        break;
    case WorldTile::Status::solution:
        statusRect->setOpacity(0.5);
        statusRect->setBrush(QBrush(Qt::green));
        break;
    default:
        statusRect->setBrush(QBrush(Qt::transparent));
    }

    std::shared_ptr<const Enemy> enemy = tile->enemy();

    float healthEffect = tile->getHealthEffect();
    if(healthEffect > 0) {
        itemEllipse->setBrush(QBrush(Qt::green));
        itemText->setText(QString::number(tile->getHealthpack()));
    } else if(enemy != nullptr) {
        std::shared_ptr<const PEnemy> penemy = std::dynamic_pointer_cast<const PEnemy>(enemy);
        if(penemy != nullptr) {
            itemEllipse->setBrush(QBrush(QColor(0xff, 0, 0xff)));
            itemText->setText(QString::number(penemy->getValue())+ " P: "+QString::number(penemy->getPoisonLevel()));
        } else {
            itemEllipse->setBrush(QBrush(Qt::red));
            itemText->setText(QString::number(enemy->getValue()));
        }
        if(enemy->getDefeated()) {
            itemEllipse->setBrush(QBrush(Qt::gray));
            itemText->setText(itemText->text()+ " (dead)");
        }
    } else {
        itemEllipse->setBrush(QBrush(Qt::transparent));
        itemText->setText("");
    }

    float poisonEffect = tile->getPoisonEffect();
    if(poisonEffect > 0) {
        poisonRect->setBrush(QBrush(QColor(0xff, 0, 0xff)));
        poisonRect->setOpacity(poisonEffect/100);
    } else {
        poisonRect->setBrush(Qt::transparent);
    }
}

void GraphicsPosition::updateScale(float scale)
{
    this->setScale(scale);
    this->setPos(tile->getX()*scale, tile->getY()*scale);
}
