#ifndef QUADTREE_H
#define QUADTREE_H
#include <iostream>
#include "Particle.h"
#include <vector>
//#include <atomic>
//used for barnes-hut algorithm
class QuadTree
{
    public:

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

        double dist(double _x, double _y);
};

#endif // QUADTREE_H
