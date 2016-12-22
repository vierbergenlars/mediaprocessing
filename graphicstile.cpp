#include "graphicstile.h"

#include <cmath>
#include <iostream>

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
    statusRect->setZValue(4);
    statusRect->setPen(Qt::NoPen);

    itemEllipse = new QGraphicsEllipseItem(this);
    itemEllipse->setRect(0, 0, 1, 1);
    itemEllipse->setPen(Qt::NoPen);

    itemText = new QGraphicsSimpleTextItem(this);
    itemText->setBrush(Qt::white);
    itemText->setPen(QPen(Qt::black, 0));
    itemText->setScale(0.1);
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


    float healthEffect = tile->getHealthEffect();
    if(healthEffect > 0) {
        itemEllipse->setBrush(QBrush(Qt::green));
        itemText->setText(QString::number(healthEffect));
    } else if(healthEffect < 0) {
        std::shared_ptr<const Enemy> enemy = tile->enemy();
        std::shared_ptr<const PEnemy> penemy = std::dynamic_pointer_cast<const PEnemy>(enemy);
        if(penemy != nullptr) {
            itemEllipse->setBrush(QBrush(QColor(0xff, 0, 0xff)));
            itemText->setText(QString::number(penemy->getValue())+ " P: "+QString::number(penemy->getPoisonLevel()));
        } else {
            itemEllipse->setBrush(QBrush(QColor(0, 0xff, 0)));
            itemText->setText(QString::number(enemy->getValue()));
        }
    } else {
        itemEllipse->setBrush(QBrush(Qt::transparent));
        itemText->setText("");
    }
}

void GraphicsPosition::updateScale(float scale)
{
    this->setScale(scale);
    this->setPos(tile->getX()*scale, tile->getY()*scale);
}
