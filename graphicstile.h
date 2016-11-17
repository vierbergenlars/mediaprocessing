#ifndef GRAPHICSTILE_H
#define GRAPHICSTILE_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <world.h>

class GraphicsTile: public QGraphicsItem
{
public:
    GraphicsTile(std::shared_ptr<const Tile> &tile, QGraphicsItem *parent = nullptr);
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
private:
    std::shared_ptr<const Tile> tile;
    QGraphicsRectItem graphicsRect;
};

#endif // GRAPHICSTILE_H
