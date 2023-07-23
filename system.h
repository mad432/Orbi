#ifndef SYSTEM_H
#define SYSTEM_H
#include <vector>
#include "Particle.h"


struct cords{
    long double x;
    long double y;
};

class System
{
public:
    static System& getInstance()
            {
                static System    instance; 
                                      
                return instance;
            }

    System(System const&)               = delete;
    void operator=(System const&)  = delete;

    System();

    static Particle* addParticle(Particle* par);

    static Particle* addParticle(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed);

    static void Remove(int id, std::vector <Particle *> input);

    int getSize(){return *size;};

    void setG(double G_){*g = G_;};

    static void clear();

    bool process();

    static void Idreset();

    void setStep(double step_){*step = step_;};

    std::vector <Particle *> Getparticles(){return *particles;};

    static float *col_threshold;

    static int *num_col_particles;


private:

    //int tickcount=0;

    static bool* beencol;

    static double* step;

    static bool update(int start, int end);

    static double* g;

    //System();

    //System(System const&);

    //void operator=(System const&);

    static cords gravity1(double par1x , double par1y , double par2x , double par2y , double m1, double m2);

    static void collision(Particle* par, Particle* par1);

    static std::vector <Particle *>* particles;

    int threads = 16;

    static int* size;

};

#endif // SYSTEM_H
