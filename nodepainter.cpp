#include "nodepainter.h"
#include "ui_nodepainter.h"
#include<QDebug>
#include <thread>
#include <windows.h>
#include <QFile>
#include <QMetaType>

NodePainter::NodePainter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NodePainter)
{
    ui->setupUi(this);
    init();
}

NodePainter::~NodePainter()
{
    delete ui;
}

void NodePainter::init()
{
    //分配储存数据的空间
    cpuData = new CpuData[DATACAPACITY];
    memData = new MemoryData[DATACAPACITY];
    diskData = new DiskData[DATACAPACITY];
    netData = new NetData[DATACAPACITY];
    nodeTime = new NodeTime[DATACAPACITY];

    //初始化绘图chart
    ui->widgetCpu->setChart(new QChart());
    setChartAxisTitle(ui->widgetCpu->chart(), QString("cpu利用"));
    ui->widgetMem->setChart(new QChart());
    setChartAxisTitle(ui->widgetMem->chart(), QString("内存利用"));
    ui->widgetDisk->setChart(new QChart());
    setChartAxisTitle(ui->widgetDisk->chart(), QString("磁盘利用"));
    ui->widgetNet->setChart(new QChart());
    setChartAxisTitle(ui->widgetNet->chart(), QString("网络利用"));

    QObject::connect(this, SIGNAL(updateBar(int)), this, SLOT(updateReadBar(int)));
}

void NodePainter::setChartAxisTitle(QChart *chart, QString title)
{
    if(chart != nullptr)
    {
        chart->setTitle(title);
        QValueAxis *xAxis = new QValueAxis();
        QValueAxis *yAxis = new QValueAxis();
        xAxis->setMin(0);
        xAxis->setMax(100);
        yAxis->setMin(0);
        yAxis->setMax(120);
        xAxis->setTitleText(QStringLiteral("时间(ms)"));
        yAxis->setTitleText(QStringLiteral("利用率(%)"));

        xAxis->setLabelFormat("%u");
        yAxis->setLabelFormat("%u");
        xAxis->setLabelsAngle(45);
        xAxis->setGridLineVisible(true);
        yAxis->setGridLineVisible(true);
        xAxis->setMinorTickCount(1);
        yAxis->setMinorTickCount(1);
        xAxis->setTickCount(11);
        chart->addAxis(xAxis, Qt::AlignBottom);
        chart->addAxis(yAxis, Qt::AlignLeft);
    }
}

void NodePainter::readData(QString fileName)
{
    if(fileName == "")
    {
        emit updateBar(-1);
        qDebug() << "empty file name";
        return;
    }
    QFile fileHandle(fileName);
    if(!fileHandle.isOpen())
    {
        fileHandle.open(QIODevice::ReadOnly);
    }
    QByteArray line;
    QJsonParseError parseJsonErr;
    QJsonDocument jsonDoc;
    int counter = 0;
    while(!fileHandle.atEnd())
    {
        if(counter >= DATACAPACITY)
        {
            counter = 0;
            emit updateBar(-1);
            qDebug() <<"read ok";
            unique_lock<mutex> threadLock(threadMutex);
            readCon.wait(threadLock);
        }
        line = fileHandle.readLine();
        counter++;
        qDebug() << counter;
        emit updateBar(counter);
        jsonDoc = QJsonDocument::fromJson(line, &parseJsonErr);
        if (!(parseJsonErr.error == QJsonParseError::NoError))
        {
            qDebug() << QString::fromLocal8Bit("解析json文件错误！");
            return;
        }
        QJsonObject each_obj=jsonDoc.object();
        dataMutex.lock();
        QJsonArray nodeTimeJson = each_obj["0"].toArray();
        QJsonArray cpuJson = each_obj["1"].toArray();
        QJsonArray memJson = each_obj["2"].toArray();
        QJsonArray diskJson = each_obj["3"].toArray();
        QJsonArray netJson = each_obj["4"].toArray();

        //提取节点和时间
        int time = nodeTimeJson.at(1).toInt() / TIMECONVERT;
        int nodeNum = nodeTimeJson.at(0).toInt();
        nodeTime[counter].node = nodeNum;
        nodeTime[counter].time = time;

        //提取cpu数据
        double cpuTotalCore = cpuJson.at(1).toDouble();
        double cpuUsedCore = cpuJson.at(2).toDouble();
        cpuData[counter].usage = cpuUsedCore / cpuTotalCore * 100;

        //提取内存数据
        double memTotal = memJson.at(1).toDouble();
        double memUsed = memJson.at(2).toDouble();
        memData[counter].usage = memUsed / memTotal * 100;

        //提取磁盘数据


        //提取网络数据
        dataMutex.unlock();
    }
    emit updateBar(-1);
    qDebug() <<"read all file";
}

void NodePainter::on_pushButtonChooseFile_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("打开文件");
    QDir cur_path(QCoreApplication::applicationDirPath());
    fileDialog->setDirectory(cur_path);
    fileDialog->setNameFilter(tr("Json File(*.json)"));
    QString jsonFile;
    if(fileDialog->exec() == QDialog::Accepted)
    {
        jsonFile = fileDialog->selectedFiles().first();
        ui->lineEditFileDir->setText(jsonFile);
    }
    delete fileDialog;
    fileDialog = nullptr;
    readBar = new QProgressBar();
    readBar->setWindowTitle(QString("读取文件中.."));
    readBar->setRange(0, DATACAPACITY);
    readBar->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
    readBar->setWindowModality(Qt::ApplicationModal);
    readBar->setFixedSize(280, 25);
    readBar->setGeometry(this->x() + this->width() / 2 - readBar->width() / 2, this->y() + this->height() / 2, 280, 25);
    readBar->show();
//    ui->pushButtonChooseFile->setEnabled(false);
    std::thread *readThread = new std::thread(&NodePainter::readData, this, jsonFile);
    readThread->detach();
}

void NodePainter::on_pushButtonStaticDisplay_clicked()
{
    readCon.notify_one();
}

void NodePainter::updateReadBar(int value)
{
    if(value == -1)
    {
        readBar->setValue(0);
        readBar->close();
//        ui->pushButtonChooseFile->setEnabled(true);
    }
    else
    {
//        if(value == 1)
//        {
//            readBar->show();
//        }
        readBar->setValue(value);
    }
}
