#include "rocket.h"
#include <iostream>


void Rocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget){

    //painter->setBrush(QBrush(color));

    QPixmap img(":Image/Rocket.png");

    QPixmap plume(":Image/Plume.png");

    QTransform tr;
    tr.rotate(heading);
    img = img.transformed(tr);
    plume = plume.transformed(tr);

    painter->drawPixmap(x - size/2 , y - size/2 , size , size, img);

    painter->setBrush(QBrush(Qt::blue));

    painter->drawRect(x - size /** ((2 * fuel)/(mass * 3/4))*/, y + size , size * (2 * fuel)/(mass * 3/4),3);

    if(thrusting){// keeps the plue displayed while thrusting

        painter->drawPixmap(x - size/2 - sin(heading*(3.14/180))*size/3 , y - size/2 + cos(heading*(3.14/180))*size/3 , size , size, plume);

        if(cycle == 100){
            thrusting = false;
            cycle = 0;
        }else{
            cycle++;
        }
    }

}

Rocket::Rocket(int Mass, long double _x, long double _y, long double _vx, long double _vy, bool fixed, int _id)
{
    mass = Mass;

    x = _x;

    y = _y;

    vx = _vx;

    vy = _vy;

    id = _id;

    size = sqrt(mass/3.14);

    fix = false;

    col = false;

    color = Qt::white;

    heading = 0;

    fuel = mass * 3/4;

    thrusting = false;

    cycle = 0;
}

void Rocket::changeheading(int change){

    heading+=change;

};

void Rocket::thrust(int n){
    //fires the engines in the heading

    if(fuel > 0){

        double EV = 500;

        double masseval = mass; // makes mass a double for dv calculation

        double dv = EV*log((masseval) / (masseval - 5));//dv rocket equation

        vy -= cos(heading*(3.14/180)) * dv;

        vx += sin(heading*(3.14/180)) * dv;

        fuel -=5;

        mass -=5;

        thrusting = true;

        cycle = 0;

    }

    //std::cout<<fuel<<std::endl;

};


void Rocket::DrawPartical(long double _x, long double _y, QGraphicsScene *scene){

}

Rocket::~Rocket(){

}
