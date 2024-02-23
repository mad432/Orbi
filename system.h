#ifndef SYSTEM_H
#define SYSTEM_H
#include <vector>
#include "flight_plan.h"
#include "Particle.h"
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include "rocket.h"
#include <thread>
#include "flight_plan.h"
#include "quadtree.h"


class System
{
public:

    struct cords{
        double x = 0;
        double y = 0;

        inline const cords& operator +=(const cords& other)  {

            //cords res ;
            x = x+other.x;
            y = y+other.y;

            return *this;
        }
    };


    System(const System& obj) = delete;

    static System* getInstance()//singleton class
      {
        if (Instance == NULL)
        {
          Instance = new System();
          return Instance;
        }
        else
        {
          return Instance;
        }
      }
    ~System();

    void setcollision(bool checked){collisionEnabled = checked;}

    void setDebris(bool checked){DebrisEnabled = checked;}

    static Particle* addParticle(Particle* par);

    static Particle* addParticle(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed);

    static Rocket* addRocket(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed);

    static void Remove(int id);

    int getSize(){return *size;};

    void setG(double G_){*g = G_;};

    double getG(){return *g * 2;};

    static void clear();

    bool process();

    static void Idreset();

    void setStep(double step_){*step = step_;};

    std::vector <Particle *> Getparticles(){return *particles;};

    static int *num_col_particles;

    void setSpecial_rel(bool checked){Special_rel = checked;};
    bool getSpecial_rel(){return Special_rel;};

    void setC(int C_){C = C_;};
    int GetC(){return C;};

    void add_flight(Rocket * _cur, int program, std::vector <Particle *> references_ , int stage_);
    void add_flight(Flight_plan * flight){ flights->push_back(flight) ;}

    std::vector <Flight_plan*>* get_flights(){return flights;};

    static bool barnes_hut;

    double get_step(){return *step;}
    static QuadTree * root;



private:
    static void constree(int start, int end);

    void constructnode(QuadTree * parent,QuadTree * node ,Particle * par);

    static QuadTree _root;
    //static QuadTree * root;

    static std::vector <Flight_plan*> flights_;

    static std::vector <Flight_plan*>* flights;

    static bool collisionEnabled;

    static bool DebrisEnabled;

    static bool Special_rel;

    static double lorentz(double vx, double vy);

    static double ref_frame_vx;

    static double ref_frame_vy;

    static double C;

    const std::mutex myMutex;

    static float col_threshold_;

    static float *col_threshold;

    static double step_;

    static double g_;

    static int size_;

    static bool beencol_;

    static int num_col_particles_;

    static std::vector <Particle *> particles_;

    int tickcount=0;

    static bool* beencol;

    static double* step;

    static bool update(int start, int end);

    static double* g;

    static cords gravity(double par1x , double par1y , double par2x , double par2y , double m1, double m2, double step_size);

    static void collision(Particle* par, Particle* par1);

    static std::vector <Particle *>* particles;

    static const int threads = 16;

    static int* size;

    static System* Instance;

    System();

};

#endif // SYSTEM_H
