#include "Particle.h"
#include <math.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

Particle::Particle(){

}
Particle::Particle(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed, int _id)
{

    mass = Mass;

    x = _x;

    y = _y;

    vx = _vx;

    vy = _vy;

    id = _id;

    size = sqrt(mass/3.14);

    fix = fixed;

    col = false;

    if (mass < 100){//select color semi-randomly based on size
        color = Qt::white;
    }else if(mass <1500){
        int sel = rand()%5;
        if(sel == 0){
            color = Qt::cyan;
        }else if(sel == 1){
            color = Qt::darkRed;
        }else if(sel == 2){
            color = Qt::green;
        }else if (sel == 3){
            color = Qt::red;
        }else{
            color = Qt::gray;
        }
    }else{

         color = Qt::yellow;

    }
}

Player::Player(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed)
{
    mass = Mass;

    x = _x;

    y = _y;

    vx = _vx;

    vy = _vy;

    size = sqrt(mass/3.14);

    fix = fixed;
}


void Particle::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget){

    painter->setBrush(QBrush(color));

    if(size<2){
        painter->drawEllipse(x - size/2 , y - size/2 , 2, 2);

    }else{
        painter->drawEllipse(x - size/2 , y - size/2 , size + 1 , size + 1);
    }

}
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget){

    painter->drawRect(x - size/2, y - size/2, size, size);

}


void Particle::DrawPartical(long double _x, long double _y, QGraphicsScene *scene){

}





int Particle::Getmass(){

    return(mass);

}
void  Particle::setmass(int _mass){
    mass = _mass;
    size = sqrt(mass/3.14);
}
QRectF Particle::boundingRect() const
{
    return QRectF(x, y,size,size);
}
void DeleteParticle(Particle * p){

}
