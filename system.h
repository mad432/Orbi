#ifndef SYSTEM_H
#define SYSTEM_H
#include <vector>
#include "Particle.h"
#include <stdio.h>
#include <stdlib.h>
#include <mutex>

//#include <CL/opencl.h>
//#include <CL/cl.hpp>

struct cords{
    long double x;
    long double y;
};

class System
{
public:



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
    //System();

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

    static int *num_col_particles;

    void setSpecial_rel(bool checked){Special_rel = checked;};

    void setC(int C_){C = C_;};

private:

    static bool Special_rel;

    static double lorenz(double vx, double vy);

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

    static cords gravity(double par1x , double par1y , double par2x , double par2y , double m1, double m2);

    static void collision(Particle* par, Particle* par1);

    static std::vector <Particle *>* particles;

    int threads = 16;

    static int* size;

    static System* Instance;

    System();

};

#endif // SYSTEM_H
