#ifndef QUADTREE_H
#define QUADTREE_H
#include <iostream>
#include "Particle.h"
#include <vector>

//#include <atomic>
//used for barnes-hut algorithm
class QuadTree: public QObject, public QGraphicsItem
{
    public:

    void QuadTree::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    void QuadTree::DrawQuadTree(long double _x, long double _y, QGraphicsScene *scene);

    QRectF QuadTree::boundingRect() const;


        double min_dist;

        void get_actors(Particle * par, std::vector <Particle*>* ret);
        //bool distance(Particle * par);

        std::vector <Particle*>* pars = new std::vector <Particle*>;
        void print();
        //int particles;
        QuadTree();

        double mass;
        double x;//center of mass
        double y;
        Particle *here;

        QuadTree * upleft;
        QuadTree * upright;
        QuadTree * downleft;
        QuadTree * downright;
        void clear();
        void constructnode(Particle * par);

        double radius;
        double x0;
        double y0;
        double x1;
        double y1;

        double dist(double _x, double _y);
};

#endif // QUADTREE_H
