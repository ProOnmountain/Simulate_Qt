#ifndef NODEPAINTER_H
#define NODEPAINTER_H

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include<mutex>
#include<condition_variable>

#include "def.h"

namespace Ui {
class NodePainter;
}
using namespace std;

class NodePainter : public QWidget
{
    Q_OBJECT

public:
    explicit NodePainter(QWidget *parent = 0);
    ~NodePainter();

private slots:
    void on_pushButtonChooseFile_clicked();

    void on_pushButtonStaticDisplay_clicked();

    void updateReadBar(int value);

private:
    void init();
    void setChartAxisTitle(QChart *chart, QString title);
    void readData(QString fileName);//从文件里面读取数据

private:
    Ui::NodePainter *ui;
    //控件
    QProgressBar  *readBar;

    //相关变量
    CpuData *cpuData;
    MemoryData *memData;
    DiskData *diskData;
    NetData *netData;
    NodeTime *nodeTime;


    mutex dataMutex;//写数据的锁
    mutex threadMutex;
    condition_variable readCon;

signals:
    void updateBar(int value);
};

#endif // NODEPAINTER_H
