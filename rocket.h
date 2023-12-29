#ifndef ROCKET_H
#define ROCKET_H
#include "Particle.h"


class Rocket:public Particle {
public:

    ~Rocket() override;

    Rocket(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed ,int _id);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void DrawPartical(long double _x, long double _y, QGraphicsScene *scene) override;

    void changeheading(int change);

    void thrust(int n);

protected:

    int cycle;

    bool thrusting;

    int fuel;

    int heading;


};

#endif // ROCKET_H
