#include <iostream>
#include <world.h>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QMainWindow>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow mainWindow;

    QGraphicsScene graphicsScene;
    QGraphicsView graphicsView(&graphicsScene, &mainWindow);

    mainWindow.setCentralWidget(&graphicsView);

    if(argc < 2) {
        std::cerr << "Pass the world file to use as first parameter" << std::endl;
        return 1;
    }
    World *w = new World();

    try {
        std::vector<std::unique_ptr<Tile>> tiles = w->createWorld(argv[1]);

        for(std::unique_ptr<Tile> &tile: tiles) {
            QColor color;
            if(isinf(tile->getValue())) {
                // Use red for impassable areas
                color = QColor(0xff, 0, 0);
            } else {
                int colorValue(tile->getValue()*0xff);
                color = QColor(colorValue, colorValue, colorValue);
            }
            //QPen pen(Qt::black, 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
            QPen pen(Qt::NoPen);
            graphicsScene.addRect(tile->getXPos(), tile->getYPos(), 1, 1, pen, QBrush(color));
        }


        std::vector<std::unique_ptr<Enemy>> enemies = w->getEnemies(8);

        for(std::unique_ptr<Enemy> &enemy: enemies) {
            // Create a shared_ptr to Enemy to be able to cast it.
            // It has to be released from the unique_ptr, or the deleter will run immediately
            std::shared_ptr<Enemy> shared_enemy = std::shared_ptr<Enemy>(enemy.release(), enemy.get_deleter());
            QColor color(0, 0, 0xff);
            QPen pen(Qt::NoPen);

            // This cast will only succeed when the object in the sared_ptr is an instance of PEnemy
            // Else it will return nullptr
            std::shared_ptr<PEnemy> penemy = std::dynamic_pointer_cast<PEnemy>(shared_enemy);
            if(penemy != nullptr) {
                // Special color for PEnemies
                color = QColor(0, 0xff, 0xff);
            }
            auto enemyEllipse = graphicsScene.addEllipse(shared_enemy->getXPos(), shared_enemy->getYPos(), 1, 1, pen, QBrush(color));
            QString enemyTextValue;
            if(penemy == nullptr) {
                enemyTextValue = QString::number(shared_enemy->getValue());
            } else {
                enemyTextValue = QString::number(penemy->getValue())+ " P: "+QString::number(penemy->getPoisonLevel());
            }
            QGraphicsSimpleTextItem* enemyText = new QGraphicsSimpleTextItem(enemyTextValue, enemyEllipse);
            enemyText->setPos(enemyEllipse->boundingRect().center()); // Fix position of text to be around the enemy bounding box
            enemyText->setBrush(QBrush(Qt::white));
            enemyText->setPen(QPen(Qt::black, 0));

            // Draw a boundingbox around the text
            auto enemyTextRect = enemyText->boundingRect();
            enemyTextRect.moveTo(enemyText->pos());
            graphicsScene.addRect(enemyTextRect, QPen(Qt::black, 0));
        }

        graphicsView.scale(20, 20);

    } catch(const QString &err) {
        std::cerr << err.toStdString() << std::endl;
        return 2;
    }


    mainWindow.show();
    return a.exec();
}
