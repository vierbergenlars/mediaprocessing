#ifndef ACTIONTIMER_H
#define ACTIONTIMER_H

#include <QTimer>
#include <functional>

class ActionTimer
{
public:
    typedef std::function<bool()> BoolCallback;
    typedef std::function<void()> VoidCallback;
    ActionTimer();
    ~ActionTimer();

    void connect(const BoolCallback callback);
    void connect(const BoolCallback callback, const VoidCallback destructor);
    void disconnect();

private:
    QTimer timer;
    VoidCallback _destructor;
};

#endif // ACTIONTIMER_H
