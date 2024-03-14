#include "flight_plan.h"
#include <iostream>
#include <chrono>
#define _USE_MATH_DEFINES
#include <math.h>
#include "system.h"



//Rocket * Flight_plan::current(ref) = nullptr;

double  Flight_plan::G = 0;

//int Flight_plan::rocket = 0;

//std::thread * Flight_plan::worker = nullptr;

//std::vector <Particle *> Flight_plan::references = {};

int Flight_plan::stage = 0;

//bool Flight_plan::terminated = false;

void Flight_plan::terminate(){terminated = true; std::cout<<terminated<<std::endl;}

Flight_plan::Flight_plan(int _cur , double G_ , int program_, std::vector <Particle *> references_, int stage_)
{
    wait(50);

    program = program_;

    terminated = false;

    //rocket = _cur->getid();

    G = G_;

    references = references_;

    stage = stage_;

    worker = new std::thread(program_sel, program, &references, _cur , &terminated, this);

    worker->detach();


}

Particle * Flight_plan::planet(int pla ,std::vector<Particle*>* ref, bool *ter){

    //fetches one of the referenced planets

    void * here = (*ref)[pla];

    if (here != nullptr && ter){

        return (*ref)[pla];

    }else{

        throw 0;

        return nullptr;

    }

}



Rocket * Flight_plan::current(std::vector<Particle*>* ref, bool *ter){
    //fetches the attached rocket
        Particle * here = planet(2, ref, ter);

        Rocket * ret_rock =  dynamic_cast<Rocket*>(here);

        if (ter && ret_rock != nullptr){

            return ret_rock;

        }else{

            throw 1;

            return nullptr;

        }
}

void Flight_plan::wait(int time){

    std::this_thread::sleep_for(std::chrono::milliseconds(1 + time / (1+int(System::getInstance()->get_step() * 1000))));

}

Flight_plan::~Flight_plan(){

    terminated = true;

}

double Flight_plan::distance(int planet_1 , int planet_2 ,std::vector<Particle*>* ref, bool *ter){
    //return the distance from planet_1 to planet_2

    return sqrt(pow(planet(planet_1 ,  ref, ter)->getx() - planet(planet_2 ,  ref, ter)->getx(), 2) + pow(planet(planet_1 ,  ref, ter)->gety() - planet(planet_2 ,  ref, ter)->gety(), 2));


}

double Flight_plan::periapsis(int planet_, std::vector<Particle*>* ref, bool *ter){
    //calculates parihapsis height assuming minimal external interferance

    double x = current(ref, ter)->getx() - planet(planet_ , ref, ter)->getx();
    double y = current(ref, ter)->gety() - planet(planet_ , ref, ter)->gety();
    double vx = current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx();
    double vy = current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy();

    double r  = sqrt(pow(x , 2)+pow(y , 2));//convert to polar cordinates
    double rdot = vx * (x / r) + vy * (y / r);
    //double theta = acos(x/r);
    double thetadot = sqrt((pow(vx , 2) + pow(vy , 2)) - (pow(rdot , 2))) / r;

    double L = thetadot * pow(r , 2);//constant term from angular momentum

    double GM = G * planet(planet_ , ref, ter)->Getmass();

    double a = .5 * ( pow(rdot , 2) + pow(r * thetadot, 2)) - GM / r;//total energy

    double solved = (GM - sqrt(pow(GM, 2) + 4 * a * pow(L, 2) / 2))/(-2 * a);//quadratic equation solve for when rdot = 0 lower value from energy

    //std::cout<<"solved : "<<r<<std::endl;

    return (solved);

}

double Flight_plan::Apoapsis(int planet_, std::vector<Particle*>* ref, bool *ter){
    //calculates Apoapsis height assuming minimal external interferance

    double x = current(ref, ter)->getx() - planet(planet_ , ref, ter)->getx();
    double y = current(ref, ter)->gety() - planet(planet_ , ref, ter)->gety();
    double vx = current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx();
    double vy = current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy();

    double r  = sqrt(pow(x , 2)+pow(y , 2));//convert to polar cordinates
    double rdot = vx * (x / r) + vy * (y / r);
    //double theta = acos(x/r);
    double thetadot = sqrt((pow(vx , 2) + pow(vy , 2)) - (pow(rdot , 2))) / r;

    double L = thetadot * pow(r , 2);//constant term from angular momentum

    double GM = G * planet(planet_ , ref, ter)->Getmass();

    double a = .5 * ( pow(rdot , 2) + pow(r * thetadot, 2)) - GM / r;//total energy

    double solved = (GM + sqrt(pow(GM, 2) + 4 * a * pow(L, 2) / 2))/(-2 * a);//quadratic equation solve for when rdot = 0 higher value from energy

    return (solved);

}

double Flight_plan::true_anomaly(int planet_, std::vector<Particle*>* ref, bool *ter){
    //calculates the true anomaly

    double x = current(ref, ter)->getx() - planet(planet_ , ref, ter)->getx();
    double y = current(ref, ter)->gety() - planet(planet_ , ref, ter)->gety();
    double vx = current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx();
    double vy = current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy();
    double r  = sqrt(pow(x , 2) + pow(y , 2));//convert to polar
    double rdot = vx * (x / r) + vy * (y / r);

    double per = periapsis(planet_,ref,ter);
    double app = Apoapsis(planet_,ref,ter);

    return abs_ang(180 - rdot/abs(rdot) * 180/M_PI * acos((pow(r, 2) + pow(app - per, 2) - pow(per + app - r, 2))/(2 * r * (app - per))));//properties of ellipse
}

double Flight_plan::eccentricity(int planet_, std::vector<Particle*>* ref, bool *ter){
    //calculates eccentricity

    double per = periapsis(planet_,ref,ter);
    double app = Apoapsis(planet_,ref,ter);

    return(app-per)/(app+per);
}

int Flight_plan::escape_angle(int planet_,std::vector<Particle*>* ref, bool *ter,double vel){
    //calculates the angle the rocket will leave from a planet

    double ve = sqrt(abs(pow(vel,2) - 2 * G * planet(planet_,ref,ter)->Getmass() /distance(2,planet_,ref,ter)));


    std::cout<<"esc_angle : "<<180/3.14*acos((ve-1)/sqrt(G * planet(planet_,ref,ter)->Getmass() /distance(2,planet_,ref,ter)))<<std::endl;
    return 180/3.14*acos((ve - 1)/sqrt(G * planet(planet_,ref,ter)->Getmass() /distance(2,planet_,ref,ter)));
}


void Flight_plan::program_sel(int program ,std::vector<Particle*>* ref,int rocket, bool *ter, Flight_plan* here){
    //selects a flight plan to follow
    wait(30);

    try{

        if(program == 3){

            circularize(0 , ref, rocket, ter, 200);
            //setheading("prograde",0,ref,ter);
            std::cout<<"orbit of "<<Apoapsis(0,ref,ter)<<" by "<<periapsis(0,ref,ter)<<std::endl;

        }

        if(program == 2){

            Inter_planet( 1 , 3 , ref ,rocket,ter);

        }

        if(program == 1){//transfer from planet 0 to planet 1


          hohmann_transfer(1 , ref, rocket, ter);

        }

    }catch(int cat){

                std::cout<<"Flight plan terminated with error code : "<<cat<<std::endl;

    }catch(...){
                std::cout<<"Flight plan terminated with error code :-101"<<std::endl;
    }

    //worker->~thread();
    std::cout<<"flight plan finished"<<std::endl;
    //delete here;


};

double Flight_plan::angle(int a, int b, int c, std::vector<Particle*>* ref, bool *ter){
    //returns the angle at b from a,c

        double ax = planet(a,ref, ter)->getx();
        double ay = planet(a,ref, ter)->gety();
        double bx = planet(b,ref, ter)->getx();
        double by = planet(b,ref, ter)->gety();
        double cx = planet(c,ref, ter)->getx();
        double cy = planet(c,ref, ter)->gety();

        double vec_abx = (ax - bx);
        double vec_aby = (ay - by);

        double vec_cbx = (cx - bx);
        double vec_cby = (cy - by);

        return acos((vec_cbx * vec_abx + vec_cby * vec_aby)/( sqrt( pow(vec_cbx , 2) + pow(vec_cby , 2)) * sqrt( pow(vec_abx , 2) + pow(vec_aby , 2) ))) * 180 / M_PI;

}

int Flight_plan::abs_ang(int angle){
    //converts an angle to be between 0 and 359

    int ret_ang = angle % 360;

    if (ret_ang < 0){

        return 360 + ret_ang;

    }

    return ret_ang;

};

int Flight_plan::rockangle(std::vector<Particle*>* ref, bool *ter){
    //returns the rockets current heading

    if(current(ref, ter) != nullptr){

        return abs_ang(current(ref, ter)->getheading());

    }else{

       throw 0;

       return 0;

    }
}

void Flight_plan::spin(double amount,std::vector<Particle*>* ref, bool *ter){
    //spins the rocket

    current(ref, ter)->changeaV(amount);


}

void Flight_plan::setheading(int angle,std::vector<Particle*>* ref, bool *ter){
        //sets the rockets heading to an angle between 0 and 359 +- 2 degrees
        angle = abs_ang(angle);

        if(angle != rockangle(ref,ter)){

            int quad;//tells which quadrant we are currently in and the targets quadrant
            int tar_quad;

            if (angle < 90){

                tar_quad = 1;

            }else if(angle < 180){

                tar_quad = 2;

            }else if(angle < 270){

                tar_quad = 3;

            }else{

                tar_quad = 4;
            }

            if (abs_ang(rockangle(ref,ter)) < 90){

                quad = 1;

            }else if(abs_ang(rockangle(ref,ter)) < 180){

                quad = 2;

            }else if(abs_ang(rockangle(ref,ter)) < 270){

                quad = 3;

            }else{

                quad = 4;
            }


             bool side;

             //std::cout<< abs_ang(rockangle(ref,ter)) - angle%180<<std::endl;

             if ((quad == 1 && tar_quad == 4) || (quad == 4 && tar_quad == 3) || (quad == 3 && tar_quad == 2) || (quad == 2 && tar_quad == 1) ||//selects which way the rocket should spin and initiates spin
                 (quad == 1 && tar_quad == 3 && abs_ang(rockangle(ref,ter)) - angle % 180 < 0) || (quad == 4 && tar_quad == 2 && abs_ang(rockangle(ref,ter)) % 270 - angle % 90 < 0) ||
                 (quad == 3 && tar_quad == 1 && abs_ang(rockangle(ref,ter)) % 180 - angle < 0) || (quad == 2 && tar_quad == 4 && abs_ang(rockangle(ref,ter)) % 90 - angle % 270 < 0) ||
                 (quad == tar_quad && angle <= abs_ang(rockangle(ref,ter)))){

                spin(-0.1,ref,ter);

                side = true;

                //std::cout<<"left"<<std::endl;

            }else{

                spin(0.1,ref,ter);

                side = false;
                //std::cout<<"right"<<std::endl;

            }

            while((angle - 2 >= abs_ang(rockangle(ref,ter)) || abs_ang(rockangle(ref,ter)) >= angle + 2) && current(ref,ter) != nullptr){//wait intell within 2 degrees from target
                wait(1);
                //std::cout<<rockangle(ref,ter)%360<<" : "<<angle<<std::endl;
            }

            if(side){//de-spins the rocket

                spin(0.1,ref,ter);

            }else{

                spin(-0.1,ref,ter);

            }
    }


}

void Flight_plan::setheading(std::string dir, int planet_ , std::vector<Particle*>* ref, bool *ter){
    //sets the rockets heading to various usefull directions referenced from planet planet_

    double x = current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx();

    double y = current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy();

    if(dir == "prograde"){

       int prograde = - atan((x/y)) * 180 / M_PI;

       if((x <= 0 && y >=0) || (x >= 0 && y >= 0)){

            prograde = - atan((x/y)) * 180 / M_PI + 180;

        }

        setheading(prograde,ref, ter);

    }else if(dir == "retrograde"){

        int retrograde = - atan((x/y)) * 180 / M_PI +180;

        if((x <= 0 && y >=0) || (x >= 0 && y >= 0)){

            retrograde = - atan((x/y)) * 180 / M_PI;

        }

        setheading(retrograde,ref, ter);
     }else if(dir == "radial out"){

        int radial =  -atan((x/y)) * 180 / M_PI + 90;

        if((x <= 0 && y >=0) || (x >= 0 && y >= 0)){

            radial =   -atan((x/y)) * 180 / M_PI - 90;

        }

        setheading(radial,ref, ter);

      }else if(dir == "radial in"){

        int radial =  -atan((x/y)) * 180 / M_PI - 90;

        if((x <= 0 && y >=0) || (x >= 0 && y >= 0)){

            radial = -atan((x/y)) * 180 / M_PI + 90;

        }

         setheading(radial,ref,ter);

      }else if(dir == "toward"){

        double x = current(ref, ter)->getx() - planet(planet_ , ref, ter)->getx();

        double y = current(ref, ter)->gety() - planet(planet_ , ref, ter)->gety();

        int toward = - atan(x/y) * 180 / M_PI + 180;

        if((x <= 0 && y >=0) || (x >= 0 && y >= 0)){

             toward  =  -atan((x/y)) * 180 / M_PI;

         }


        setheading(toward,ref,ter);

    }
}

void Flight_plan::burn(double dv,std::vector<Particle*>* ref, bool *ter){
    //changes the rockets velocity by dv;

    std::cout<<current(ref, ter)->getvy()<<std::endl;

    double vx_0 = current(ref, ter)->getvx();

    double vy_0 = current(ref, ter)->getvy();

    std::cout<<"Burning"<<std::endl;

    std::cout<<"Target dv : current(ref) dv"<<std::endl;


    while(sqrt(pow(vx_0,2) + pow(vy_0,2)) > sqrt((pow(current(ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy(),2))) - sqrt(dv*dv) && sqrt(pow(vx_0,2) + pow(vy_0,2)) < sqrt((pow(current(ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy(),2))) + sqrt(dv*dv)){


        current(ref, ter)->thrust(50);

        std::cout<<dv<<" : "<<sqrt((pow(current(ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy(),2)))-sqrt(pow(vx_0,2) + pow(vy_0,2))<<std::endl;

        wait(10);

    }


    std::cout<<"burn finished  Starting vel:"<<sqrt(pow(vx_0,2) + pow(vy_0,2))<<" : Ending vel:"<<sqrt((pow(current(ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy(),2)))<<std::endl;




};

void Flight_plan::burn(double dv,int planet_,std::vector<Particle*>* ref,bool *ter){
    //changes the rockets velocity relative to planet_ by dv
    std::cout<<current(ref, ter)->getvy()<<std::endl;

    double vx_0 = current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx();

    double vy_0 = current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy();

    std::cout<<"Burning"<<std::endl;

    std::cout<<"Target dv : current dv"<<std::endl;


    while(sqrt(pow(vx_0,2) + pow(vy_0,2)) > sqrt((pow(current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy(),2))) - sqrt(dv*dv) &&
          sqrt(pow(vx_0,2) + pow(vy_0,2)) < sqrt((pow(current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy(),2))) + sqrt(dv*dv) ){//while our dv is less then tarket dv

        current(ref, ter)->thrust(50);

        std::cout<<dv<<" : "<<sqrt((pow(current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy(),2)))-sqrt(pow(vx_0,2) + pow(vy_0,2))<<std::endl;

        wait(10);

    }

    std::cout<<"burn finished  Starting vel:"<<sqrt(pow(vx_0,2) + pow(vy_0,2))<<" : Ending vel:"<<sqrt((pow(current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy(),2)))<<std::endl;


}

void Flight_plan::burn(double dv,int planet_,std::vector<Particle*>* ref,bool *ter , std::string heading){
    //changes the rockets velocity relative to planet_ by dv
    std::cout<<current(ref, ter)->getvy()<<std::endl;

    double vx_0 = current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx();

    double vy_0 = current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy();

    std::cout<<"Burning"<<std::endl;

    std::cout<<"Target dv : current dv"<<std::endl;


    while(sqrt(pow(vx_0,2) + pow(vy_0,2)) > sqrt((pow(current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy(),2))) - sqrt(dv*dv) &&
          sqrt(pow(vx_0,2) + pow(vy_0,2)) < sqrt((pow(current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy(),2))) + sqrt(dv*dv) ){//while our dv is less then tarket dv

        current(ref, ter)->thrust(50);

        std::cout<<dv<<" : "<<sqrt((pow(current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy(),2)))-sqrt(pow(vx_0,2) + pow(vy_0,2))<<std::endl;

        setheading(heading,planet_,ref,ter);

        wait(10);

    }

    std::cout<<"burn finished  Starting vel:"<<sqrt(pow(vx_0,2) + pow(vy_0,2))<<" : Ending vel:"<<sqrt((pow(current(ref, ter)->getvx() - planet(planet_ , ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy() - planet(planet_ , ref, ter)->getvy(),2)))<<std::endl;


}

float period(int planet ,std::vector<Particle*>* ref , int rocket, bool *ter ,int altitude){
    return 0;
}

void Flight_plan::circularize(int planet_, std::vector<Particle*>* ref, int rocket, bool *ter ,int altitude){
    //circularizes the orbit at altitude

       if(Apoapsis(planet_,ref,ter) > altitude + 2){

           while(true_anomaly(planet_,ref,ter) < 358){//wait tell periapsis

               wait(10);
               std::cout<<true_anomaly(planet_,ref,ter)<<std::endl;

               setheading("retrograde",planet_,ref,ter);

           }
           while(Apoapsis(planet_,ref,ter) > altitude+2){

               burn(0.1,planet_,ref,ter,"retrograde");

           }
       }
       if(periapsis(planet_,ref,ter) < altitude - 2){

           while(true_anomaly(planet_,ref,ter) < 178 || true_anomaly(planet_,ref,ter) > 180){//wait tell apoapsis

               //std::cout<<true_anomaly(planet_,ref,ter)<<std::endl;

               wait(10);

               setheading("prograde",planet_,ref,ter);

           }

           double start_height = periapsis(planet_,ref,ter);

           while(periapsis(planet_,ref,ter) < altitude - 2 && start_height < Apoapsis(planet_,ref,ter)){

               burn(0.1,planet_,ref,ter,"prograde");

           }

           circularize(planet_,ref,rocket,ter,altitude);
       }

}

void Flight_plan::hohmann_transfer(int planet_, std::vector<Particle*>* ref, int rocket, bool *ter){

    if(stage == 1){//lets the program jump to a certain stage

        goto stage1;//transfer

    }else if(stage == 2){

        goto stage2;//capture

    }
    //inital burn
    double h_0 = 0;

    if(planet(0 , ref, ter) != nullptr){

        h_0 = distance(rocket, 0, ref,ter);//inital height

    }

    double h_p = 0;

    if(planet(1, ref, ter) != nullptr && planet(0 , ref, ter) != nullptr){

        h_p = distance(0 , 1, ref,ter) - (20);//final height

    }

    double dv = 0;

    if(planet(0 , ref, ter) != nullptr){

        dv = sqrt((planet(0 , ref, ter)->Getmass()*G /h_0)) * (sqrt(((2.0 * h_p)/(h_p + h_0))) - 1.0);

    }


    int trans_angle = abs_ang(180 * (1 - ( 1 / (sqrt(8)) * sqrt( pow(h_0/h_p + 1 , 3)))));//transfer angle

    if (h_p < h_0){
        trans_angle = trans_angle - 180;
    }

    std::cout<<"transfer angle : "<<trans_angle<<std::endl<<"Dv required : "<<dv<<std::endl;


    while((trans_angle - 1 > angle(1,0,2,ref,ter) || angle(1,0,2,ref,ter) > trans_angle + 1 || current(ref, ter)->getvx() * (planet(1, ref, ter)->getx()-planet(0 , ref, ter)->getx()) + current(ref, ter)->getvy() * (planet(1, ref, ter)->gety()-planet(0 , ref, ter)->gety()) < 0)){

        if(h_p > h_0){
            setheading("prograde" , 0 ,ref,ter);
        }else{
            setheading("retrograde", 0 ,ref,ter);
        }

        wait(10);

    }


    double off = 0;
    if( h_0 <h_p){
        while(Apoapsis(0,ref,ter) < h_p){
            burn(0.1,0,ref,ter);
        }
    }else{
        while(periapsis(0,ref,ter) > h_p){
            burn(0.1,0,ref,ter);
        }
    }

    stage = 1;

    stage1://transfer

    intercept(planet_,ref,rocket,ter);

    stage = 2;

    stage2://capture

    Capture(planet_,ref,rocket,ter);

}

void Flight_plan::intercept(int planet_, std::vector<Particle*>* ref, int rocket, bool *ter){

    double off = 0;

    bool heading = true;

    while(distance(planet_,rocket,ref,ter) > 100){

            off = periapsis(planet_,ref,ter);

            if(off > 22.5){//if the perhiapse is to low point out else point in

               setheading("radial in" , planet_, ref,ter);
               heading = true;

            }else{

               setheading("radial out" , planet_, ref,ter);
               heading = false;

            }

            wait(10);

    }

    while(distance(planet_,rocket,ref,ter) > 75){//stops the rocket from tring to correct heading at last minute

//        if (periapsis(1,ref,ter)<20||periapsis(1,ref,ter)>25){
//            burn(0.1,0,ref,ter);
//        }
        off = periapsis(planet_,ref,ter);

        if (off > 22.5){


                setheading("radial in" , planet_,ref,ter);

                heading = true;


        }else{


                setheading("radial out" , planet_,ref,ter);

                heading = false;



        }

        wait(10);
    }
    //int F_alt = 0;

    while((off < 20 || off > 25) && (distance(planet_,rocket,ref,ter) > 25) ){//burn to raise or lower the periapsis to the desired height

        //int off1 = off;


        //off = (off - G * planet(1, ref)->Getmass() * current(ref)->Getmass())/ pow(off,2);
        off = periapsis(planet_,ref,ter);

/*        if (true_anomaly(1,ref,ter) > 180){

            setheading("towards" , 1 , ref,ter);

        }else */if (heading){

            setheading("radial in" , planet_,ref,ter);

        }else{

            setheading("radial out" , planet_,ref,ter);

        }

        if((off < 20 || off > 25)){
            burn(0.1, 1, ref,ter);
        }

        wait(5);

    }

    std::cout<<"Final alt calc: "<<periapsis(planet_,ref,ter)<<std::endl;
}

void Flight_plan::Capture(int planet_, std::vector<Particle*>* ref, int rocket, bool *ter){

    double dv = 0;

    double d_i = true_anomaly(planet_,ref,ter);

    while((d_i < 355)){// wait for periapsis

        //d_i = distance(rocket,1,ref,ter);

        if(periapsis(planet_,ref,ter)<planet(planet_,ref,ter)->getsize()){
            setheading("radial out" , planet_,ref,ter);
            burn(0.1, 1, ref,ter);
        }else{
            setheading("retrograde" , planet_,ref,ter);
        }

        wait(10);

        d_i = true_anomaly(planet_,ref,ter);


        //setheading("retrograde" , 1,ref,ter);
    }
    std::cout<<"Final alt : "<<distance(rocket,planet_,ref,ter)<<std::endl;

    dv = sqrt(pow(current(ref, ter)->getvx() - planet(planet_, ref, ter)->getvx(),2) + pow(current(ref, ter)->getvy() - planet(planet_, ref, ter)->getvy(),2)) - sqrt((G * planet(planet_, ref, ter)->Getmass()/distance(rocket , planet_,ref,ter))) ;// calculates the burn to achive a circular orbit

    std::cout<<"incertion dv : "<<dv<<std::endl;

    burn(dv, planet_,ref,ter);

    circularize(planet_,ref ,rocket,ter, periapsis(planet_,ref,ter));

}

void Flight_plan::Inter_planet(int planet_Dest, int planet_home, std::vector<Particle*>* ref, int rocket , bool *ter){
    //circularize(0,ref,rocket,ter,periapsis(0,ref,ter));

    if(stage>=1){

        hohmann_transfer(planet_Dest,ref,rocket,ter);

    }else{

        //circularize(planet_home,ref,rocket,ter,Apoapsis(planet_home,ref,ter));

        int h_0 = distance(0,planet_home,ref,ter);//inital hieght

        int h_p = distance(planet_Dest,0,ref,ter);//final height

        double int_vel = sqrt(planet(planet_home,ref,ter)->Getmass()*G / distance(rocket,planet_home,ref,ter));

        if( h_0 > h_p){
            int_vel = - int_vel;
        }

        double hohmann = sqrt((planet(0 , ref, ter)->Getmass()*G /h_0)) * ((sqrt(((2.0 * h_p)/(h_p + h_0))) - 1.0)) ;

        double dv = sqrt((pow(hohmann,2) + (2 * G * planet(planet_home,ref,ter)->Getmass() /distance(2,planet_home,ref,ter)))) - int_vel;

        int trans_angle = abs_ang(160 * (1 - ( 1 / (sqrt(8)) * sqrt( pow(h_0/h_p + 1 , 3)))))%180;//transfer angle

        std::cout<<trans_angle<<": dv"<<std::endl;
        waitangle:

        while(trans_angle - 1 > angle(planet_home,0,planet_Dest,ref,ter) || angle(planet_home,0,planet_Dest,ref,ter) > trans_angle + 1 || current(ref, ter)->getvx() * (planet(1, ref, ter)->getx()-planet(0 , ref, ter)->getx()) + current(ref, ter)->getvy() * (planet(1, ref, ter)->gety()-planet(0 , ref, ter)->gety()) < 0){

            //std::cout<<angle(1,0,2,ref,ter)<<std::endl;
            setheading("prograde" , planet_home ,ref,ter);

            wait(10);

        }

        int dept_angle =escape_angle(planet_home,ref,ter,dv + int_vel);

        if(h_p < h_0){
            dept_angle = 180 - dept_angle;
        }

        std::cout<<dept_angle<<std::endl;

        //std::cout<<dept_angle<<std::endl;

        while ((angle(0,planet_home,rocket,ref,ter) > dept_angle + 2 || angle(0,planet_home,rocket,ref,ter) < dept_angle - 2) || (distance(planet_Dest,planet_home,ref,ter)>distance(planet_Dest,rocket,ref,ter))) {

            std::cout<<abs(trans_angle - angle(planet_home,0,planet_Dest,ref,ter))<<std::endl;

            if(abs(trans_angle - angle(planet_home,0,planet_Dest,ref,ter))>20){
                goto waitangle;
            }

            setheading("prograde" , planet_home ,ref,ter);

            wait(10);
        }

        burn(dv,planet_home,ref,ter);

        wait(10000);
        setheading("prograde",0,ref,ter);

        if(h_0<h_p){

            while(Apoapsis(0,ref,ter) < distance(0,planet_Dest,ref,ter)*1){

                setheading("prograde",0,ref,ter);

                burn(0.1,0,ref,ter,"prograde");

            }

        }else{

            while(periapsis(0,ref,ter) < distance(0,planet_Dest,ref,ter)){

                setheading("retrograde",0,ref,ter);


                burn(0.1,0,ref,ter,"retrograde");

            }
        }

        stage = 1;

        hohmann_transfer(planet_Dest,ref,rocket,ter);

    }
    //wait(3000);

    //stage = 0;

    //Inter_planet(planet_home,planet_Dest,ref,rocket,ter);
};
