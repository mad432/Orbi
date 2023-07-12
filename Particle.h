#ifndef PARTICAL_H
#define PARTICAL_H
#include <QObject>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsView>
#include "vector"


class Particle: public QObject, public QGraphicsItem{

    Q_OBJECT

public:

    Particle();

    Particle(int Mass,long double _x,long double _y ,long double _vx,long double _vy, bool fixed,int _id);

    void DrawPartical(long double _x,long double _y,QGraphicsScene* scene);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void Changemass(int set);

    int Getmass();

    void setvx(long double _vx){vx = _vx;}

    void setvy(long double _vy){vy = _vy;}

    void setx(long double _x){x = _x;}

    void sety(long double _y){y = _y;}

    void setmass(int _mass);

    void setid(int _id){id = _id;}

    void hascol(){col = true;}

    double getsize(){return size;}

    long double getx(){return x;}

    long double gety(){return y;}

    long double getvx(){return vx;}

    long double getvy(){return vy;}

    bool getcol(){return col;}

    int getid(){return id;}

    QRectF boundingRect() const override;

    bool getfix(){return fix;}

    void setcol(int num){colnum = num;}

    int getcolnum(){return colnum;}

signals:

    void DeleteParticle(Particle * p);

protected:

    int colnum = -1; //what other particle we collided with

    QColor color;

    int id;

    bool fix;

    int mass;

    double size;

    long double x;

    long double y;

    long double vx;

    long double vy;

    bool col = false;
};

class Player:public Particle{

public:

    Player(int Mass,long double _x,long double _y ,long double _vx,long double _vy, bool fixed);


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;


};

#endif // PARTICAL_H
