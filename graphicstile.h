#ifndef GRAPHICSTILE_H
#define GRAPHICSTILE_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <world.h>

class GraphicsComponent: public QGraphicsItem
{
public:
    GraphicsComponent(const std::shared_ptr<const Tile> & tile, QGraphicsItem *parent = nullptr);
    virtual QRectF boundingRect() const override;
    void showBoundingBox();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
protected:
    virtual void doPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) = 0;
    QGraphicsRectItem boundingBox;
};

class GraphicsTile: public GraphicsComponent
{
public:
    GraphicsTile(const std::shared_ptr<const Tile> &tile, QGraphicsItem *parent = nullptr);
    virtual void doPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
protected:
    QGraphicsRectItem tileRect;
private:
    std::shared_ptr<const Tile> tile;
};

class GraphicsEnemy: public GraphicsComponent
{
public:
    GraphicsEnemy(const std::shared_ptr<const Enemy> &enemy, QGraphicsItem *parent = nullptr);
    virtual void doPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
private:
    QGraphicsEllipseItem enemyEllipse;
    QGraphicsSimpleTextItem enemyText;
    std::shared_ptr<const Enemy> enemy;
};

class GraphicsHealthpack: public GraphicsComponent
{
public:
    GraphicsHealthpack(const std::shared_ptr<const Tile> &tile, QGraphicsItem * parent = nullptr);
    virtual void doPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
private:
    QGraphicsEllipseItem hpEllipse;
    QGraphicsSimpleTextItem hpText;
    std::shared_ptr<const Tile> tile;
};

class GraphicsProtagonist: public GraphicsComponent
{
public:
    GraphicsProtagonist(const std::shared_ptr<const Protagonist> & protagonist, QGraphicsItem* parent = nullptr);
    virtual void doPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
private:
    std::shared_ptr<const Protagonist> protagonist;
};

#endif // GRAPHICSTILE_H
