#ifndef GRAPHICSTILE_H
#define GRAPHICSTILE_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <worldtile.h>


class GraphicsPosition: public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    GraphicsPosition(const std::shared_ptr<WorldTile> tile, QGraphicsItem *parent=nullptr);
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    void updateScale(float scale);
public slots:
    void update();
private:
    const std::shared_ptr<WorldTile> tile;
    QGraphicsRectItem* statusRect;
    QGraphicsRectItem* poisonRect;
    QGraphicsEllipseItem* itemEllipse;
    QGraphicsSimpleTextItem* itemText;
};


class GraphicsProtagonist: public QGraphicsItem
{
public:
    GraphicsProtagonist(QGraphicsItem* parent = nullptr);
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
private:
    QGraphicsRectItem boundingBox;
};

#endif // GRAPHICSTILE_H
