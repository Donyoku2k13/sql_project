#include "workerfire.h"
#include "ui_workerfire.h"

#define SQL_SELECT_WORKERS_LIST "SELECT first_name, middle_name, last_name, id FROM worker"
#define SQL_DELETE_WORKER "DELETE FROM worker WHERE worker.id = %1"
#define SQL_SELECT_EMPTY_SCHEDULE "SELECT dayweek_id, floor_id FROM cleaning_schedule WHERE worker_id = %1"

#include <QSqlQuery>
#include <QStringListModel>
#include <QDebug>

WorkerFire::WorkerFire(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WorkerFire)
{
    ui->setupUi(this);

    QSqlQuery query;
    if (query.exec(SQL_SELECT_WORKERS_LIST)) {
        QStringList* allLines = new QStringList(); //Your list for lines from the file.
        allLines->clear();
        while (query.next()) {

            QStringList l;
            l.append(query.value(0).toString());
            l.append(query.value(1).toString());
            l.append(query.value(2).toString());
            l.append(query.value(3).toString());
            QString str = QString("%1 %2 %3").arg(l[0]).arg(l[1]).arg(l[2]);
            _workers.insert(l[3].toInt(), str);

            allLines->append(str);
        }
        QStringListModel* linesModel = new QStringListModel(*allLines, NULL);
        linesModel->setStringList(*allLines);
        ui->workers_list->setModel(linesModel);
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
}

WorkerFire::~WorkerFire()
{
    delete ui;
}

void WorkerFire::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    emit this->closed();
}

void WorkerFire::on_workers_list_clicked(const QModelIndex& index)
{

    ui->fire_button->setEnabled(true);
}

void WorkerFire::prevIsClosed(bool isOk)
{
    ChangeScheduleWorker* closedWindow = qobject_cast<ChangeScheduleWorker*>(sender());
    _windows.removeOne(closedWindow);
    if (!isOk) {
        for (ChangeScheduleWorker* window : _windows) {
            window->close();
        }
        this->show();
    }
    if (_windows.empty() && isOk) {
        _workers.remove(_worker_id);
        QSqlQuery delete_worker;
        if (!delete_worker.exec(QString(SQL_DELETE_WORKER).arg(_worker_id)))
            qCritical() << "Failed executing SQL-query";
        this->close();
    }
}

void WorkerFire::on_fire_button_clicked()
{
    QString str = ui->workers_list->selectionModel()->selectedRows()[0].data().toString();
    _worker_id = _workers.key(str);
    _workers.remove(_worker_id);
    QList<QPair<int/*day*/, int/*floor*/>> emptyDaysFloors;

    QSqlQuery empty_chedule_query;
    if (empty_chedule_query.exec(QString(SQL_SELECT_EMPTY_SCHEDULE).arg(_worker_id))) {
        QStringList* allLines = new QStringList(); //Your list for lines from the file.
        allLines->clear();
        while (empty_chedule_query.next()) {
            ChangeScheduleWorker* workerChange = new ChangeScheduleWorker(_workers, empty_chedule_query.value(0).toInt(), empty_chedule_query.value(1).toInt());
            _windows.append(workerChange);
            workerChange->show();
            connect(workerChange, SIGNAL(closed(bool)), this, SLOT(prevIsClosed(bool)));
        }
        this->hide();
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
}