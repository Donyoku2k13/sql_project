#include "changescheduleworker.h"
#include "ui_changescheduleworker.h"
#include "popupmenu.h"

#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringListModel>

#define SQL_UPDATE_SCHEDULE "UPDATE cleaning_schedule SET worker_id = %1 WHERE floor_id = %2 AND dayweek_id = %3"

ChangeScheduleWorker::ChangeScheduleWorker(QMap<int, QString> workers, int dayweek, int floor, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ChangeScheduleWorker)
{
    ui->setupUi(this);
    _workers = workers;
    _floor = floor;
    _isOk = false;
    _dayweek = dayweek;

    QString label = QString("Выберите рабочего для уборки %1 на %2 этаже").arg(getDayString(dayweek)).arg(floor);
    ui->label->setText(label);
    ui->label->setStyleSheet(QString("font-size:16pt; font-weight:150; color: white"));

    QStringList* allLines = new QStringList(); //Your list for lines from the file.
    allLines->clear();

    for (QString str : workers) {

        allLines->append(str);
    }
    QStringListModel* linesModel = new QStringListModel(*allLines, NULL);
    linesModel->setStringList(*allLines);
    ui->workers_list->setModel(linesModel);

    ui->ok_button->setEnabled(false);
}

ChangeScheduleWorker::~ChangeScheduleWorker()
{
    delete ui;
}

QString ChangeScheduleWorker::getDayString(int i)
{
    switch (i) {
    case 1:
        return "в понедельник";

    case 2:
        return "во вторник";

    case 3:
        return "в среду";

    case 4:
        return "в четверг";

    case 5:
        return "в пятницу";

    case 6:
        return "в субботу";

    case 7:
        return "в воскресенье";
    }
    return "";
}

void ChangeScheduleWorker::on_workers_list_clicked(const QModelIndex& index)
{
    ui->ok_button->setEnabled(true);
}

void ChangeScheduleWorker::on_ok_button_clicked()
{
    if (!ui->workers_list->selectionModel()->selectedRows().empty()) {
        QString str = ui->workers_list->selectionModel()->selectedRows()[0].data().toString();
        QSqlQuery updateQuery;
        int worker_id = _workers.key(str);
        if (!updateQuery.exec(QString(SQL_UPDATE_SCHEDULE).arg(worker_id).arg(_floor).arg(_dayweek)))
        {
            qCritical() << qPrintable("Failed executing SQL-queryа");
            _isOk = false;
        }
        else {
            _isOk = true;
        }
    }
    else {
        qCritical() << qPrintable("Ошибка при считывании выбранной строки");
        _isOk = false;
    }
    this->close();
}

void ChangeScheduleWorker::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    emit this->closed(_isOk);
}