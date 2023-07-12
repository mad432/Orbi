#include "system.h"
#include <iostream>
#include <thread>
#define _USE_MATH_DEFINES
#include <math.h>
#include <mutex>
//#include <stdio.h>
//#include <stdlib.h>
//#include <CL/opencl.h>
//#include <CL/cl.hpp>
//#include <CL/cl.h>



std::mutex myMutex;

double step_ = 1;

double* System::step = &step_;

double g_ = .001;

double* System::g = &g_;

int size_ = 0;

int* System::size = &size_;

bool beencol_ = false;

bool* System::beencol = &beencol_;

float col_threshold_ = 50;

float* System::col_threshold = &col_threshold_;

int num_col_particles_ = 2;

int* System::num_col_particles = &num_col_particles_;

std::vector <Particle *> particles_;

std::vector <Particle *>* System::particles = &particles_;



std::string kernel_code =
"        void kernel gravity(global const double* par1x, global const double* par1y, global const double* par2x, global const double* par2y, global const double* m1, global const double* m2, global const int* step, global const double* g, global double* forcex, global double* forcey){"

"                       double dist = sqrt(pow(par1x[get_global_id(0)] - par2x[get_global_id(0)] , 2) + pow(par1y[get_global_id(0)] - par2y[get_global_id(0)] , 2));"//calculates distance
""
"                       double grav = -(g[get_global_id(0)] * m1[get_global_id(0)] * m2[get_global_id(0)]) / pow(dist , 2);"//calculates gravitational force
""
"                       forcex[get_global_id(0)] = step[get_global_id(0)] * (g[get_global_id(0)] / 0.0048)  * grav * (par1x[get_global_id(0)] - par2x[get_global_id(0)]) / dist;"// calculates x component of acceleration due to gravity
"                       forcey[get_global_id(0)] = step[get_global_id(0)] * (g[get_global_id(0)] / 0.0048)  * grav * (par1y[get_global_id(0)] - par2y[get_global_id(0)]) / dist;"// y component of gravity
"          }";

System::System()
{


//    std::vector<cl::Platform> all_platforms;//opencl stuff
//    cl::Platform::get(&all_platforms);
//    if (all_platforms.size() == 0) {
//        std::cout << " No OpenCL platforms found.\n";
//        exit(1);
//    }

//    //We are going to use the platform of id == 0
//    static cl::Platform default_platform = all_platforms[0];
//    std::cout << "Using platform: " <<default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";



//    std::vector<cl::Device> all_devices;
//    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
//    if (all_devices.size() == 0) {
//        std::cout << " No devices found.\n";
//        exit(1);
//    }

//    static cl::Device default_device = all_devices[0];
//    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";

//    cl::Context context({ default_device });

//    cont = &context;


//    // create buffers on the device
//    cl::Buffer A_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10);
//    cl::Buffer B_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10);
//    cl::Buffer C_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10);
//    cl::Buffer D_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10);
//    cl::Buffer E_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10);
//    cl::Buffer F_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10);
//    cl::Buffer G_d(context, CL_MEM_READ_ONLY, sizeof(int) * 10);
//    cl::Buffer H_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10);
//    cl::Buffer I_d(context, CL_MEM_WRITE_ONLY, sizeof(double) * 10);
//    cl::Buffer J_d(context, CL_MEM_WRITE_ONLY, sizeof(double) * 10);
//    std::vector <double> thing;
//    thing.push_back(5);
//    thing.push_back(5);
//    thing.push_back(5);
//    thing.push_back(5);
//    thing.push_back(5);
//    thing.push_back(5);
//    thing.push_back(5);
//    thing.push_back(5);
//    thing.push_back(5);
//    thing.push_back(50);
//    int k = 10;


//    double A_h[] = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    double* B_h=new double[thing.size()];
//    //double B_h[] = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    for(int i=0;i<thing.size();i++){
//        B_h[i]=thing[i];
//    }


//    double C_h[] = { 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000 };

//    double D_h[] = { 1000, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

//    double E_h[] = { 1000, 1000, 2000, 3000, 400, 5000, 6000, 700, 800, 9000 };

//    double F_h[] = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

//    int G_h[] = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

//    double H_h[] = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //std::cout<<"B_h:"<<B_h[0]<<std::endl;

//    //create queue to push commands to the device.
//    cl::CommandQueue queue(context, default_device);

//    queue.enqueueWriteBuffer(A_d, CL_TRUE, 0, sizeof(double) * 10, A_h);
//    queue.enqueueWriteBuffer(B_d, CL_TRUE, 0, sizeof(double) * 10, B_h);
//    queue.enqueueWriteBuffer(C_d, CL_TRUE, 0, sizeof(double) * 10, C_h);
//    queue.enqueueWriteBuffer(D_d, CL_TRUE, 0, sizeof(double) * 10, D_h);
//    queue.enqueueWriteBuffer(E_d, CL_TRUE, 0, sizeof(double) * 10, E_h);
//    queue.enqueueWriteBuffer(F_d, CL_TRUE, 0, sizeof(double) * 10, F_h);
//    queue.enqueueWriteBuffer(G_d, CL_TRUE, 0, sizeof(int) * 10, G_h);
//    queue.enqueueWriteBuffer(H_d, CL_TRUE, 0, sizeof(double) * 10, H_h);

//    cl::Program::Sources sources;

//    //Appending the kernel, which is presented here as a string.
//    sources.push_back({ kernel_code.c_str(),kernel_code.length() });

//    cl::Program program(context, sources);


//    if (program.build({ default_device }) != CL_SUCCESS) {
//        std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
//        exit(1);
//    }

//    cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer> gravity(cl::Kernel(program, "gravity"));

//    //Details to enqueue the kernel for execution.
//    cl::NDRange global(10);
//    gravity(cl::EnqueueArgs(queue, global), A_d, B_d, C_d, D_d, E_d, F_d , G_d, H_d, I_d, J_d).wait();

//    double I_h[10];
//    //read result C_d from the device to array C_h
//    queue.enqueueReadBuffer(I_d, CL_TRUE, 0, sizeof(double) * 10, I_h);

//    std::cout << " result: \n";
//    for (int i = 0; i<10; i++) {
//        std::cout << I_h[i] << " ";
//    }
//    //delete[] B_h;

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

void System::collision(Particle* par, Particle* par1){// collision between particles
    //std::cout<<par->getid()<<std::endl;

    if(!par->getcol() && !par1->getcol()){
        par->hascol();
        par1->hascol();

        double posx = (par1->getx()*par1->Getmass()+par->getx()*par->Getmass())/(par->Getmass()+par1->Getmass());// new particle components

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
//        std::cout<<"relmass: "<<par->Getmass()/(par1->Getmass())<<std::endl;


//        std::cout<<"calc : "<</*(abs(par->Getmass() - par1->Getmass()))*/ 100000 / ( 1/rel_speed / ((collision_angle)+10)) / (0.5 + relmass * 4) <<std::endl;


        double breakmass = 0;
        double break_mom_x = 0;//break away particles momentum
        double break_mom_y = 0;
        if((1000) / ( 1/rel_speed * ((collision_angle+20))+1) * (0.1 + relmass * 0.5)  > *col_threshold && rel_speed > 20){//determines if any new particles will be created
            //std::cout<<"new Particles"<<std::endl;
            double breaksize;
            double breakvelx;
            double breakvely;

            if(par->Getmass()>par1->Getmass()){//determines the smallest particle
                breaksize = (par1->Getmass()/5);//breakaway mass

            }else{
                breaksize = (par->Getmass()/5);

            }if (breaksize>2){
                bool side = true;
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

    ret.x = *step * (*g / 0.0048)  *grav * (par1x - par2x) / dist;// calculates x component of acceleation due to gravity
    ret.y = *step * (*g / 0.0048)  *grav * (par1y - par2y) / dist;// y component of gravity


//    std::cout<<ret.y<<":";
//    std::cout<<ret.x<<std::endl;
    return ret;

}




bool System::process(){
    //allocates threads to particles

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

    if(*beencol){
        const std::vector <Particle *> hold = *particles;
        for(auto &par : *particles){
            //par->hascol();
            if(par->getcolnum() != -1 && !par->getcol()){
                for(auto &par1 : *particles){
                    if(par->getcolnum()==par1->getid()){
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
    //takes the first par id to compute to the last particle to compute motion
    //returns if a collision has occured

    bool col = false; //if collision

    const std::vector <Particle *> hold = *particles;

    double stepfactor = .0005;

//    static std::vector<cl::Platform> all_platforms;
//    cl::Platform::get(&all_platforms);
//    if (all_platforms.size() == 0) {
//        std::cout << " No OpenCL platforms found.\n";
//        exit(1);
//    }

//    //We are going to use the platform of id == 0
//    static cl::Platform default_platform = all_platforms[0];
//    //std::cout << "Using platform: " <<default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";



//    static std::vector<cl::Device> all_devices;
//    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
//    if (all_devices.size() == 0) {
//        std::cout << " No devices found.\n";
//        exit(1);
//    }

//    static cl::Device default_device = all_devices[0];
//    //std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";

//    static cl::Context context({ default_device });
//    //cl::Context context = *cont;


//    cl::Buffer A_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10000);
//    cl::Buffer B_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10000);
//    cl::Buffer C_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10000);
//    cl::Buffer D_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10000);
//    cl::Buffer E_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10000);
//    cl::Buffer F_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10000);
//    cl::Buffer G_d(context, CL_MEM_READ_ONLY, sizeof(int) * 10000);
//    cl::Buffer H_d(context, CL_MEM_READ_ONLY, sizeof(double) * 10000);
//    cl::Buffer I_d(context, CL_MEM_WRITE_ONLY, sizeof(double) * 10000);
//    cl::Buffer J_d(context, CL_MEM_WRITE_ONLY, sizeof(double) * 10000);

//    static cl::CommandQueue queue(context, default_device);

//    cl::Program::Sources sources;
//    sources.push_back({ kernel_code.c_str(),kernel_code.length() });

//    //OpenCL compiles the kernel in runtime, that's the reason it is expressed as a string.
//    //There are also ways to compile the device-side code offline.
//    cl::Program program(context, sources);

//    cl::NDRange global(*size);


    //cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer> gravity(cl::Kernel(program, "gravity"));






    for (int i = start ; i < end ; i++){

        Particle* par = hold[i];

        if(par->getfix() == false){

            par->setx(par->getx() + par->getvx() **step * stepfactor);//move particle

            par->sety(par->gety() + par->getvy() **step * stepfactor);


//            int j = 0;
//            double* parx=new double[*size ];//opencl stuff
//            double* pary=new double[*size ];
//            double* parvx=new double[*size ];
//            double* parvy=new double[*size ];
//            double* parm=new double[*size ];
            for(auto &par1 : hold){
                if(par1->getid() != par->getid() && par1->getcolnum() == -1){

                    if((par->getsize() + par1->getsize()) / 2 > sqrt(pow(par->getx() - par1->getx(),2) + pow(par->gety() - par1->gety() , 2))){//check for collision

                        if(!par->getcol() && !par1->getcol()){//check to see if a particle has already collided

                            //par->hascol();

                            par->setcol(par1->getid());

                            //par1->hascol();

                            par1->setcol(par->getid());

                        }
                        *beencol = true;

                        //goto end;
                        //break;

                    }else{
//                        parx[j] = par1->getx();//opencl stuff
//                        pary[j] = par1->gety();
//                        parvy[j] = par1->getvy();
//                        parvx[j] = par1->getvx();
//                        parm[j] = par1->Getmass();




                        cords k1 = gravity1(par->getx() , par->gety() , par1->getx(), par1->gety(), par->Getmass(), par1->Getmass());// Runge - Kutta calculate force

                        cords k2 = gravity1(par->getx() + k1.x/2**step * stepfactor , par->gety() + k1.y/2**step * stepfactor , par1->getx() - k1.x/2**step * stepfactor , par1->gety() -k1.y/2**step * stepfactor, par->Getmass(), par1->Getmass());

                        cords k3 = gravity1(par->getx() + k2.x/2**step * stepfactor , par->gety() + k2.y/2**step * stepfactor ,  par1->getx() - k1.x/2**step * stepfactor , par1->gety() -k1.y/2**step * stepfactor , par->Getmass(), par1->Getmass());

                        cords k4 = gravity1(par->getx() + k3.x**step * stepfactor , par->gety() + k3.y**step * stepfactor ,  par1->getx() - k1.x**step * stepfactor , par1->gety() -k1.y**step * stepfactor , par->Getmass(), par1->Getmass());

                        par->setvx(par->getvx() + (k1.x + 2*k2.x + 2*k3.x + k4.x)/6/par->Getmass());

                        par->setvy(par->getvy() + (k1.y + 2*k2.y + 2*k3.y + k4.y)/6/par->Getmass());

                        par1->setvx(par1->getvx() - (k1.x + 2*k2.x + 2*k3.x + k4.x)/6/par1->Getmass());

                        par1->setvy(par1->getvy() - (k1.y + 2*k2.y + 2*k3.y + k4.y)/6/par1->Getmass());

                    }                    //j++;

                }
            }

//            double * cparx=new double[*size];//  OPENCL currently kindofworking though 1)slower 2) crashes with large particle count
//            double * cpary=new double[*size];
//            double * cparm=new double[*size];
//            int * cstep=new int[*size];
//            double * cg = new double[*size];
//            for(int j = 0;j<*size;j++){
//                cparx[j]=par->getx();
//                cpary[j]=par->gety();
//                cparm[j]=par->Getmass();
//                cstep[j]=*step;
//                cg[j]=*g;
//            }
//            myMutex.lock();

//            //std::cout<<"B_h:"<<B_h[0]<<std::endl;


//            queue.enqueueWriteBuffer(A_d, CL_TRUE, 0, sizeof(double) * *size, cparx);
//            queue.enqueueWriteBuffer(B_d, CL_TRUE, 0, sizeof(double) * *size, cpary);
//            queue.enqueueWriteBuffer(C_d, CL_TRUE, 0, sizeof(double) * *size, parx);
//            queue.enqueueWriteBuffer(D_d, CL_TRUE, 0, sizeof(double) * *size, pary);
//            queue.enqueueWriteBuffer(E_d, CL_TRUE, 0, sizeof(double) * *size, parm);
//            queue.enqueueWriteBuffer(F_d, CL_TRUE, 0, sizeof(double) * *size, cparm);
//            queue.enqueueWriteBuffer(G_d, CL_TRUE, 0, sizeof(int) * *size, cstep);
//            queue.enqueueWriteBuffer(H_d, CL_TRUE, 0, sizeof(double) * *size, cg);

//            if (program.build({ default_device }) != CL_SUCCESS) {
//                std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
//                exit(1);
//            }

//            cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer> gravity(cl::Kernel(program, "gravity"));

//            //Details to enqueue the kernel for execution.
//            //cl::NDRange global(*size);
//            gravity(cl::EnqueueArgs(queue, global), A_d, B_d, C_d, D_d, E_d, F_d , G_d, H_d, I_d, J_d).wait();

//            double* I_h = new double[*size];
//            queue.enqueueReadBuffer(I_d, CL_TRUE, 0, sizeof(double) * *size, I_h);
//            double* J_h = new double[*size];
//            queue.enqueueReadBuffer(J_d, CL_TRUE, 0, sizeof(double) * *size, J_h);
//            myMutex.unlock();

//            //std::cout << " result: \n";
//            for (int i = 0; i<*size-1; i++) {
//                par->setvx(par->getvx()+I_h[i]/par->Getmass()*2);
//                par->setvy(par->getvy()+J_h[i]/par->Getmass()*2       );
//            }
//           //std::cout<<std::endl;



//            delete [] cg;
//            delete [] cstep;
//            delete [] cparx;
//            delete [] cpary;
//            delete [] cparm;

//            end:

//            delete[] parx;
//            delete[] pary;
//            delete[] parvx;
//            delete[] parvy;
//            delete[] parm;





        }


    }
//    end:;

    return col;
}
