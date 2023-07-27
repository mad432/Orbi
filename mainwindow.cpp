#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>
#include <QApplication>
#include "Particle.cpp"
#include <math.h>
#include <vector>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QGraphicsView * view = ui->Gameview;

    scene = new QGraphicsScene;

    view->setScene(scene);

    this->scene->addLine(0,0,0,700);//Keep the screen from shifting

    this->scene->addLine(0,0,1425,0);

    qApp->processEvents();

    scene->clear();

    //on_TimeSlider_valueChanged(10);

    QObject::connect(timer1, SIGNAL(timeout()), this, SLOT(timetick()));

    timer1->start(0);

    Sysfactory(1);

}

void MainWindow::pause(){// stops the signal from the timer

    if(this->timer1->signalsBlocked()){
        this->timer1->blockSignals(false);
    }else{
        this->timer1->blockSignals(true);
    }

}

void MainWindow::timetick(){
    //happens every tick


    timer->stop();

    if (system->process()){// calculates the particle movements
        //executs if there has been a collision in the system
        for (auto &par : system->Getparticles()){
            if(par->scene() == nullptr){

                scene->addItem(par);

            }
            if(par->getcol()){//removes particles that have collided

                //std::cout<<"here";

                system->Remove(par->getid(),system->Getparticles());

                delete par;

            }

            //qApp->processEvents();

        }

    }

    scene->update();

    timer->start(0);

}

void MainWindow::addParticle(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed){

    scene->addItem(system->addParticle(Mass, _x,_y , _vx, _vy, fixed));

}

void MainWindow::Sysfactory(int sel){
    //factory to set up various different systems 

    on_pushButton_clicked();

    if(sel == 0){//Earth-Moon
        on_horizontalSlider_valueChanged(90);

        addParticle(5000,1425/2 ,700/2 ,0,0,1);

        addParticle(1000,1425/2 ,700/8 ,28,0,0);

        addParticle(50,1425/2 ,700/8+20 ,-30,0,0);


    }else if(sel == 1){//binary

        on_horizontalSlider_valueChanged(60);

        addParticle(4000, 1425/2 , 700/2+70,25,0,0);

        addParticle(5000, 1425/2 , 700/2-70 , -20,0,0);

        addParticle(50, 1425/2 , 700/2-105, -90,0,0);

        addParticle(75, 1425/2 , 700/2+125, -20,0,0);

        addParticle(100, 1425/2 , 700/10, 35, 0, 0);

        addParticle(10, 1425/2, 700/11 - 8, 20 , 2, 0);

    }else if(sel == 2){//binary-binary

        on_horizontalSlider_valueChanged(90);

        addParticle(3000, 2 * 1425/6, 700/2 + 45, 32, 22, 0);

        addParticle(20, 2 * 1425/6 , 700/2 + 20, 120, 12, 0);

        addParticle(3000 , 2 * 1425/6, 700/2 - 45, -32, 22, 0);

        addParticle(3000,4*1425/6, 700/2 - 45, -30, -22, 0);

        addParticle(20, 4 * 1425/6, 700/2 - 20, -120, - 12, 0);

        addParticle(3000, 4 * 1425/6, 700/2 + 45, 30, -22, 0);

        //addParticle(50, 1425/2, 700/2, 0, 0, 0);



    }else if(sel == 3){//rings

        on_horizontalSlider_valueChanged(40);

        addParticle(5000,1425/2 ,700/2 , 0, 0, 1);

        for(double i = 0; i<250 ;i++){

            double rinx = sin(i/20*acos(0));

            double riny = cos(i/20*acos(0));

            addParticle(4, 1425/2 + rinx*270 + rand()%20 - 10  , 700/2+riny*270+rand()%20 - 10 ,-riny*17.5+rand()%1,rinx*17.5+rand()%1,0);

            addParticle(4, 1425/2 + rinx*250 + rand()%20 - 10 , 700/2+riny*250+rand()%20 - 10 ,-riny*18+rand()%1,rinx*18+rand()%1,0);

            addParticle(4, 1425/2 + rinx*230 + rand()%20 - 10 , 700/2+riny*230+rand()%20 - 10 ,-riny*18.5+rand()%1,rinx*18.5+rand()%1,0);

        }
    }else if(sel == 4){//Moonception

        on_horizontalSlider_valueChanged(90);

        addParticle(2900, 1425/2, 700/2,0,0,1);

        addParticle(1000, 1425/2,700/8,20,0,0);

        addParticle(200,1425/2+60,700/8,20,34,0);

        addParticle(20,1425/2+60,700/8 + 8,-22,37,0);

    }else if (sel == 5){//random

        on_horizontalSlider_valueChanged(70);

        int num = 800+rand()%400;

        for(int i = 0 ; i < num; i++){

            int rmass = rand()%20+5;

            int rxp = rand()%1425;

            int ryp = rand()%700;

            int rxv = rand()%20-10;

            int ryv = rand()%20-10;

            addParticle(rmass,rxp,ryp,rxv,ryv,0);

            }

    }else if (sel == 6){//random twist

        on_horizontalSlider_valueChanged(70);

        int num = 800+rand()%400;

        for(int i = 0 ; i < num; i++){

            int rmass = rand()%20+5;

            int rxp = rand()%1425;

            int ryp = rand()%700;

            int rxv = rand()%20-10;

            int ryv = rand()%20-10+(700/2-rxp*.5)*.08;

            addParticle(rmass,rxp,ryp,rxv,ryv,0);
         }
    }else if (sel == 7) {//performance test
            on_horizontalSlider_valueChanged(90);

            int num = 10000+rand()%400;

            for(int i = 0 ; i < num; i++){

                int rmass = rand()%20+5;

                int rxp = rand()%2000;

                int ryp = rand()%1400;

                int rxv = rand()%20-10;

                int ryv = rand()%20-10+(700/2-rxp*.5)*.08;

                addParticle(rmass,rxp,ryp,rxv,ryv,0);
            }
    }



}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::mousePressEvent(QMouseEvent *event){
    //lets the user add a particle to the interface by first clicking to specify location and then clicking to specify velocity


//   qApp->processEvents();

    //_sleep(10);


    if(*bre1==false){//check if initial click

        *bre1 = true;
        int gx = 0;

        int gy = 0;
        //int x=this->mapFromGlobal(cursor().pos()).x();//initial click cordinates

        QGraphicsView * view = ui->Gameview;

        int yoff = -30;

        int xoff = -10;

        int x = view->mapToScene(event->pos()).x() + xoff;//offset

        int y = view->mapToScene(event->pos()).y() + yoff;

        int sx = this->mapFromGlobal(cursor().pos()).x() + xoff;//startin global x

        int sy = this->mapFromGlobal(cursor().pos()).y() + yoff;


        //Gameview->pos();
        //std::cout<<x<<std::endl;
        QPen pen;//determine how the line is drawen


        pen.setColor(Qt::white);
        pen.setWidth(1);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);

        QGraphicsItem* liny = this->scene->addLine(x,y,x,y,pen);//initializes line drawing


        while(*bre1 == true){
            //int mouseScreen = qApp->desktop()->screenNumber(globalCursorPos);

            //std::cout<< view->mapToGlobal(cursor().pos()).x() - x<<" : "<<std::endl;

            gx =  this->mapFromGlobal(cursor().pos()).x() - sx + x + xoff;//calculate change in global pos then add relative pos
            gy =  this->mapFromGlobal(cursor().pos()).y() - sy + y + yoff;

            //std::cout<<gx<<std::endl;
            this->scene->removeItem(liny);

            liny = this->scene->addLine(x, y, gx, gy, pen);//Redraws the line

            //this->scene->update();

            qApp->processEvents();

            }

        scene->addItem(system->addParticle(*mSlider*100, x, y, (gx - x),(gy - y), fixedcheck));

        this->scene->removeItem(liny);

            //create Particle
        }else{//second click

                *bre1 = false;

        }
    this->scene->update();
}


void MainWindow::on_MassSlider_valueChanged(int value)
{
    *mSlider = value+1;
}

void MainWindow::on_TimeSlider_valueChanged(int value)
{
    *tSlider = value+1;

    system->setStep(.05 * *tSlider);
}

void MainWindow::on_pushButton_clicked()//clear
{
    pause();


    for (auto &par : system->Getparticles()){

        system->Remove(par->getid(),system->Getparticles());

        delete par;

        //qApp->processEvents();

    }
    pause();
    //scene->update();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)//G
{
    g = (value+1) * 0.0014;

    system->System::setG(g);

    this->ui->horizontalSlider->setSliderPosition(value);
}

void MainWindow::on_checkBox_clicked(bool checked)//fixed
{
    fixedcheck = checked;
}



void MainWindow::on_earthmooon_triggered()
{
    Sysfactory(0);
}

void MainWindow::on_actionBinary_triggered()
{
    Sysfactory(1);
}

void MainWindow::on_actionBinary_Binary_triggered()
{
    Sysfactory(2);
}

void MainWindow::on_actionRings_triggered()
{
    Sysfactory(3);
}

void MainWindow::on_pushButton_2_pressed()
{
    pause();
}


void MainWindow::on_actionInner_Solar_System_triggered()//Moonception
{
    Sysfactory(4);
}

void MainWindow::on_Refresh_clicked()
{
    pause();
}

void MainWindow::on_actionRandom_triggered()
{
    Sysfactory(5);
}

void MainWindow::on_actionRandom_Twist_triggered()
{
    Sysfactory(6);
}

