#include "quadtree.h"

QuadTree::QuadTree()
{
    upleft = nullptr;
    upright = nullptr;
    downleft = nullptr;
    downright = nullptr;
    x = 99999;
    y = 99999;
    mass = 6666;
    radius = 0;
    x0 = 999999;
    y0 = 999999;
    y1 = -999999;
    x1 = -999999;
    here = new Particle(0,0,0,0,0,1,9999999);
    pars = new std::vector <Particle*>;
    min_dist = 0.5;

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
    radius = 0;
    here = new Particle(0,0,0,0,0,1,9999999);
    pars->clear();
}

void QuadTree::print(){
    //prints the tree
    bool down = false;

    if(downright != nullptr){
        downright->print();
        down=true;
    }

    if(upright != nullptr){
        upright->print();
        down=true;
    }

    if(downleft != nullptr){
        downleft->print();
        down=true;
    }

    if(upleft != nullptr){
        upleft->print();
        down=true;
    }
    std::cout<<" size :"<<pars->size()<<" cords:"<<x<<","<<y<<" radius:"<<radius<<" :: ";
    if(!down){
        std::cout<<std::endl;
    }


    //std::cout<<std::endl;
}
double QuadTree::dist(double _x, double _y){

    return (radius)/(sqrt(pow(_x - here->getx() ,2) + pow(_y - here->gety() ,2)));

}

void QuadTree::get_actors(Particle * par, std::vector <Particle*>* ret){
    //gets what actors should be acting on each particle and appends it to ret
    //std::cout<<dist(par->getx(),par->gety())<<" : "<<radius<<std::endl;

    if(pars->size() == 1){

        Particle * part = pars->front();
        ret->push_back(part);


    }else if(dist(par->getx(),par->gety()) < min_dist){

        ret->push_back(here);

        //std::cout<<"here"<<std::endl;

    }else{

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

    }
}

void QuadTree::constructnode(Particle * par){
    // takes in a new particle and alters the nodes approriatly

    if(pars->size() == 0){
        //std::cout<<"here"<<std::endl;

        x = par->getx();
        y = par->gety();
        x0 = x;
        y0 = y;
        x1 = x;
        y1 = y;
        mass = par->Getmass();
        pars->push_back(par);
        //radius = 10;
        return;

    }else if(pars->size() == 1){//if there is only one particle we need to create 2 new nodes to make sure each particle has its own leaf

        x = (par->getx() * par->Getmass() + x * mass)/(par->Getmass() + mass);

        y = (par->gety() * par->Getmass() + y * mass)/(par->Getmass() + mass);

        mass += par->Getmass();

        pars->push_back(par);
        //Particle * par1 = par;

        for(Particle * par1 : *pars){

            if(x > par1->getx()){

                if(y < par1->gety()){

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

                if(y < par1->gety()){

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
//        for(Particle* par3: *pars){
//            if((sqrt(pow(par3->getx() - x, 2) + pow(par3->gety() - y , 2)) > radius)){
//                radius = sqrt(pow(par3->getx() - x, 2) + pow(par3->gety() - y , 2));
//            }

//        }

    }else{//

        x = (par->getx() * par->Getmass() + x * mass)/(par->Getmass() + mass);

        y = (par->gety() * par->Getmass() + y * mass)/(par->Getmass() + mass);

        mass = mass + par->Getmass();

        pars->push_back(par);

        //for(Particle * par1 : *pars){

            if(x > par->getx()){

                if(y < par->gety()){

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

                if(y < par->gety()){

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
//            for(Particle* par3: *pars){
//                if((sqrt(pow(par3->getx() - x, 2) + pow(par3->gety() - y , 2) > radius))){
//                    radius = sqrt(pow(par3->getx() - x, 2) + pow(par3->gety() - y , 2));
//                }

//            }
        }


    //here->setvx((par->getvx() * par->Getmass() + here->getvx() * mass)/(par->Getmass() + mass));
    //here->setvy((par->getvy() * par->Getmass() + here->getvy() * mass)/(par->Getmass() + mass));

    if(par->getx() > x1){
        x1 = par->getx();

    }else if(par->getx() < x0){
        x0 = par->getx();
    }

    if(par->gety() > y1){
        y1 = par->gety();

    }else if(par->gety() < y0){
        y0 = par->gety();
    }
    radius = sqrt(pow(x1-x0,2)+pow(y1-y0,2));
    here->setx(x);
    here->sety(y);
    here->setmass(mass);
    //std::cout<<here->getx() - par->getx()<<std::endl;

    //double _radius = 2*(sqrt(pow(par->getx() - x, 2) + pow(par->gety() - y, 2)));

    //if (radius < _radius){
        //radius = _radius;
//        for(Particle* par3: *pars){
//            if((sqrt(pow(par3->getx() - x, 2) + pow(par3->gety() - y , 2)) > radius)){
//                radius = sqrt(pow(par3->getx() - x, 2) + pow(par3->gety() - y , 2));
//            }

//        }
    //}
}


void QuadTree::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget){

    painter->setBrush(QBrush(Qt::gray));
    painter->setOpacity(0.1);
    painter->drawRect(x0  , y0 ,x1 -x0, y1-y0);
    painter->setBrush(QBrush(Qt::darkCyan));
    painter->drawEllipse(x-here->getsize()/2,y-here->getsize()/2,here->getsize(),here->getsize());
    if(downright != nullptr){
        //painter->setBrush(QBrush(Qt::blue));
        downright->paint(painter,item,widget);
    }

    if(upright != nullptr){
        //painter->setBrush(QBrush(Qt::red));
        upright->paint(painter,item,widget);
    }

    if(downleft != nullptr){
        //painter->setBrush(QBrush(Qt::green));
        downleft->paint(painter,item,widget);
    }

    if(upleft != nullptr){
        //painter->setBrush(QBrush(Qt::gray));
        upleft->paint(painter,item,widget);
    }

}
void QuadTree::DrawQuadTree(long double _x, long double _y, QGraphicsScene *scene){

}
QRectF QuadTree::boundingRect() const
{
    return QRectF(x, y, radius, radius);

}
