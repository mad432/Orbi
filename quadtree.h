#ifndef QUADTREE_H
#define QUADTREE_H
#include <iostream>
#include "Particle.h"
#include <vector>
#include <mutex>

#include <atomic>
//used for barnes-hut algorithm
class QuadTree: public QObject, public QGraphicsItem
{
    public:

        std::mutex mymutex;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
        void DrawQuadTree(long double _x, long double _y, QGraphicsScene *scene);

        QRectF boundingRect() const;

        void get_actors(Particle * par, std::vector <Particle*>* ret);
        //bool distance(Particle * par);

        std::vector <Particle*>* pars = new std::vector <Particle*>;
        void print();
        //int particles;
        QuadTree();
        ~QuadTree();

        void clear();
        void constructnode(Particle * par);

        static int count;
    private:

        double min_dist = 0;


        double mass = 0;
        double x = 0;//center of mass
        double y = 0;
        Particle *here;

        QuadTree * upleft;
        QuadTree * upright;
        QuadTree * downleft;
        QuadTree * downright;


        std::atomic <double> radius;
        std::atomic <double> x0;
        std::atomic <double> y0;
        std::atomic <double> x1;
        std::atomic <double> y1;

        double dist(double _x, double _y);
};

#endif // QUADTREE_H
