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
#include "flight_plan.h"
#include <QMediaPlaylist>
#include <QMediaPlayer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    setWindowIcon(QIcon(":Image/thumb.png"));

    srand (time(0));

    ui->setupUi(this);

    QGraphicsView * view = ui->Gameview;

    scene = new QGraphicsScene;

    view->setScene(scene);

    this->scene->addLine(0,0,0,700);//Keep the screen from shifting

    this->scene->addLine(0,0,1425,0);

    qApp->processEvents();

    scene->clear();

    system->setC(0*30 + 150);

    on_TimeSlider_valueChanged(10);

    QObject::connect(timer1, SIGNAL(timeout()), this, SLOT(timetick()));

    timer1->start(0);

    on_pushButton_clicked();

    Sysfactory(-2);

    //saves->Write_system("test");

    //on_pushButton_clicked();

//    _sleep(100);
//////    on_GSlider_valueChanged(50);

    on_Traced_toggled(true);

    //Load("test");
    playlist->addMedia(QUrl("qrc:/Transfer.mp3"));
    playlist->addMedia(QUrl("qrc:/Drifting.mp3"));
    music->setPlaylist(playlist);
    music->setVolume(20);
    music->play();
    music->playlist()->shuffle();
    connect(music, &QMediaPlayer::stateChanged, [this](QMediaPlayer::State state) {
            music->setVolume(20);

            if (state == QMediaPlayer::State::StoppedState)
            {
                music->play();
            }
    });



    rocket = false;



}

void MainWindow::keyPressEvent(QKeyEvent *event){
//lets the player move
    if(player != nullptr){
        if( event->key() == Qt::Key_W ){

           player->thrust(100);

        }
        if( event->key() == Qt::Key_A){

           player->changeaV(-.1);

        }
        if( event->key() == Qt::Key_D ){

           player->changeaV(.1);

        }
    }
}

void MainWindow::pause(){// stops the signal from the timer

    if(this->timer1->signalsBlocked()){

        this->timer1->blockSignals(false);

    }else{

        this->timer1->blockSignals(true);

    }

}

void MainWindow::timetick(){

    timer->stop();

    if (system->process()){// calculates the particle movements

        for (Particle *par : system->Getparticles()){

            if(par->scene() == nullptr){

                scene->addItem(par);

            }
            if(par->getcol()){

                if(par == player){

                    player = nullptr;

                }

                system->Remove(par->getid());

                delete par;
            }

        }
    }
    if(traced && tick % 150 == 0){//traces the paths
        for(Particle * par:system->Getparticles()){
            QPen pen;

            int c = par->getid() % 5;

            if (c == 0){

                pen.setColor(Qt::darkCyan);

            }else if(c == 1 ){

                pen.setColor(Qt::darkGreen);

            }else if (c == 2){

                pen.setColor(Qt::darkYellow);

            }else if (c == 3){

                pen.setColor(Qt::blue);

            }else{

                pen.setColor(Qt::darkMagenta);

            }

            QGraphicsEllipseItem* el = scene->addEllipse(int(par->getx()),int(par->gety()),1,1,pen);

            el->setOpacity(.5);

            trace.push_back(el);
        }
    }
    tick++;

    //std::cout<<tick<<std::endl;

    this->scene->update();

    timer->start(0);

}

void MainWindow::addParticle(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed){

    scene->addItem(system->addParticle(Mass, _x, _y , _vx, _vy, fixed));

}

void MainWindow::addRocket(int Mass, long double _x, long double _y , long double _vx, long double _vy, int plan ,int ref){
    //adds a rocket to the system with flightplan plan

    Rocket *rock = system->addRocket(Mass, _x, _y , _vx, _vy, 0);

    scene->addItem(rock);

    player = rock;

    if(plan == 1){

        std::vector <Particle *> references = {};

        references.push_back(system->Getparticles()[0]);

        references.push_back(system->Getparticles()[ref]);

        references.push_back(rock);

        int stage = 0;

        system->add_flight(rock, plan , references, stage);

        //system->add_flight(y);
    }else if(plan == 2){

        std::vector <Particle *> references = {};

        references.push_back(system->Getparticles()[0]);

        references.push_back(system->Getparticles()[ref]);

        references.push_back(rock);

        references.push_back(system->Getparticles()[1]);

        int stage = 0;

        system->add_flight(rock, plan , references, stage);
    }
}


void MainWindow::Sysfactory(int sel){

    on_pushButton_clicked();

    if(sel == -2){

        on_GSlider_valueChanged(10);//sets G

        int sol = 59000 + rand()%2000;

        addParticle(2*sol,1425/2  ,700/2  ,0,0,1);

        //addParticle(sol,1425/2  ,700/2 - 40 ,-sqrt(g*sol/160),0,0);

        addParticle(1000, 1425/2 + 300, 700/2 ,0,sqrt(g*2*sol/300),0);

        addParticle(1000, 1425/2 - 700, 700/2 ,0,-sqrt(g*2*sol/700),0);

        addRocket(200,1425/2 + 300 ,700/2 + 20 , sqrt(g * 600/ 10), sqrt(g*2*sol/300) , 2 , 2);

        //addRocket(200,1425/2 ,700/2 + 150  , - sqrt(g*2*sol/150) , 0 , 1 , 1);
    }

    if(sel == -1){//transfer

        on_GSlider_valueChanged(50);//sets G

        int sol = 10000+rand()%15000;

        addParticle(sol,1425/2 ,700/2 ,0,0,1);

        double ang = (rand()%360)*180/3.14;

        double p_h = rand()%200 + 200;

        double r_h = rand()%25 + 75;

        int mass = 400 + rand()%400;

        addParticle(mass, 1425/2 + p_h * sin(ang) , 700/2 + p_h * cos(ang) , sqrt(g*sol/p_h) * cos(ang), -sqrt(g*sol/p_h) * sin(ang), 0);

        addParticle(mass, 1425/2 - p_h/1.2 * sin(ang) , 700/2 - p_h/1.2 * cos(ang) , -sqrt(g*sol/(p_h/1.2)) * cos(ang), +sqrt(g*sol/(p_h/1.2)) * sin(ang), 0);

        addRocket(200,1425/2 - r_h ,700/2 , 0, +sqrt((g*sol+200)/r_h) , 1 , 1);

        addRocket(200,1425/2 + r_h ,700/2 , 0, -sqrt((g*sol+200)/r_h) , 1 , 2);
    }

    if(sel == 0){//Earth-Moon

        on_GSlider_valueChanged(50);

        addParticle(5000,1425/2 ,700/2 ,0,0,1);

        addParticle(1000,1425/2 ,700/8 ,24,0,0);

        addParticle(50,1425/2 ,700/8+20 ,-15,0,0);


    }else if(sel == 1){//binary

        on_GSlider_valueChanged(60);

        addParticle(4000, 1425/2 , 700/2+70, 25, 0, 0);

        addParticle(5000, 1425/2, 700/2-70 , -20, 0, 0);

        addParticle(50, 1425/2, 700/2-105, -90, 0, 0);

        addParticle(75, 1425/2, 700/2+125, -20, 0, 0);

        addParticle(100, 1425/2, 700/10, 35, 0, 0);

        addParticle(10, 1425/2, 700/11 - 8, 20 , 2, 0);

    }else if(sel == 2){//binary-binary

        on_GSlider_valueChanged(100);

        addParticle(3000, 2 * 1425/6, 700/2 + 45, 30, 22, 0);

        addParticle(20, 2 * 1425/6 , 700/2 + 20, 120, 12, 0);

        addParticle(3000 , 2 * 1425/6, 700/2 - 45, -32, 22, 0);

        addParticle(3000,4*1425/6, 700/2 - 45, -30, -22, 0);

        addParticle(20, 4 * 1425/6, 700/2 - 20, -120, - 12, 0);

        addParticle(3000, 4 * 1425/6, 700/2 + 45, 30, -22, 0);

        //addParticle(50, 1425/2, 700/2, 0, 0, 0);



    }else if(sel == 3){//rings

        on_GSlider_valueChanged(13);

        addParticle(6000,1425/2 ,700/2 , 0, 0, 1);

        for(double i = 0; i<250 ;i++){

            double rinx = sin(i/20 * acos(0));

            double riny = cos(i/20 * acos(0));

            addParticle(4, 1425/2 + rinx*270 + rand()%20 - 10  , 700/2+riny*270+rand()%20 - 10 ,-riny*16.8+rand()%1,rinx*16.8+rand()%1,0);

            addParticle(4, 1425/2 + rinx*250 + rand()%20 - 10 , 700/2+riny*250+rand()%20 - 10 ,-riny*17+rand()%1,rinx*17+rand()%1,0);

            addParticle(4, 1425/2 + rinx*230 + rand()%20 - 10 , 700/2+riny*230+rand()%20 - 10 ,-riny*17.2+rand()%1,rinx*17.2+rand()%1,0);

        }
    }else if(sel == 4){//Moonception

        on_GSlider_valueChanged(90);

        addParticle(2900, 1425/2, 700/2,0,0,1);

        addParticle(1000, 1425/2,700/8,25,0,0);

        addParticle(200,1425/2+60,700/8,25,32,0);

        addParticle(20,1425/2+60,700/8 + 9, -8 ,24,0);

    }else if (sel == 5){//random

        on_GSlider_valueChanged(70);

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

        on_GSlider_valueChanged(70);

        int num = 800+rand()%400;

        for(int i = 0 ; i < num; i++){

            int rmass = rand()%20+5;

            int rxp = rand()%1425;

            int ryp = rand()%700;

            int rxv = rand()%20-10;

            int ryv = rand()%20-10+(700/2-rxp*.5)*.13;

            addParticle(rmass,rxp,ryp,rxv,ryv,0);
         }
    }else if (sel == 7) {//performance test
            on_GSlider_valueChanged(40);

            int num = 5000;

            int x = 0;

            int y = 0;

            for(int i = 0 ; i < num; i++){

                int rmass = rand()%20+5;

                x += 50;

                if(x % 2100 == 0){
                    y += 50;
                    x = 0;
                }

                int rxp = x+rand()%49;

                int ryp = y+rand()%49;

                int rxv = rand()%20-10;

                int ryv = rand()%20-10;

                addParticle(rmass,rxp,ryp,rxv,ryv,0);
            }
    }else if (sel == 8){//smash
        on_GSlider_valueChanged(40);

        int num = 300+rand()%200;

        for(int i = 0 ; i < num; i++){

            int rmass = rand()%20+5;

            int rxp = rand()%200 + 3 * 1225 /4;

            int ryp = rand()%700;

            int rxv = rand()%20-70;

            int ryv = rand()%20-10;

            addParticle(rmass,rxp,ryp,rxv,ryv,0);
         }

        for(int i = 0 ; i < num; i++){

            int rmass = rand()%20+5;

            int rxp = rand()%200 + 1225 /4;

            int ryp = rand()%700;

            int rxv = rand()%20+50;

            int ryv = rand()%20-10;

            addParticle(rmass,rxp,ryp,rxv,ryv,0);
         }


    }else if(sel == 9){// precession

        on_GSlider_valueChanged(80);

        on_Specialrel_toggled(true);

        SetC(0);

        addParticle(9000,1425/2 ,700/2 ,0,0,1);

        addParticle(100,1425/2 ,700/2 + 50 ,100,0,0);


    }


}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::mousePressEvent(QMouseEvent *event){


//   qApp->processEvents();

    //_sleep(10);


    if(*bre1==false){//check if initial click
        QGraphicsView * view = ui->Gameview;

        int yoff = -30;

        int xoff = -10;

        int gx = 0;

        int gy = 0;
        //int x=this->mapFromGlobal(cursor().pos()).x();//initial click cordinates

        int x = view->mapToScene(event->pos()).x() + xoff;//offset

        int y = view->mapToScene(event->pos()).y() + yoff;


        *bre1 = true;

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

            if(rocket){

                addRocket(200,x,y,(gx - x),(gy - y),0,0);

                rocket = false;

            }else{

                scene->addItem(system->addParticle(*mSlider*100, x, y, (gx - x),(gy - y), fixedcheck));

            }
            this->scene->removeItem(liny);
            //create Particle
        }else{//second click

                *bre1 = false;

        }
    this->scene->update();
}



void MainWindow::on_Gameview_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint)
{

}

void MainWindow::on_MassSlider_valueChanged(int value)
{
    *mSlider = value+1;
}

void MainWindow::on_TimeSlider_valueChanged(int value)
{
    *tSlider = value+1;

    system->setStep( .00005 * *tSlider);
}

void MainWindow::on_pushButton_clicked()//clear
{
    pause();
    player = nullptr;
    system->clear();
    for(auto dot:trace){
        delete dot;
    }
    trace.clear();
    pause();
    scene->update();
    tick = 0;
    //scene->setSceneRect(0, 0, scene->width(), scene->height());
}

void MainWindow::on_GSlider_valueChanged(int value)//G
{
    g = (value) * 0.3 * 2;

    system->System::setG(g/2);

    this->ui->GSlider->setSliderPosition(value);
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

void MainWindow::on_actionInner_Solar_System_triggered()//Moonception
{
    Sysfactory(4);
}


void MainWindow::on_actionRandom_triggered()
{
    Sysfactory(5);
}

void MainWindow::on_actionRandom_Twist_triggered()
{
    Sysfactory(6);
}

void MainWindow::on_actionSmash_triggered()
{
    Sysfactory(8);
}

void MainWindow::on_actionPrecession_triggered()
{
    Sysfactory(9);

}

void MainWindow::SetC(int value)//C
{
    system->setC(value*10 + 150);
}

void MainWindow::on_Collision_triggered(bool checked)
{
    system->setcollision(checked);
}

void MainWindow::on_actionEnable_Debris_triggered(bool checked)
{
    system->setDebris(checked);
}


void MainWindow::on_Refresh_clicked()
{
    pause();
}

void MainWindow::on_Spawn_rocket_triggered()
{
     rocket = true;
}


void MainWindow::on_actionset_Speed_of_Light_triggered()
{
    w.move(QWidget::mapToGlobal(QPoint(100,0)));
    w.show();

    QObject::connect(&w,&C_slider::valueChanged,this,&MainWindow::SetC);

}

void MainWindow::on_actionLoad_Save_triggered()
{

    savewindow.move(QWidget::mapToGlobal(QPoint(10,20)));
    savewindow.show();

    QObject::connect(&savewindow,&SaveLoad::Save_sig,this,&MainWindow::Save);

    QObject::connect(&savewindow,&SaveLoad::Load_sig,this,&MainWindow::Load);
}

void MainWindow::Save(std::string name){

    saves->Write_system(name);

}

void MainWindow::Load(std::string name){

    on_pushButton_clicked();

    for(auto par: saves->Read_system(name)){

        scene->addItem(par);

    };
    rocket = false;
}

void MainWindow::on_Specialrel_toggled(bool arg1)
{
//    if(arg1 == true){
//        on_actionBarnes_Hut_triggered(false);
//    }
    system->setSpecial_rel(arg1);

    this->ui->Specialrel->setChecked(arg1);
}


void MainWindow::on_actionTransfer_triggered()
{
    Sysfactory(-1);
}

void MainWindow::on_Traced_toggled(bool checked)
{
    for(auto dot:trace){
        delete dot;
    }
    trace.clear();

    traced = checked;

    this->ui->Traced->setChecked(checked);

}



void MainWindow::on_actionEnable_toggled(bool arg1)
{
    if(arg1 == false){
        music->pause();
    }else{
        music->play();
    }
}


void MainWindow::on_actionBarnes_Hut_triggered(bool checked)
{
//    if(checked == true){
//        on_Specialrel_toggled(false);
//    }

    //this->ui->actionBarnes_Hut->setChecked(checked);
}


void MainWindow::on_actionPerformance_Test_triggered()
{
    Sysfactory(7);
}


void MainWindow::on_actionShow_Visualization_triggered(bool checked)
{
    if(system->root->scene()==nullptr && checked){
        scene->addItem(system->root);
    }else{
        scene->removeItem(system->root);
    }
}


void MainWindow::on_Barnes_Hut_triggered(bool checked)
{
        system->barnes_hut = checked;
}

