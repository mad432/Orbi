#ifndef FLIGHT_PLAN_H
#define FLIGHT_PLAN_H
#include "rocket.h"
#include "system.h"
#include <thread>



class Flight_plan
{
public:
    ~Flight_plan();

    Flight_plan(Rocket * _cur , System * _sys, int program, std::vector <Particle *> references_);

    void inctime(int T){time += T;};

    void set_ref(std::vector <Particle *> references_){references = references_;}

private:

    static Particle * planet(int pla);

    static int rockangle();
    static void spin(double amount);

    static void program_sel(int program);

    static void burn(double dv);
    static void burn(double dv,int planet);

    static int abs_ang(int angle);

    static Rocket * current();

    static int rocket;

    static System * Sys;

    static double angle(int a, int b, int c);

    int time;

    int wait(int time);
    
    static std::thread * worker;

    static void setheading(int angle);
    static void setheading(std::string dir, int planet);

    static double distance(int planet_1 , int planet_2);
    static std::vector <Particle *> references;


//    void burn(int dv);
};

#endif // FLIGHT_PLAN_H
