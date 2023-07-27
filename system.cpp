#include "system.h"
#include <iostream>
#include <thread>
#define _USE_MATH_DEFINES
#include <math.h>
#include <mutex>


System* System::Instance = NULL;

double System::step_ = 1;

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



System::System()
{


}
void System::Idreset(){

    //resets the particles index in the system

    std::vector <Particle *> hold = *particles;

    for(int id = 0; id < *size ; id++){
        hold[id]->setid(id);
    }

//    *particles = hold;
}

void System::Remove(int id, std::vector <Particle *> input){

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

    *size = *size + 1;

    return par;
}

Particle* System::addParticle(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed){

    //adds a particle to system

    Particle* par = new Particle( Mass, _x, _y , _vx, _vy, fixed, *size);

    particles->push_back(par);

    *size = *size + 1;

    return par;
}

void System::collision(Particle* par, Particle* par1){
    // collision between particles
    // std::cout<<par->getid()<<std::endl;

    if(!par->getcol() && !par1->getcol()){
        par->hascol();
        par1->hascol();

        double posx = (par1->getx()*par1->Getmass()+par->getx()*par->Getmass())/(par->Getmass()+par1->Getmass());// new particle position

        double posy = (par1->gety()*par1->Getmass()+par->gety()*par->Getmass())/(par->Getmass()+par1->Getmass());


        double rel_vel_x = par->getvx() - par1->getvx();//relative velocities
        double rel_vel_y = par->getvy() - par1->getvx();

        double rel_speed = sqrt(pow(rel_vel_x,2)+pow(rel_vel_y,2));
        double relunitvecx = rel_vel_x/rel_speed;
        double relunitvecy = rel_vel_y/rel_speed;

        double rel_pos_x = par->getx() - par1->getx();//relative position
        double rel_pos_y = par->gety() - par1->gety();

        double rel_d_pos = sqrt(pow(rel_pos_x,2)+pow(rel_pos_y,2));
        double relunitposx = rel_pos_x/rel_d_pos;
        double relunitposy = rel_pos_y/rel_d_pos;

        double collision_angle = acos((relunitvecx*relunitposx)+(relunitvecy*relunitposy))/M_PI * 90;
        //std::cout<<"angle : "<<collision_angle<<std::endl;
        //std::cout<<"relunitposx : "<<relunitposy <<std::endl;
        double relmass = 0;
        double mass1 = par1->Getmass();
        double mass = par->Getmass();
        if(par->Getmass()>par1->Getmass()){
            relmass = mass1/(mass);
        }else {
            relmass = mass/(mass1);
        }
//        std::cout<<"relmass: "<<relmass<<std::endl;


//        std::cout<<"calc : "<</*(abs(par->Getmass() - par1->Getmass()))*/ 100000 / ( 1/rel_speed / ((collision_angle)+10)) / (0.5 + relmass * 4) <<std::endl;


        double breakmass = 0;
        double break_mom_x = 0;//break away particles momentum
        double break_mom_y = 0;
        if((1000 * rel_speed) / (collision_angle+20) * (0.1 + relmass * 0.5) > *col_threshold && rel_speed > 20){//determines if any new particles will be created from collision
            //std::cout<<"new Particles"<<std::endl;
            double breaksize;
            double breakvelx;
            double breakvely;

            if(par->Getmass()>par1->Getmass()){//determines the smallest particle in the collision
                breaksize = (par1->Getmass()/5);//breakaway mass

            }else{
                breaksize = (par->Getmass()/5);

            }if (breaksize>2){
                bool side = true;//puts a particle on either side 
                for(int i = 0;i < *num_col_particles;i++){;
                    if(side){
                        breakvelx = (rel_speed*relunitposy*-1 - relunitposx*rel_speed) * .1;
                        breakvely = (rel_speed*relunitposx - relunitposy*rel_speed) * .1;
                        addParticle(breaksize,  par->getx()-(rel_pos_x)-(sqrt(breaksize/3.14))*relunitposx-2*relunitposy,  par->gety() - (rel_pos_y)-(sqrt(breaksize/3.14)) * relunitposy+2*relunitposx,  breakvelx,  breakvely, 0);
                    }else{
                        breakvelx = (rel_speed*relunitposy - relunitposx*rel_speed) * .1;
                        breakvely = (rel_speed*relunitposx*-1 - relunitposy*rel_speed) * .1;
                        addParticle(breaksize,  par->getx()-(rel_pos_x)-(sqrt(breaksize/3.14))*relunitposx+2*relunitposy,  par->gety() - (rel_pos_y)-(sqrt(breaksize/3.14)) * relunitposy-2*relunitposx,  breakvelx,  breakvely, 0);
                    }
                    break_mom_x += breakvelx * breaksize;
                    break_mom_y += breakvely * breaksize;
                    breakmass += breaksize;
                    side = !side;
                    //std::cout<<side<<std::endl;
                }
            }
        }

        double vx = (par1->getvx()*par1->Getmass()+par->getvx()*par->Getmass()-break_mom_x)/(par->Getmass()+par1->Getmass());//new particle velocity

        double vy = (par1->getvy()*par1->Getmass()+par->getvy()*par->Getmass()-break_mom_y)/(par->Getmass()+par1->Getmass());

        bool fix = false;

        if (par->getfix() || par1->getfix()){
            fix = true;
        }

        addParticle(par->Getmass()+par1->Getmass() - breakmass, posx , posy , vx , vy , fix);


        //Idreset();
    }

}
void System::clear(){

    particles->clear();
    *size = 0;

}

cords System::gravity1(double par1x , double par1y , double par2x , double par2y , double m1, double m2){
    //input x and y cordiantes and mass of 2 particles
    //returns gravitational attraction
    long double dist = sqrt(pow(par1x - par2x , 2) + pow(par1y - par2y , 2));

    long double grav = -(*g * m1 * m2) / pow(dist , 2);

    cords ret;

    ret.x = *step * (*g )  *grav * (par1x - par2x) / dist;// calculates x component of force due to gravity
    ret.y = *step * (*g )  *grav * (par1y - par2y) / dist;// y component of gravity


//    std::cout<<ret.y<<":";
//    std::cout<<ret.x<<std::endl;
    return ret;

}




bool System::process(){
    //allocates threads to particles and returns if any paricles collided

    int parper = *size/threads + 1;

    if(*size == 0){return false;}

    const int length = *size;

    //std::cout<<parper<<std::endl;

    std::thread *Mythreads=new std::thread[threads];

    int start =  parper;

    *beencol = false;

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

    for(int i = 1; i < threads ; i++ ){
        Mythreads[i].join();
    }

    if(*beencol){//checks to see if any paricles have collided
        
        const std::vector <Particle *> hold = *particles;
        
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


bool System::update(int start, int end){
    //takes the first par id to compute to the last particle there respective motion
    //returns if a colision has occured

    bool col = false; //if collision

    const std::vector <Particle *> hold = *particles;

    for (int i = start ; i < end ; i++){

        Particle* par = hold[i];

        if(par->getfix() == false){

            par->setx(par->getx() + par->getvx() **step );//move particle

            par->sety(par->gety() + par->getvy() **step );

            for(auto &par1 : hold){
                if(par1->getid() != par->getid() && par1->getcolnum() == -1){

                    if((par->getsize() + par1->getsize()) / 2 > sqrt(pow(par->getx() - par1->getx(),2) + pow(par->gety() - par1->gety() , 2))){//check for collision

                        if(!par->getcol() && !par1->getcol()){//check to see if a particle has already collided

                            par->setcol(par1->getid());

                            par1->setcol(par->getid());

                        }
                        *beencol = true;


                    }else{


                        cords k1 = gravity1(par->getx() , par->gety() , par1->getx(), par1->gety(), par->Getmass(), par1->Getmass());// Runge - Kutta calculates force on the particles

                        cords k2 = gravity1(par->getx() + k1.x/2**step  , par->gety() + k1.y/2**step , par1->getx() - k1.x/2**step , par1->gety() -k1.y/2**step , par->Getmass(), par1->Getmass());

                        cords k3 = gravity1(par->getx() + k2.x/2**step , par->gety() + k2.y/2**step ,  par1->getx() - k1.x/2**step , par1->gety() -k1.y/2**step , par->Getmass(), par1->Getmass());

                        cords k4 = gravity1(par->getx() + k3.x**step  , par->gety() + k3.y**step ,  par1->getx() - k1.x**step , par1->gety() -k1.y**step , par->Getmass(), par1->Getmass());

                        par->setvx(par->getvx() + (k1.x + 2*k2.x + 2*k3.x + k4.x)/6/par->Getmass());

                        par->setvy(par->getvy() + (k1.y + 2*k2.y + 2*k3.y + k4.y)/6/par->Getmass());

                        par1->setvx(par1->getvx() - (k1.x + 2*k2.x + 2*k3.x + k4.x)/6/par1->Getmass());

                        par1->setvy(par1->getvy() - (k1.y + 2*k2.y + 2*k3.y + k4.y)/6/par1->Getmass());

                    }

                }
            }

        }


    }

    return col;
}
