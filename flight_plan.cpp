#include "flight_plan.h"
#include <iostream>
#include <windows.h>


//Rocket * Flight_plan::current() = nullptr;

System * Flight_plan::Sys = nullptr;

int Flight_plan::rocket = 0;

std::thread * Flight_plan::worker = nullptr;

std::vector <Particle *> Flight_plan::references = {};

Flight_plan::Flight_plan(Rocket * _cur , System * _sys , int program, std::vector <Particle *> references_)
{

    rocket = _cur->getid();

    Sys = _sys;

    references = references_;

    worker = new std::thread(program_sel,program);

    worker->detach();

}

Particle * Flight_plan::planet(int pla){

    void * here = references[pla];

    if (here != nullptr){

        return references[pla];

    }else{

        throw 0;

        return nullptr;

    }

}

Rocket * Flight_plan::current(){

    if(Sys->Getparticles().size()>rocket){

        Particle * here = planet(rocket);

        Rocket * ret_rock =  dynamic_cast<Rocket*>(here);

        if (ret_rock != nullptr){

            return ret_rock;

        }else{

            throw 1;

            return nullptr;

        }
    }else{

        throw 1;

    }
}

Flight_plan::~Flight_plan(){

}

double Flight_plan::distance(int planet_1 , int planet_2){


    return sqrt(pow(planet(planet_1)->getx() - planet(planet_2)->getx(), 2) + pow(planet(planet_1)->gety() - planet(planet_2)->gety(), 2));


}


void Flight_plan::program_sel(int program){
    //selects a flight plan to follow
    Sleep(30);



    if(program == 0){
        while(true){
            Sleep(200);
            std::cout<<rockangle()<<std::endl;
            setheading(10);
        }
    }

    if(program == 1){

        try{

       double h_0 = 0;

       if(planet(0) != nullptr){

           h_0 = distance(rocket, 0);

       }

       double h_p = 0;

       if(planet(1) != nullptr && planet(0) != nullptr){

           h_p = distance(0,1);

       }

       double dv = 0;

       if(planet(0) != nullptr){
           dv = sqrt((planet(0)->Getmass()*Sys->getG() /h_0)) * (sqrt(((2.0 * h_p)/(h_p + h_0))) - 1.0) - 3.0 ;
       }


       int trans_angle = 180 * (1 - ( 1 / (sqrt(8)) * sqrt( pow(h_0/h_p + 1 , 3))));

       std::cout<<"transfer angle : "<<trans_angle<<std::endl<<"Dv required : "<<dv<<std::endl;


       while((trans_angle - 1 > angle(1,0,2) || angle(1,0,2) > trans_angle + 1 || current()->getvx() * (planet(1)->getx()-planet(0)->getx()) + current()->getvy() * (planet(1)->gety()-planet(0)->gety()) < 0) ){


           setheading("prograde" , 0);

       }


       double off = 50;


       burn(dv);

       while(distance(1,rocket) > 50){
           double x = current()->getx() - planet(1)->getx();
           double y = current()->gety() - planet(1)->gety();
           double vx = current()->getvx() - planet(1)->getvx();
           double vy = current()->getvy() - planet(1)->getvy();

           off = (tan(acos((vx * x + vy * y)/(sqrt(pow(vx, 2) + pow(vy, 2)) * sqrt(pow(x, 2) + pow(y, 2))))) * sqrt(pow(x, 2) + pow(y, 2)));

           if((off < 40 && off > 0)||(off < -40)){
              setheading("radial out" , 1);
           }else{
              setheading("radial in" , 1);
           }

           Sleep(10);
       }
       //int F_alt = 0;

       while( (off < 35 || off > 45)){

           double x = current()->getx() - planet(1)->getx();
           double y = current()->gety() - planet(1)->gety();
           double vx = current()->getvx() - planet(1)->getvx();
           double vy = current()->getvy() - planet(1)->getvy();

           off = abs((tan(acos((vx * x + vy * y)/(sqrt(pow(vx, 2) + pow(vy, 2)) * sqrt(pow(x, 2) + pow(y, 2))))) * sqrt(pow(x, 2) + pow(y, 2))));

           //off = (off - Sys->getG() * planet(1)->Getmass() * current()->Getmass())/ pow(off,2);

           Sleep(10);


           burn(0.1,1);

       }

       std::cout<<"Final alt calc: "<<off<<std::endl;




       double d_i = distance(rocket,1);

       while( d_i >= distance(rocket,1)){// wait for periapsis

           d_i = distance(rocket,1);

           Sleep(10);

           setheading("retrograde" , 1);
       }

       dv = sqrt(pow(current()->getvx() - planet(1)->getvx(),2) + pow(current()->getvy() - planet(1)->getvy(),2)) - sqrt((Sys->getG() * planet(1)->Getmass()/distance(rocket , 1))) - 3 ;

       std::cout<<"incertion dv : "<<dv<<std::endl;

       burn(dv, 1);

       std::cout<<"Final alt : "<<distance(rocket,1)<<std::endl;



       }catch(int cat){

            std::cout<<"Flight plan terminated with error code : "<<cat<<std::endl;

       }
    }

    //worker->~thread();
    std::cout<<"flight plan finished"<<std::endl;

};

double Flight_plan::angle(int a, int b, int c){

        double ax = planet(a)->getx();
        double ay = planet(a)->gety();
        double bx = planet(b)->getx();
        double by = planet(b)->gety();
        double cx = planet(c)->getx();
        double cy = planet(c)->gety();

        double vec_abx = (ax - bx);
        double vec_aby = (ay - by);

        double vec_cbx = (cx - bx);
        double vec_cby = (cy - by);

        return acos((vec_cbx * vec_abx + vec_cby * vec_aby)/( sqrt( pow(vec_cbx , 2) + pow(vec_cby , 2)) * sqrt( pow(vec_abx , 2) + pow(vec_aby , 2) ))) * 180 / 3.14;

}

int Flight_plan::abs_ang(int angle){

    int ret_ang = angle % 360;

    if (ret_ang < 0){

        return 360 + ret_ang;

    }

    return ret_ang;

};

int Flight_plan::rockangle(){

    if(current() != nullptr){

        return abs_ang(current()->getheading());

    }else{

       throw 0;

       return 0;

    }
}

void Flight_plan::spin(double amount){

    if(current() != nullptr && current()->object() == "Rocket") {

        current()->changeaV(amount);

     }

}

void Flight_plan::setheading(int angle){
        //sets the rockets heading to angle between 0 and 359 +- 1 degrees
        angle = abs_ang(angle);

        if(angle != rockangle()){

            int quad;//tells which quadrant we are current()ly in and the target quadrant
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

            if (abs_ang(rockangle()) < 90){

                quad = 1;

            }else if(abs_ang(rockangle()) < 180){

                quad = 2;

            }else if(abs_ang(rockangle()) < 270){

                quad = 3;

            }else{

                quad = 4;
            }


             bool side;

             //std::cout<< abs_ang(rockangle()) - angle%180<<std::endl;

             if ((quad == 1 && tar_quad == 4) || (quad == 4 && tar_quad == 3) || (quad == 3 && tar_quad == 2) || (quad == 2 && tar_quad == 1) ||
                 (quad == 1 && tar_quad == 3 && abs_ang(rockangle()) - angle % 180 < 0) || (quad == 4 && tar_quad == 2 && abs_ang(rockangle()) % 270 - angle % 90 < 0) ||
                 (quad == 3 && tar_quad == 1 && abs_ang(rockangle()) % 180 - angle < 0) || (quad == 2 && tar_quad == 4 && abs_ang(rockangle()) % 90 - angle % 270 < 0) ||
                 (quad == tar_quad && angle <= abs_ang(rockangle()))){

                spin(-0.1);

                side = true;

                //std::cout<<"left"<<std::endl;

            }else{

                spin(0.1);

                side = false;
                //std::cout<<"right"<<std::endl;

            }

            while((angle - 2 >= abs_ang(rockangle()) || abs_ang(rockangle()) >= angle + 2) && current() != nullptr){
                Sleep(10);
                //std::cout<<rockangle()%360<<" : "<<angle<<std::endl;
            }

            if(side){

                spin(0.1);

            }else{

                spin(-0.1);

            }
    }


}

void Flight_plan::setheading(std::string dir, int planet_){

    double x = current()->getvx() - planet(planet_)->getvx();

    double y = current()->getvy() - planet(planet_)->getvy();

    if(dir == "prograde"){

       int prograde = - atan((x/y)) * 180 / 3.14;

       if((x <= 0 && y >=0) || (x >= 0 && y >= 0)){

            prograde = - atan((x/y)) * 180 / 3.14 + 180;

        }

        setheading(prograde);

    }else if(dir == "retrograde"){

        int retrograde = - atan((x/y)) * 180 / 3.14 +180;

        if((x <= 0 && y >=0) || (x >= 0 && y >= 0)){

            retrograde = - atan((x/y)) * 180 / 3.14;

        }

        setheading(retrograde);
     }else if(dir == "radial out"){

        int radial =  -atan((x/y)) * 180 / 3.14 + 90;

        if((x <= 0 && y >=0) || (x >= 0 && y >= 0)){

            radial =  -atan((x/y)) * 180 / 3.14 - 90;

        }

        setheading(radial);

      }else if(dir == "radial in"){

        int radial =  -atan((x/y)) * 180 / 3.14 - 90;

        if((x <= 0 && y >=0) || (x >= 0 && y >= 0)){

            radial =  -atan((x/y)) * 180 / 3.14 + 90;

         }

         setheading(radial);

      }
}

void Flight_plan::burn(double dv){

    std::cout<<current()->getvy()<<std::endl;

    double vx_0 = current()->getvx();

    double vy_0 = current()->getvy();

    std::cout<<"Burning"<<std::endl;

    std::cout<<"Target dv : current() dv"<<std::endl;


    while(sqrt(pow(vx_0,2) + pow(vy_0,2)) > sqrt((pow(current()->getvx(),2) + pow(current()->getvy(),2))) - sqrt(dv*dv) && sqrt(pow(vx_0,2) + pow(vy_0,2)) < sqrt((pow(current()->getvx(),2) + pow(current()->getvy(),2))) + sqrt(dv*dv)){


        current()->thrust(50);

        std::cout<<dv<<" : "<<sqrt((pow(current()->getvx(),2) + pow(current()->getvy(),2)))-sqrt(pow(vx_0,2) + pow(vy_0,2))<<std::endl;

        Sleep(10);

    }


    std::cout<<"burn finished  Starting vel:"<<sqrt(pow(vx_0,2) + pow(vy_0,2))<<" : Ending vel:"<<sqrt((pow(current()->getvx(),2) + pow(current()->getvy(),2)))<<std::endl;




};

void Flight_plan::burn(double dv,int planet_){

    std::cout<<current()->getvy()<<std::endl;

    double vx_0 = current()->getvx() - planet(planet_)->getvx();

    double vy_0 = current()->getvy() - planet(planet_)->getvy();

    std::cout<<"Burning"<<std::endl;

    std::cout<<"Target dv : current dv"<<std::endl;


    while(sqrt(pow(vx_0,2) + pow(vy_0,2)) > sqrt((pow(current()->getvx() - planet(planet_)->getvx(),2) + pow(current()->getvy() - planet(planet_)->getvy(),2))) - sqrt(dv*dv) &&
          sqrt(pow(vx_0,2) + pow(vy_0,2)) < sqrt((pow(current()->getvx() - planet(planet_)->getvx(),2) + pow(current()->getvy() - planet(planet_)->getvy(),2))) + sqrt(dv*dv) ){

        current()->thrust(50);

        std::cout<<dv<<" : "<<sqrt((pow(current()->getvx() - planet(planet_)->getvx(),2) + pow(current()->getvy() - planet(planet_)->getvy(),2)))-sqrt(pow(vx_0,2) + pow(vy_0,2))<<std::endl;

        Sleep(10);

    }

    std::cout<<"burn finished  Starting vel:"<<sqrt(pow(vx_0,2) + pow(vy_0,2))<<" : Ending vel:"<<sqrt((pow(current()->getvx() - planet(planet_)->getvx(),2) + pow(current()->getvy() - planet(planet_)->getvy(),2)))<<std::endl;


}
