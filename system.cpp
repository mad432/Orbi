#include "system.h"
#include <iostream>
#include <thread>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>



System* System::Instance = NULL;

double System::step_ = .0005;

double* System::step = &step_;

double System::g_ = .001;

double* System::g = &g_;

int System::size_ = 0;

int* System::size = &size_;

bool System::beencol_ = false;

bool* System::beencol = &beencol_;

float System::col_threshold_ = 50;

float* System::col_threshold = &col_threshold_;

int System::num_col_particles_ = 2;

int* System::num_col_particles = &num_col_particles_;

std::vector <Particle *> System::particles_;

std::vector <Particle *>* System::particles = &particles_;

double System::ref_frame_vx = 0;

double System::ref_frame_vy = 0;

bool System::Special_rel = false;

double System::C = 1000;

bool System::collisionEnabled = true;

bool System::DebrisEnabled = true;

std::vector <Flight_plan*> System::flights_ = {};

std::vector <Flight_plan*>* System::flights = &flights_;

System::System()
{
    std::cout<<"System Created"<<std::endl;
}

System::~System(){


}


void System::Idreset(){

    //resets the particles index in the system

    std::vector <Particle *> hold = *particles;

    for(int id = 0; id < *size ; id++){
        hold[id]->setid(id);
    }

//    *particles = hold;
}

void System::Remove(int id){

    // removes a particle from index id

    const std::vector <Particle *> returned = *particles;

    particles->clear();

    for(int i = 0; i < *size; i++ ){
        if(i != id){
            particles->push_back(returned[i]);
        }
    }

    *size = particles->size();

    Idreset();

}


Particle* System::addParticle(Particle* par){

    //adds a particle to system

    particles->push_back(par);

    *size += 1;

    return par;
}

Particle* System::addParticle(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed){

    //adds a particle to system

    if(Special_rel){// must not travel faster then C
        if(pow(_vx , 2) + pow(_vy , 2) > pow(C , 2)){

            double unitvec = 1 / sqrt(pow(_vx , 2) + pow(_vy , 2));

            _vx = _vx * unitvec * C - 1;

            _vy = _vy * unitvec * C - 1;

        }
    }

    Particle* par = new Particle( Mass, _x, _y , _vx, _vy, fixed, *size);

    particles->push_back(par);

    *size += 1;

    return par;
}

Rocket* System::addRocket(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed){

    if(Special_rel){// must not travel faster then C
        if(pow(_vx , 2) + pow(_vy , 2) > pow(C , 2)){

            double unitvec = 1 / sqrt(pow(_vx , 2) + pow(_vy , 2));

            _vx = _vx * unitvec * C - 1;

            _vy = _vy * unitvec * C - 1;

        }
    }

    Rocket* par = new Rocket( Mass, _x, _y , _vx, _vy, 0, *size);

    particles->push_back(par);

    *size += 1;

    return par;
};

void System::add_flight(Rocket * _cur ,int program, std::vector <Particle *> references_ , int stage_){
    //adds a flight plan to a rocket

    int j = 0;

    for(auto plan : *flights){
        if(plan->get_rocket()->getid() == _cur->getid()){

            plan->terminate();

            flights->erase(flights->begin()+j);

            j--;

        }

        j++;
    }

    Flight_plan * here = new Flight_plan(2 , *g *2 , program , references_, stage_);

    flights->push_back(here);

};

void System::collision(Particle* par, Particle* par1){
    // collision between particles
    // std::cout<<par->getid()<<std::endl;
    double posx = (par1->getx() * par1->Getmass() + par->getx() * par->Getmass()) / (par->Getmass() + par1->Getmass());// new particle position
    double posy = (par1->gety() * par1->Getmass() + par->gety() * par->Getmass()) / (par->Getmass() + par1->Getmass());


    if(!par->getcol() && !par1->getcol() && abs(posx - par->getx()) < par->getsize() + par1->getsize() && abs(posy - par->gety()) < par->getsize() + par1->getsize()){//determine if the collision has been processed already
        par->hascol();
        par1->hascol();

        double rel_vel_x = par->getvx() - par1->getvx();//relative velocities
        double rel_vel_y = par->getvy() - par1->getvx();

        double rel_speed = sqrt(pow(rel_vel_x,2)+pow(rel_vel_y,2));
        double relunitvecx = rel_vel_x/rel_speed;
        double relunitvecy = rel_vel_y/rel_speed;

        double rel_pos_x = par->getx() - par1->getx();//relative position
        double rel_pos_y = par->gety() - par1->gety();


        double rel_d_pos = sqrt(pow(rel_pos_x , 2) + pow(rel_pos_y , 2));
        double relunitposx = rel_pos_x / rel_d_pos;
        double relunitposy = rel_pos_y / rel_d_pos;

        double collision_angle = acos((relunitvecx * relunitposx) + (relunitvecy * relunitposy)) / M_PI * 90;
        //std::cout<<"angle : "<<collision_angle<<std::endl;
        //std::cout<<"relunitposx : "<<relunitposy <<std::endl;

        double relmass = 0;
        double mass1 = par1->Getmass();
        double mass = par->Getmass();
        double nvx = 0;// keeps track of collision speed
        double nvy = 0;

        if(par->Getmass()>par1->Getmass()){//finds the smaller of the particles
            nvx = par->getvx();
            nvy = par->getvy();
            relmass = mass1/(mass);
        }else {
            nvx = par1->getvx();
            nvy = par1->getvy();
            relmass = mass/(mass1);
        }
        //std::cout<<"relmass: "<<relmass<<std::endl;


        //std::cout<<"calc : "<</*(abs(par->Getmass() - par1->Getmass()))*/ 100000 / ( 1/rel_speed / ((collision_angle)+10)) / (0.5 + relmass * 4) <<std::endl;


        double breakmass = 0;
        double break_mom_x = 0;//break away particles momentum
        double break_mom_y = 0;

        if((100 * pow(rel_speed,2)) / (collision_angle+20) * (0.3 + relmass) > *col_threshold && rel_speed > 10 && DebrisEnabled && !Special_rel){//determines if any debris will be created from collision

            double breaksize;
            double breakvelx;
            double breakvely;

            if(par->Getmass()>par1->Getmass()){//determines the smallest particle in the collision
                breaksize = (par1->Getmass()/5);//breakaway mass

            }else{
                breaksize = (par->Getmass()/5);

            }if (breaksize>2){
                bool side = true;//puts a particle on either side
                for(int i = 0 ; i < *num_col_particles ; i++){;
                    if(side){

                        breakvelx = (rel_speed * relunitposy * -1 - relunitposx * rel_speed) * .1 + nvx;
                        breakvely = (rel_speed * relunitposx - relunitposy * rel_speed) * .1 + nvy;
                        addParticle(breaksize,  par->getx() - (rel_pos_x) - (sqrt( breaksize / 3.14)) * relunitposx - (2 * relunitposy),  par->gety() - (rel_pos_y)-(sqrt(breaksize/3.14)) * relunitposy + (2 * relunitposx),  breakvelx,  breakvely, 0);

                    }else{

                        breakvelx = (rel_speed * relunitposy - relunitposx * rel_speed) * .1 + nvx;
                        breakvely = (rel_speed * relunitposx * -1 - relunitposy * rel_speed) * .1 + nvy;
                        addParticle(breaksize,  par->getx() - (rel_pos_x) - (sqrt( breaksize / 3.14)) * relunitposx + (2 * relunitposy),  par->gety() - (rel_pos_y)-(sqrt(breaksize/3.14)) * relunitposy - (2 * relunitposx),  breakvelx,  breakvely, 0);

                    }
                    break_mom_x += breakvelx * breaksize;
                    break_mom_y += breakvely * breaksize;
                    breakmass += breaksize;
                    side = !side;
                    //std::cout<<side<<std::endl;
                }
            }
        }

        double parlorentz = 1;
        double par1lorentz = 1;

        if (Special_rel){
            parlorentz = lorentz( par->getvx(), par->getvy());
            par1lorentz = lorentz( par1->getvx(), par1->getvy());
        }

        double vx = (par1->getvx() * par1->Getmass() + par->getvx()*par->Getmass() - break_mom_x) / (par->Getmass() * parlorentz + par1->Getmass() * par1lorentz);//new particle velocity

        double vy = (par1->getvy() * par1->Getmass() + par->getvy()*par->Getmass() - break_mom_y) / (par->Getmass() * parlorentz + par1->Getmass() * par1lorentz);

        bool fix = false;

        if (par->getfix() || par1->getfix()){
            fix = true;
        }

        Particle * result = addParticle(par->Getmass()+par1->Getmass() - breakmass, posx , posy , vx , vy , fix);

        int j = 0;

        //std::vector<Flight_plan*> hold = *flights;

        for(Flight_plan* plan: *flights){//don't break flight plans

                if(plan->get_rocket()->getid() == par->getid() || plan->get_rocket()->getid() == par1->getid()){

                   flights->erase(flights->begin()+j);

                    plan->terminate();

                    j--;

                }else{

                    for(auto planet:plan->get_references()){

                        int i = 0;

                        //std::cout<<"id"<<par->getid()<<" : "<<result->getid();

                        if(par->getid() == planet->getid() || par1->getid() == planet->getid()){

                            std::vector <Particle *> nref = plan->get_references();

                            //nref.erase(nref.begin() + i);

                            nref[i] = result;

                            //std::cout<<result->getid()<<std::endl;

                            plan->setrefernces(nref);

                            std::cout<<"id : "<<par->getid()<<" : "<<plan->get_references()[i]->getid()<<" : "<<result->getid()<<std::endl;

                            i++;

                            }

                        i++;
                    }

                }

            j++;
            }
        }
    //std::cout<<"size : "<<flights->size()<<std::endl;




        //Idreset();


}
void System::clear(){

    for(auto par : *particles){
        delete par;
    }

    particles->clear();
    for(auto flight : *flights){

        flight->terminate();

        //delete flight;

    }
    flights->clear();

    *size = 0;

}

cords System::gravity(double par1x , double par1y , double par2x , double par2y , double m1, double m2, double step_size){
    //input x and y cordiantes and mass of 2 particles
    //returns gravitational attraction
    long double dist = sqrt(pow(par1x - par2x , 2) + pow(par1y - par2y , 2));

    long double grav = -(*g * m1 * m2) / pow(dist , 2);

    cords ret;

    ret.x = step_size * grav * (par1x - par2x) / dist;// calculates x component of force due to gravity
    ret.y = step_size * grav * (par1y - par2y) / dist;// y component of gravity


//    std::cout<<ret.y<<":";
//    std::cout<<ret.x<<std::endl;
    return ret;

}



double System::lorentz(double vx, double vy){
    //returns the lorentz factor for a given velocity
    double speed2 =  pow((vx - ref_frame_vx) , 2) + pow((vy - ref_frame_vy) , 2);

    if(speed2 + 10  > pow(C , 2)){
        std::cout<<"warning exceeded lorentz"<<std::endl;

        return 100;
    }

    return 1 / sqrt(1 - (speed2 / pow(C , 2)));
}




bool System::process(){
    //allocates threads to particles and returns if any particles collided

    int parper = *size/threads + 1;//particles per thread

    if(*size == 0){return false;}

    const int length = *size;

    //std::cout<<parper<<std::endl;

    static std::thread *Mythreads = new std::thread[threads];

    int start =  parper;

    *beencol = false;

    std::vector <Particle *> hold = *particles;

    for(int i = 0; i < threads ; i++ ){
        //std::lock_guard<std::mutex> guard(myMutex);
        if(start < length - parper){

            Mythreads[i] = std::thread(update, start, start + parper);

            //std::cout<<"thread : "<<i<<" from :"<<start<<" to :"<<start+parper<<std::endl;

        }else{

            Mythreads[i] = std::thread(update, start, length);

            //std::cout<<"thread : "<<i<<" from :"<<start<<" to :"<<*size<<std::endl;

        }
        start = start + parper;
    }

    update(0,parper);

    for(int i = 0; i < threads ; i++ ){
        Mythreads[i].join();
    }

    if(*beencol && collisionEnabled){//checks to see if any paricles have collided

        for(auto &par : *particles){

            if(par->getcolnum() != -1 && !par->getcol()){

                for(auto &par1 : *particles){

                    if(par->getcolnum() == par1->getid()){

                        collision(par,par1);
                    }
                }
            }
        //std::cout<<par->getcolnum()<<std::endl;
        }
    }
//    tickcount++;
//    std::cout<<tickcount<<std::endl;

    return *beencol;

};

//std::mutex myMutex;


bool System::update(int start, int end){
    //takes the first par id to compute to the last particle (there respective motion)
    //returns if a collision has occured

    bool col = false; //if collision

    const std::vector <Particle *> hold = *particles;

    double *lorentztable =new double[*size];//tabulation

    for(int i = 0; i < *size; i++){
        if(Special_rel){
            lorentztable[i] = lorentz(hold[i]->getvx() , hold[i]->getvy());
        }else{
            break;
        }
    }

    for (int i = start ; i < end ; i++){

        Particle* par = hold[i];

        if (par->object() == "Rocket"){

            par->changeheading(par->getaV() * *step * 2000);
        }

        if(par->getcol()==true){
            *beencol = true;
        }

        if(par->getfix() == false){

            double rel_step = *step;

            double parlorentz = 1;

            if (Special_rel){

                parlorentz = lorentztable[par->getid()];

                rel_step = *step / parlorentz; // time dilation

            }

            par->setx(par->getx() + (par->getvx() * *step));//move particle

            par->sety(par->gety() + (par->getvy() * *step));

            for(auto &par1 : hold){

                if(par1->getid() != par->getid() && par1->getcolnum() == -1){

                    //myMutex.lock();

                    if((par->getsize() + par1->getsize()) / 2 > sqrt(pow(par->getx() - par1->getx() , 2) + pow(par->gety() - par1->gety() , 2))){//check for collision

                        if(!par->getcol() && !par1->getcol()){//check to see if a particle has already collided

                            par->setcol(par1->getid());

                            par1->setcol(par->getid());

                        }
                        *beencol = true;


                    }else{

                        double par1lorentz = 1;

                        if (Special_rel){

                            par1lorentz = lorentztable[par1->getid()];

                        }


                        cords k1 = gravity(par->getx() , par->gety() , par1->getx(), par1->gety(), par->Getmass() , par1->Getmass() , rel_step);// Runge - Kutta calculates force on the particles

                        cords k2 = gravity(par->getx() + (k1.x/2 / (par->Getmass() * parlorentz) * rel_step) , par->gety() + (k1.y/2 / (par->Getmass()* parlorentz) * rel_step) , par1->getx() - (k1.x/2 / (par1->Getmass() * par1lorentz) * rel_step)  , par1->gety() - (k1.y/2 / (par1->Getmass() * par1lorentz) * rel_step) , par->Getmass() , par1->Getmass() , rel_step);

                        cords k3 = gravity(par->getx() + (k2.x/2 / (par->Getmass() * parlorentz) * rel_step) , par->gety() + (k2.y/2 / (par->Getmass()* parlorentz) * rel_step) , par1->getx() - (k2.x/2 / (par1->Getmass() * par1lorentz) * rel_step)  , par1->gety() - (k2.y/2 / (par1->Getmass() * par1lorentz) * rel_step)  , par->Getmass() , par1->Getmass() , rel_step);

                        cords k4 = gravity(par->getx() + (k3.x / (par->Getmass() * parlorentz) * rel_step) , par->gety() + (k3.y/ (par->Getmass()* parlorentz) * rel_step) ,  par1->getx() - (k3.x/(par1->Getmass() * par1lorentz) * rel_step)  , par1->gety() - (k3.y/ (par1->Getmass() * par1lorentz) * rel_step)  , par->Getmass(), par1->Getmass(), rel_step);


                        if (Special_rel){

                            par->setvx( par->getvx() + 2 * (k1.x + 2*k2.x + 2*k3.x + k4.x)/6 / (par->Getmass() * parlorentz));

                            par->setvy( par->getvy() + 2 * (k1.y + 2*k2.y + 2*k3.y + k4.y)/6 / (par->Getmass() * parlorentz));

                            if((pow(par->getvx(),2) + pow(par->getvy(),2)) >= pow(C,2)){//make sure we didn't overshoot C

                                std::cout<<"warning exceeded C"<<std::endl;

                                if(par->getvx()>0){

                                    par->setvx(par->getvx() - 1);

                                }else{

                                    par->setvx(par->getvx() + 1);
                                }

                                if(par->getvy()>0){

                                    par->setvy(par->getvy() - 1);

                                }else{

                                    par->setvy(par->getvy() + 1);
                                }
                            }

                        }else{

                            if(par1->getfix()){ // keeps force consistant

                                par->setvx( par->getvx() + 2 * (k1.x + 2*k2.x + 2*k3.x + k4.x)/6 / par->Getmass());

                                par->setvy( par->getvy() + 2 * (k1.y + 2*k2.y + 2*k3.y + k4.y)/6 / par->Getmass());

                            }else{

                                par->setvx( par->getvx() + (k1.x + 2*k2.x + 2*k3.x + k4.x)/6 / par->Getmass());

                                par->setvy( par->getvy() + (k1.y + 2*k2.y + 2*k3.y + k4.y)/6 / par->Getmass());

                                par1->setvx( par1->getvx() - (k1.x + 2*k2.x + 2*k3.x + k4.x)/6 / par1->Getmass());

                                par1->setvy( par1->getvy() - (k1.y + 2*k2.y + 2*k3.y + k4.y)/6 / par1->Getmass());
                            }

                        }


                    }
                }
            }

        }else{

            for(auto &par1 : hold){

                if(par1->getid() != par->getid() && par1->getcolnum() == -1){


                        if((par->getsize() + par1->getsize()) / 2 > sqrt(pow(par->getx() - par1->getx() , 2) + pow(par->gety() - par1->gety() , 2))){//check for collision

                            if(!par->getcol() && !par1->getcol()){//check to see if a particle has already collided

                                par->setcol(par1->getid());

                                par1->setcol(par->getid());

                            }
                            *beencol = true;
                        }

                  }
             }
        }


    }

    delete[] lorentztable;

    return col;
}
