#ifndef FLIGHT_PLAN_H
#define FLIGHT_PLAN_H
#include "rocket.h"
#include <thread>



class Flight_plan
{
public:
    ~Flight_plan();

    Flight_plan(int _cur , double G , int program, std::vector <Particle *> references_ , int stage_);

    void inctime(int T){time += T;};

    void setrefernces(std::vector <Particle *> references_){references = references_;};

    std::vector <Particle *> get_references(){return references;}

    void terminate();

    bool get_ter(){return terminated;}

    int get_plan(){return program;}

    Rocket * get_rocket(){return current(&references,&terminated);}

    int get_stage(){return stage;}

private:
    static void circularize(int planet_, std::vector<Particle*>* ref, int rocket, bool *ter ,int altitude);

    int program;

    bool terminated;

    static void hohmann_transfer(int planet_, std::vector<Particle*>* , int rocket , bool *ter);

    static void Inter_planet(int planet_, int planet_home, std::vector<Particle*>* , int rocket , bool *ter);

    static int stage;

    static Particle * planet(int pla, std::vector<Particle*>* ref , bool *ter);

    static int rockangle(std::vector<Particle*>* ref, bool *ter);

    static void spin(double amount,std::vector<Particle*>* ref, bool *ter);

    static void program_sel(int program ,std::vector<Particle*>* ref, int rocket, bool *ter, Flight_plan* here);

    static void burn(double dv,std::vector<Particle*>* ref, bool *ter);
    static void burn(double dv,int planet,std::vector<Particle*>* ref, bool *ter);
    static void burn(double dv,int planet_,std::vector<Particle*>* ref,bool *ter , std::string heading);

    static void Capture(int planet_, std::vector<Particle*>* ref, int rocket, bool *ter);

    static void intercept(int planet_, std::vector<Particle*>* ref, int rocket, bool *ter);

    static int abs_ang(int angle);

    static Rocket * current(std::vector<Particle*>* ref,bool *ter);

    //static int rocket;

    static double G;

    static double angle(int a, int b, int c,std::vector<Particle*>* ref, bool *ter);

    int time;

    static void wait(int time);

    std::thread * worker;

    static void setheading(int angle,std::vector<Particle*>* ref, bool *ter);
    static void setheading(std::string dir, int planet,std::vector<Particle*>* ref, bool *ter);

    static double distance(int planet_1 , int planet_2, std::vector<Particle*>* ref, bool *ter);

    std::vector <Particle *> references;

    static double periapsis(int planet_, std::vector<Particle*>* ref, bool *ter);

    static double Apoapsis(int planet_, std::vector<Particle*>* ref, bool *ter);

    static double true_anomaly(int planet_, std::vector<Particle*>* ref, bool *ter);

    static double eccentricity(int planet_, std::vector<Particle*>* ref, bool *ter);

    static int escape_angle(int planet_,std::vector<Particle*>* ref, bool *ter ,double vel);



//    void burn(int dv);
};

#endif // FLIGHT_PLAN_H
