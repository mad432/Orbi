#ifndef ROCKET_H
#define ROCKET_H
#include "Particle.h"


class Rocket:public Particle {
public:

    std::string object() override {return "Rocket";};

    ~Rocket() override;

    Rocket(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed ,int _id);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void DrawParticle(long double _x, long double _y, QGraphicsScene *scene) override;

    void changeheading(double change) override;

    void thrust(double EV) override;

    int getFuel(){return fuel;};

    int getheading() override {return heading;};

    double getaV() override{return aV;};

    void changeaV(double i) override;

protected:

    double aV;

    int cycle;

    bool thrusting;

    int fuel;

    double heading;

    double Av;

    double I;


};

#endif // ROCKET_H
