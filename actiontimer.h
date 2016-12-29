#ifndef ACTIONTIMER_H
#define ACTIONTIMER_H

#include <QTimer>
#include <QObject>
#include <functional>


class ActionTimer: public QObject
{
    Q_OBJECT
public:
    typedef std::function<bool()> BoolCallback;
    typedef std::function<void()> VoidCallback;
    ActionTimer();
    virtual ~ActionTimer();

    void connect(const BoolCallback callback);
    void connect(const BoolCallback callback, const VoidCallback destructor);
public slots:
    void disconnect();
signals:
    void activated(bool activated);

private:
    QTimer timer;
    VoidCallback _destructor;
};

#endif // ACTIONTIMER_H
