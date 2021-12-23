#ifndef CHOOSEACTIONADMIN_H
#define CHOOSEACTIONADMIN_H

#include <QWidget>

namespace Ui {
    class chooseActionAdmin;
}

class chooseActionAdmin : public QWidget
{
    Q_OBJECT

public:
    explicit chooseActionAdmin(QWidget* parent = nullptr);
    ~chooseActionAdmin();

private:
    Ui::chooseActionAdmin* ui;

private slots:
    void onPrevWindowClosed();

    void on_fire_worker_button_clicked();

    void on_cleanin_schedule_change_button_clicked();
};

#endif // CHOOSEACTIONADMIN_H