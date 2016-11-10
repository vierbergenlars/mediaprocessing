#include <iostream>
#include <world.h>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 2) {
        std::cerr << "Pass the world file to use as first parameter" << std::endl;
        return 1;
    }
    World *w = new World();
    try {
        auto tiles = w->createWorld(argv[1]);
    } catch(const QString &err) {
        std::cerr << err.toStdString() << std::endl;
        return 2;
    }

}
