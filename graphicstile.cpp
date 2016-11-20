#include "graphicstile.h"

#include <cmath>
#include <iostream>

GraphicsComponent::GraphicsComponent(const std::shared_ptr<const Tile> & tile, QGraphicsItem *parent):
    QGraphicsItem(parent), boundingBox(this)
{
    boundingBox.setRect(0, 0, 1, 1);
    boundingBox.setPen(Qt::NoPen);
    boundingBox.setBrush(Qt::NoBrush);
}

QRectF GraphicsComponent::boundingRect() const
{
    return boundingBox.boundingRect();
}

void GraphicsComponent::showBoundingBox()
{
    boundingBox.setPen(QPen(Qt::magenta, 0));
}

void GraphicsComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    this->doPaint(painter, option, widget);
    boundingBox.paint(painter, option, widget);
}


GraphicsTile::GraphicsTile(const std::shared_ptr<const Tile> &tile, QGraphicsItem * parent):
    GraphicsComponent(tile, parent), tileRect(this), tile(tile)
{
    tileRect.setRect(0, 0, 1, 1);
    tileRect.setPen(Qt::NoPen);
    QColor color;
    if(std::isinf(tile->getValue())) {
        color = QColor(Qt::red);
    } else {
        int colorValue(tile->getValue()*0xff);
        color = QColor(colorValue, colorValue, colorValue);
    }
    tileRect.setBrush(QBrush(color));
}


void GraphicsTile::doPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    tileRect.paint(painter, option, widget);
}


GraphicsEnemy::GraphicsEnemy(const std::shared_ptr<const Enemy> &enemy, QGraphicsItem * parent):
    GraphicsComponent(enemy, parent), enemyEllipse(this), enemyText(), enemy(enemy)
{
    enemyEllipse.setRect(0, 0, 1, 1);
    enemyText.setScale(0.1);
    enemyText.setBrush(Qt::white);
    enemyText.setPen(QPen(Qt::black, 0));
    enemyText.setParentItem(this);
    QColor color(0, 0xff, 0);

    // This cast will only succeed when the object in the shared_ptr is an instance of PEnemy
    // Else it will return nullptr
    std::shared_ptr<const PEnemy> penemy = std::dynamic_pointer_cast<const PEnemy>(enemy);
    if(penemy != nullptr) {
        // Special color for PEnemies
        color = QColor(0xff, 0, 0xff);
    }
    enemyEllipse.setBrush(QBrush(color));
    enemyEllipse.setPen(Qt::NoPen);
}

void GraphicsEnemy::doPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    std::shared_ptr<const PEnemy> penemy = std::dynamic_pointer_cast<const PEnemy>(enemy);
    if(penemy == nullptr) {
        enemyText.setText(QString::number(enemy->getValue()));
    } else {
        enemyText.setText(QString::number(penemy->getValue())+ " P: "+QString::number(penemy->getPoisonLevel()));
    }
    enemyEllipse.paint(painter, option, widget);
    enemyText.paint(painter, option, widget);
}

GraphicsHealthpack::GraphicsHealthpack(const std::shared_ptr<const Tile> &tile, QGraphicsItem *parent):
    GraphicsComponent(tile, parent), hpEllipse(this), hpText(), tile(tile)
{
    hpEllipse.setRect(0, 0, 1, 1);
    hpText.setScale(0.1);
    hpText.setBrush(Qt::white);
    hpText.setPen(QPen(Qt::black, 0));
    hpText.setParentItem(this);

    hpEllipse.setBrush(Qt::red);
    hpEllipse.setPen(Qt::NoPen);
}


void GraphicsHealthpack::doPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    hpText.setText(QString::number(tile->getValue()));
    hpText.paint(painter, option, widget);
    hpEllipse.paint(painter, option, widget);
}

GraphicsProtagonist::GraphicsProtagonist(const std::shared_ptr<const Protagonist> &protagonist, QGraphicsItem *parent):
    GraphicsComponent(protagonist, parent), protagonist(protagonist)
{
    this->showBoundingBox();
}

void GraphicsProtagonist::doPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}
