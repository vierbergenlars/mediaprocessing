#include "graphicstile.h"

#include <cmath>

GraphicsTile::GraphicsTile(std::shared_ptr<const Tile> &tile, QGraphicsItem * parent):
    QGraphicsItem(parent), graphicsRect(this), tile(tile)
{
    float scale=10;
    graphicsRect.setRect(tile->getXPos()*scale, tile->getYPos()*scale, scale, scale);
    graphicsRect.setPen(Qt::NoPen);
}

QRectF GraphicsTile::boundingRect() const
{
    return graphicsRect.boundingRect();
}

void GraphicsTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor color;
    if(std::isinf(tile->getValue())) {
        color = QColor(Qt::red);
    } else {
        int colorValue(tile->getValue()*0xff);
        color = QColor(colorValue, colorValue, colorValue);
    }
    QBrush brush(color);

    graphicsRect.setBrush(brush);

    graphicsRect.paint(painter, option, widget);
}



