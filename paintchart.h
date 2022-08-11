#ifndef PAINTCHART_H
#define PAINTCHART_H

#include <QWidget>
#include <QChartView>

using namespace QtCharts;
namespace Ui {
class PaintChart;
}

class PaintChart : public QChartView
{
    Q_OBJECT

public:
    explicit PaintChart(QWidget *parent = 0);
    ~PaintChart();
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

private:
    Ui::PaintChart *ui;
    int mouse_x, mouse_y;
};

#endif // PAINTCHART_H
