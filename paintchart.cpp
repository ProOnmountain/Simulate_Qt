#include "paintchart.h"
#include "ui_paintchart.h"
#include <QMouseEvent>
#include <QValueAxis>

PaintChart::PaintChart(QWidget *parent) :
    QChartView(parent),
    ui(new Ui::PaintChart)
{
    ui->setupUi(this);
}

PaintChart::~PaintChart()
{
    delete ui;
}

void PaintChart::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        //qDebug() << "chart x:" <<event->x() << " y:"<<event->y();
        int cur_mouse_x = event->x();
        int cur_mouse_y = event->y();
        QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
        if(cur_mouse_x < mouse_x)
        {
            this->chart()->scroll(2, 0);
        }
        else
        {
            if(axisX->min() > 3)
            {
                this->chart()->scroll(-2, 0);
            }

        }
        QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
        if(cur_mouse_y > mouse_y)
        {
            this->chart()->scroll(0, 1);
        }
        else
        {
            if(axisY->min() > 1)
            {
                this->chart()->scroll(0, -1);
            }

        }
        mouse_x = cur_mouse_x;
        mouse_y = cur_mouse_y;
    }
}

void PaintChart::mousePressEvent(QMouseEvent *event)
{
    mouse_x = event->x();
    mouse_y = event->y();
}
