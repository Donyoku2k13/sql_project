#ifndef CHANGESCHEDULEWORKER_H
#define CHANGESCHEDULEWORKER_H

#include <QMap>
#include <QPair>
#include <QWidget>

namespace Ui {
    class ChangeScheduleWorker;
}

class ChangeScheduleWorker : public QWidget
{
    Q_OBJECT

public:
    ChangeScheduleWorker(QMap<int, QString> workers, int dayweek, int floor, QWidget* parent = nullptr);
    ~ChangeScheduleWorker();

private slots:
    void on_workers_list_clicked(const QModelIndex& index);
    void on_ok_button_clicked();

private:
    Ui::ChangeScheduleWorker* ui;
    void closeEvent(QCloseEvent* event);
    QMap<int, QString> _workers;
    int _floor;
    int _dayweek;
    QString getDayString(int i);
    bool _isOk;


signals:
    void closed(bool);
};

#endif // CHANGESCHEDULEWORKER_H#endif // CHANGESCHEDULEWORKER_H