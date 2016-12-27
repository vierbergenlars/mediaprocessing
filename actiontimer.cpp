#include "actiontimer.h"
#include <QDebug>

void empty()
{

}

ActionTimer::BoolCallback callbackWrapper(const ActionTimer::VoidCallback callback)
{
    return [callback] () -> bool {
        callback();
        return true;
    };
}

ActionTimer::ActionTimer()
    : _destructor(&empty)
{
    timer.setInterval(100);
}

ActionTimer::~ActionTimer()
{
    this->disconnect();
}

void ActionTimer::connect(const BoolCallback callback)
{
    connect(callback, &empty);
}


void ActionTimer::connect(const BoolCallback callback, const VoidCallback destructor)
{
    disconnect();
    _destructor = destructor;
    QObject::connect(&timer, &QTimer::timeout, [this, callback] {
        if(!callback()) {
            this->disconnect();
        }
    });
    timer.start();
}


void ActionTimer::disconnect()
{
    timer.stop();
    timer.disconnect();
    _destructor();
    _destructor = &empty;
}

