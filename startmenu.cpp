#include "startmenu.h"
#include "ui_startmenu.h"
#include "nodepainter.h"

StartMenu::StartMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartMenu)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(),this->height());
}

StartMenu::~StartMenu()
{
    delete ui;
}

void StartMenu::on_pushButtonNode_clicked()
{
    //创建一个NodePainter
    NodePainter *nodePainter = new NodePainter();
    nodePainter->show();
}

void StartMenu::on_pushButtonTask_clicked()
{
    //创建一个TaskPainter
}
