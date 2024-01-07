#ifndef C_SLIDER_H
#define C_SLIDER_H

#include <QWidget>

namespace Ui {
class C_slider;
}

class C_slider : public QWidget
{
    Q_OBJECT

public:
    explicit C_slider(QWidget *parent = nullptr);
    ~C_slider();

public slots:

    void on_horizontalSlider_valueChanged(int value);

signals:

    void valueChanged(int newValue);

private:

    int c;

    Ui::C_slider *ui;
};

#endif // C_SLIDER_H
