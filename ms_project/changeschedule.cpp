#include "changeschedule.h"
#include "ui_changeschedule.h"
#include "changescheduleworker.h"

#include <QSqlTableModel>
#include <QSqlQuery>
#include <QDebug>

#define SQL_SELECT_WORKERS_LIST "SELECT first_name, middle_name, last_name, id FROM worker"

changeSchedule::changeSchedule(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::changeSchedule)
{
    ui->setupUi(this);

    _model = new QSqlTableModel(this);
    _model->setTable("cleaninng_schedule_view");
    _model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    _model->select();

    _model->setHeaderData(0, Qt::Horizontal, tr("День недели"));
    _model->setHeaderData(1, Qt::Horizontal, tr("Этаж"));
    _model->setHeaderData(2, Qt::Horizontal, tr("Сотрудник"));

    _model->submitAll();

    ui->tableView->setModel(_model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setColumnWidth(0, 150);
    ui->tableView->setColumnWidth(1, 30);
    ui->tableView->setColumnWidth(2, 350);
}

changeSchedule::~changeSchedule()
{
    delete ui;
}

void changeSchedule::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    emit this->closed();
}

void changeSchedule::on_tableView_doubleClicked(const QModelIndex& index)
{
    if (index.column() != 2)
        return;
    QSqlQuery query;
    QMap<int, QString> workers;
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
            workers.insert(l[3].toInt(), str);
        }
        int floor = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 1)).toInt();
        int dayweek = getDayInt(ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toString());
        ChangeScheduleWorker* changeScheduleWorker = new ChangeScheduleWorker(workers, dayweek, floor);
        changeScheduleWorker->show();
        connect(changeScheduleWorker, SIGNAL(closed(bool)), this, SLOT(prevIsClosed(bool)));
        this->setEnabled(false);
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }

}

void changeSchedule::prevIsClosed(bool isOK)
{
    this->setEnabled(true);
    _model->select();
}

int changeSchedule::getDayInt(QString i)
{
    QStringList myOptions;
    myOptions << "Понедельник" << "Вторник" << "Среда" << "Четверг" << "Пятница" << "Суббота" << "Воскресенье";

    switch (myOptions.indexOf(i)) {
    case 0:
        return 1;

    case 1:
        return 2;

    case 2:
        return 3;

    case 3:
        return 4;

    case 4:
        return 5;

    case 5:
        return 6;

    case 6:
        return 7;
    }
    return 0;
}