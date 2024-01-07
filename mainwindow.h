#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <vector>
#include "Particle.h"
#include "system.h"
#include "rocket.h"
#include "c_slider.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Sysfactory(int sel);

private slots:

    void SetC(int value);

    void on_Gameview_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

    void on_MassSlider_valueChanged(int value);

    void on_TimeSlider_valueChanged(int value);

    void timetick();

    void on_pushButton_clicked();

    void on_GSlider_valueChanged(int value);

    void on_checkBox_clicked(bool checked);

    void on_earthmooon_triggered();

    void on_actionBinary_triggered();

    void on_actionBinary_Binary_triggered();

    void on_actionRings_triggered();

    //void on_pushButton_2_clicked();

    //void on_pushButton_2_pressed();

    void on_actionInner_Solar_System_triggered();

    void on_Refresh_clicked();

    void on_actionRandom_triggered();

    void on_actionRandom_Twist_triggered();

    void on_Spawn_rocket_triggered();

    void on_Collision_triggered(bool checked);

    void on_actionEnable_Debris_triggered(bool checked);

    void on_actionset_Speed_of_Light_triggered();

    void on_actionSmash_triggered();

    void on_Specialrel_toggled(bool arg1);

    void on_actionTransfer_triggered();

private:

    C_slider w;

    void keyPressEvent(QKeyEvent *event) override;

    Rocket * player;

    bool rocket;

    void addParticle(int Mass, long double _x, long double _y , long double _vx, long double _vy, bool fixed);
    void addRocket(int Mass, long double _x, long double _y , long double _vx, long double _vy, int plan);

    cords gravity(double par1x , double par1y , double par2x , double par2y , double m1, double m2);

    double step = .00004;

    void Remove(int id, std::vector <Particle *> input);

    void Idreset();

    bool col = false;

    QTimer *timer1 = new QTimer(this);

    void pause();

    bool fixedcheck = false;

    float g = 1; //gravitational const

    System * system;

    QTimer * timer = new QTimer(this);

    int tslider = 1;

    int* tSlider = &tslider;

    int mslider = 1;

    int* mSlider = &mslider;

    bool bre = false;//Break bool

    bool* bre1 = &bre;

    Ui::MainWindow *ui;

    QGraphicsScene *scene;

    void mousePressEvent(QMouseEvent *event) override;

    int id = 0;

};
#endif // MAINWINDOW_H
