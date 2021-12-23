#ifndef WORKERFIRE_H
#define WORKERFIRE_H

#include <QWidget>
#include <QMap>
#include "changescheduleworker.h"

namespace Ui {
    class WorkerFire;
}

class WorkerFire : public QWidget
{
    Q_OBJECT

public:
    explicit WorkerFire(QWidget* parent = nullptr);
    ~WorkerFire();

private:
    Ui::WorkerFire* ui;
    void closeEvent(QCloseEvent* event);
    QMap<int, QString> _workers;
    int _worker_id;
    void on_workers_list_clicked(const QModelIndex& index);
    QList<ChangeScheduleWorker*> _windows;


signals:
    void closed();
private slots:
    void on_fire_button_clicked();
    void prevIsClosed(bool isOk);
};

#endif // WORKERFIRE_H