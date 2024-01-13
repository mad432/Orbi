#ifndef FLIGHT_PLAN_H
#define FLIGHT_PLAN_H
#include "rocket.h"
#include <thread>



class Flight_plan
{
public:
    ~Flight_plan();

    Flight_plan(Rocket * _cur , double G , int program, std::vector <Particle *> references_ , int stage_);

    void inctime(int T){time += T;};

    void setrefernces(std::vector <Particle *> references_){references = references_;};

    std::vector <Particle *> get_references(){return references;}

    void terminate();

    int get_plan(){return program;}

    int get_rocket(){return current()->getid();}

    int get_stage(){return stage;}

private:

    int program;

    static bool terminated;

    static void hohmann_transfer(int planet_);

    static int stage;

    static Particle * planet(int pla);

    static int rockangle();

    static void spin(double amount);

    static void program_sel(int program);

    static void burn(double dv);
    static void burn(double dv,int planet);

    static int abs_ang(int angle);

    static Rocket * current();

    static int rocket;

    static double G;

    static double angle(int a, int b, int c);

    int time;

    static void wait(int time);
    
    static std::thread * worker;

    static void setheading(int angle);
    static void setheading(std::string dir, int planet);

    static double distance(int planet_1 , int planet_2);

    static std::vector <Particle *> references;

    static double off_set(int planet_);


//    void burn(int dv);
};

#endif // FLIGHT_PLAN_H
