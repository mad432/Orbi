#include "quadtree.h"

QuadTree::QuadTree()
{
    upleft = nullptr;
    upright = nullptr;
    downleft = nullptr;
    downright = nullptr;
    x = 0;
    y = 0;
    mass = 0;
    radius = 0;
    here = new Particle(0,0,0,0,0,0,9999999);
    //here->setcol(-10);
    pars = new std::vector <Particle*>;
    min_dist = 100;

}
void QuadTree::clear() {
    //clears the tree
    if (downright != nullptr) {
        downright->clear();
        delete downright;
        downright = nullptr;
    }
    if (upright != nullptr) {
        upright->clear();
        delete upright;
        upright = nullptr;
    }
    if (downleft != nullptr) {
        downleft->clear();
        delete downleft;
        downleft = nullptr;
    }
    if (upleft != nullptr) {
        upleft->clear();
        delete upleft;
        upleft = nullptr;
    }
    mass = 0;
    pars->clear();
}

void QuadTree::print(){
    //prints the tree
    std::cout<<radius<<","<<radius + min_dist<<"  size :"<<pars->size()<<" :: ";

    if(downright != nullptr){
        downright->print();
    }

    if(upright != nullptr){
        upright->print();
    }

    if(downleft != nullptr){
        downleft->print();
    }

    if(upleft != nullptr){
        upleft->print();
    }

    //std::cout<<std::endl;
}
double QuadTree::dist(double _x, double _y){

    return sqrt(pow(_x - here->getx() ,2) + pow(_y - here->gety() ,2)) - radius;

}

void QuadTree::get_actors(Particle * par, std::vector <Particle*>* ret){
    //gets what actors should be acting on each particle and appends it to ret
    //std::cout<<dist(par->getx(),par->gety())<<" : "<<radius<<std::endl;

    if(pars->size() == 1){

        //Particle * part = pars->operator[](0);
        ret->push_back((pars->operator[](0)));


    }else if(dist(par->getx(),par->gety()) < min_dist){

        if(downright != nullptr){

            downright->get_actors(par,ret);

        }

        if(upright != nullptr){

            upright->get_actors(par,ret);

        }

        if(downleft != nullptr){

            downleft->get_actors(par,ret);

        }

        if(upleft != nullptr){

            upleft->get_actors(par,ret);

        }

    }else{

        ret->push_back(here);
        //std::cout<<"this is a test"<<std::endl;

    }
}

void QuadTree::constructnode(Particle * par){
    // takes in a new particle and alters the nodes approriatly

    if(pars->size() == 0){
        //std::cout<<"here"<<std::endl;

        x = par->getx();
        y = par->gety();
        mass = par->Getmass();
        pars->push_back(par);

    }else if(pars->size() == 1){//if there is only one particle we need to create 2 new nodes to make sure each particle has its own leaf

        x = (par->getx() * par->Getmass() + x * mass)/(par->Getmass() + mass);

        y = (par->gety() * par->Getmass() + y * mass)/(par->Getmass() + mass);

        mass = mass + par->Getmass();

        pars->push_back(par);
        //Particle * par1 = par;

        for(Particle * par1 : *pars){

            if(x < par1->getx()){

                if(y > par1->gety()){

                    if(downleft == nullptr){
                        downleft = new QuadTree();
                    }

                    downleft->constructnode(par1);

                }else{

                    if(upleft == nullptr){
                        upleft = new QuadTree();
                    }
                    upleft->constructnode(par1);
                }

            }else{

                if(y > par1->gety()){

                    if(downright == nullptr){
                        downright = new QuadTree();
                    }

                    downright->constructnode(par1);

                }else{

                    if(upright == nullptr){
                        upright = new QuadTree();
                    }

                    upright->constructnode(par1);

                }
            }
        }

    }else{//

        //x = (par->getx() * par->Getmass() + x * mass)/(par->Getmass() + mass);

        //y = (par->gety() * par->Getmass() + y * mass)/(par->Getmass() + mass);

        mass = mass + par->Getmass();

        pars->push_back(par);

        //for(Particle * par1 : *pars){

            if(x > par->getx()){

                if(y > par->gety()){

                    if(downleft == nullptr){
                        downleft = new QuadTree();
                    }

                    downleft->constructnode(par);

                }else{

                    if(upleft == nullptr){
                        upleft = new QuadTree();
                    }

                    upleft->constructnode(par);

                }

            }else{

                if(y > par->gety()){

                    if(downright == nullptr){
                        downright = new QuadTree();
                    }

                    downright->constructnode(par);

                }else{

                    if(upright == nullptr){
                        upright = new QuadTree();
                    }

                    upright->constructnode(par);

                }
            }

    }

    here->setx(x);
    here->sety(y);
    here->setmass(mass);
    //std::cout<<here->getx() - par->getx()<<std::endl;

    double _radius = abs(sqrt(pow(par->getx() - x, 2) + pow(par->gety() - y, 2)));

    if (radius < _radius){
        radius = _radius;
        //std::cout<<dist(par->getx(),par->gety())<<std::endl;
    }
}
