#ifndef FILESAVE_H
#define FILESAVE_H
#include "system.h"
#include "Particle.h"
#include "flight_plan.h"

class Filesave
{
public:
    Filesave();

    void Write_system(std::string name);

    std::vector<Particle *> Read_system(std::string name);

    Particle * read_particle(std::string par_str);

    std::string write_particle(Particle * par);


private:

    std::string write_flight_plan(Flight_plan flight);

    void read_flightplan(std::string line);

    long double F_string(std::string val);

    std::string dir;

    //std::string write_particle(Particle * par);

    std::string write_rocket(Particle * rock);

    System * system;

    Rocket * read_rocket(std::string rock_str);

};

#endif // FILESAVE_H
