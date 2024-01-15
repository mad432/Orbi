#include "filesave.h"
#include <fstream>
#include <iostream>


Filesave::Filesave()
{
    dir = "Saves/";
}

void Filesave::Write_system(std::string name){

    std::ofstream outputFile(dir + name + ".txt");

    if (outputFile.is_open()){

        std::cout<<"Writing savefile"<<std::endl;

        outputFile<<system->getG()<<'\n';

        for(auto par : system->Getparticles()){

            outputFile <<par->object()<<":";

            if(par->object() == "Particle"){

                outputFile << write_particle(par);

            }else if(par->object() == "Rocket"){

                outputFile << write_rocket(par);

            }

            outputFile <<std::endl;

        }
        for(auto plan : *system->get_flights()){

            outputFile <<"Flight"<<":";

            outputFile << write_flight_plan(*plan);

            outputFile << std::endl;

        }

        outputFile.close();
    }

}

std::vector<Particle *> Filesave::Read_system(std::string name){

    std::vector<Particle *> ret;

    try{

        std::string line;

        std::ifstream myfile (dir + name + ".txt");

        if (myfile.is_open()){

            getline(myfile,line,'\n');

            system->setG(stold(line) / 2);

            while (getline(myfile,line, '\n') ){

                if(line.find("Particle") == 0){

                    line = line.substr(9, line.find("Particle") - 9 );

                    //ret.push_back(system->addParticle(read_particle(line)));
                    std::cout<<line<<std::endl;

                    ret.push_back(read_particle(line));

                }else if(line.find("Rocket") == 0){

                    line = line.substr(7, line.find("Rocket") - 7 );

                    ret.push_back(read_rocket(line));

                }else if(line.find("Flight") == 0){

                    line = line.substr(7, line.find("Flight") - 7);

                    read_flightplan(line);


                }

            }

            myfile.close();

        }else{

            std::cout<<"file not found";

        }
    }catch(...){

        std::cout<<"file corrupted"<<std::endl;

    }

    return ret;

}
//int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed

std::string Filesave::write_particle(Particle *par){

    std::string mass = std::to_string(par->Getmass());

    std::string x = std::to_string(par->getx());

    std::string y = std::to_string(par->gety());

    std::string vx = std::to_string(par->getvx());

    std::string vy = std::to_string(par->getvy());

    std::string fix = std::to_string(par->getfix());

    std::string id = std::to_string(par->getid());

    return mass + ',' + x + ',' + y + ',' + vx + ',' + vy + ',' + fix + ',' + id;

}

long double Filesave::F_string(std::string val){

    std::cout<<val<<std::endl;

    if(val.find("*") != val.npos){

        std::string val1 = val.substr( 0 , val.find("*"));

        std::string val2 = val.substr( val.find("*") + 1 , val.back());

        return F_string(val1) * F_string(val2);
    }

    if(val.find("/") != val.npos){

        std::string val1 = val.substr( 0 , val.find("/"));

        std::string val2 = val.substr( val.find("/") + 1 , val.back());

        return F_string(val1) / F_string(val2);
    }
    if(val.find("+") != val.npos){

        std::string val1 = val.substr( 0 , val.find("+"));

        std::cout<<"first val : "<<val1<<std::endl;

        std::string val2 = val.substr( val.find("+") + 1, val.back());

        return F_string(val1) + F_string(val2);
    }
    if(val.find("sqrt(") == 0){

        val = val.substr( 5 , val.find(")") - 5);

        return sqrt(F_string(val));

    }
    if(val.find("sin(") == 0){

        val = val.substr( 4 , val.find(")") - 4);

        return sin(F_string(val));

    }

    if(val.find("cos(") == 0){

        val = val.substr( 4 , val.find(")") - 4);

        return cos(F_string(val));

    }
    if(val.find("rand(") == 0){

        val = val.substr( 5 , val.find(")") - 5);

        std::cout<<val<<std::endl;

        return rand()%int(F_string(val));

    }

    return stold(val);

}

Particle * Filesave::read_particle(std::string par_str){

    size_t pos = 0;

    std::string token = par_str.substr(pos, par_str.find(','));

    int mass = F_string(token);

    token.erase();

    par_str.erase(pos , par_str.find(',') + 1);

    token = par_str.substr(pos, par_str.find(','));

    long double x = F_string(token);

    token.erase();

    par_str.erase(pos , par_str.find(',') + 1);

    token = par_str.substr(pos, par_str.find(','));

    long double y = F_string(token);

    token.erase();

    par_str.erase(pos , par_str.find(',') + 1);

    token = par_str.substr(pos, par_str.find(','));

    long double vx = F_string(token);

    token.erase();

    par_str.erase(pos , par_str.find(',') + 1);

    token = par_str.substr(pos, par_str.find(','));

    long double vy = F_string(token);

    token.erase();

    par_str.erase(pos , par_str.find(',') + 1);

    token = par_str.substr(pos, par_str.find(','));

    int fixed = stoi(token);

    return system->addParticle(mass, x, y , vx, vy, fixed);

}

Rocket * Filesave::read_rocket(std::string par_str){

    size_t pos = 0;

    std::string token = par_str.substr(pos, par_str.find(','));

    int mass = stoi(token);

    token.erase();

    par_str.erase(pos , par_str.find(',') + 1);

    token = par_str.substr(pos, par_str.find(','));

    long double x = F_string(token);

    token.erase();

    par_str.erase(pos , par_str.find(',') + 1);

    token = par_str.substr(pos, par_str.find(','));

    long double y = F_string(token);

    token.erase();

    par_str.erase(pos , par_str.find(',') + 1);

    token = par_str.substr(pos, par_str.find(','));

    long double vx = F_string(token);

    token.erase();

    par_str.erase(pos , par_str.find(',') + 1);

    token = par_str.substr(pos, par_str.find(','));

    long double vy = F_string(token);

    token.erase();

    par_str.erase(pos , par_str.find(',') + 1);

    token = par_str.substr(pos, par_str.find(','));

    int program = stoi(token);

    return system->addRocket(mass, x, y , vx, vy, program);

}

std::string Filesave::write_rocket(Particle * par){

    std::string mass = std::to_string(par->Getmass());

    std::string x = std::to_string(par->getx());

    std::string y = std::to_string(par->gety());

    std::string vx = std::to_string(par->getvx());

    std::string vy = std::to_string(par->getvy());

    std::string fix = std::to_string(par->getfix());

    std::string id = std::to_string(par->getid());

    return mass + ',' + x + ',' + y + ',' + vx + ',' + vy + ',' + fix + ',' + id;


}

//(Rocket * _cur , double G , int program, std::vector <Particle *> references_ , int stage_)

std::string Filesave::write_flight_plan(Flight_plan flight){

    std::string rock = std::to_string(flight.get_rocket()->getid());

    std::string program = std::to_string(flight.get_plan());

    std::string references = "";

    for(auto par : flight.get_references()){

        references += std::to_string(par->getid()) + ":";

    }

    std::string stage = std::to_string(flight.get_stage());

    return rock + ',' + program + ',' + references + ',' + stage;
};

void Filesave::read_flightplan(std::string line){

    size_t pos = 0;

    std::string token = line.substr(pos, line.find(','));

    int rock = stoi(token);

    token.erase();

    line.erase(pos , line.find(',') + 1);

    token = line.substr(pos, line.find(','));

    int program = stoi(token);

    token.erase();

    line.erase(pos , line.find(',') + 1);

    token = line.substr(pos, line.find(','));

    std::vector<int> references = {};

    int count = 0;

    for(int i = 0 ; i < token.length() ; i++){

        if(token[i] == ':'){

            count++;

        }
    }

    std::vector <Particle*> ref = {};

    for(int i = 0; i < count; i++){

        token = line.substr(pos, line.find(':'));

        ref.push_back(system->Getparticles()[stoi(token)]);

        line.erase(pos , line.find(':') + 1);

    }

    line.erase(pos,pos+1);

    token = line.substr(pos , line.find(','));

    int stage = stoi(token);

    Particle * rocky = system->Getparticles()[rock];

    Rocket * Rockey = dynamic_cast<Rocket*>(rocky);

    system->add_flight(Rockey,program,ref,stage);
}
