#ifndef SYSTEM_H
#define SYSTEM_H
#include <vector>
#include "Particle.h"
#include <stdio.h>
#include <stdlib.h>
#include <CL/opencl.h>
#include <CL/cl.hpp>

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

   static cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer> gravity;
   static cl::Buffer A_d;//Kernal input arrays
   static cl::Buffer B_d;
   static cl::Buffer C_d;
   static cl::Buffer D_d;
   static cl::Buffer E_d;
   static cl::Buffer F_d;
   static cl::Buffer G_d;
   static cl::Buffer H_d;
   static cl::Buffer I_d;
   static cl::Buffer J_d;
   //static cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer>;
   static cl::Program program;
   static cl::Program::Sources sources;
   static cl::CommandQueue queue;

   static cl::Device default_device;

   static cl::Context context;

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
