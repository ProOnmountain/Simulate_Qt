#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>

namespace Ui {
class StartMenu;
}

class StartMenu : public QWidget
{
    Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = 0);
    ~StartMenu();

private slots:
    void on_pushButtonNode_clicked();

    void on_pushButtonTask_clicked();

private:
    Ui::StartMenu *ui;
};

#endif // STARTMENU_H
