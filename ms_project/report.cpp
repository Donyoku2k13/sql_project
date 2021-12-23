#include "report.h"
#include "ui_report.h"

#include <QSqlQuery>
#include <QStringListModel>
#include <QDebug>

#define SQL_SELECT_GUESTS_COUNT "SELECT count(distinct guest_id) AS exact_count FROM contract WHERE arrival < now() AND arrival > now() - interval '3 month'"
#define SQL_SELECT_ROOMS_LIST "SELECT id FROM room ORDER BY id"
#define SQL_SELECT_ROOM_DURATIONS "SELECT duration FROM contract WHERE room_id = %1"
#define SQL_SELECT_COSTS "SELECT room_type.price, contract.duration FROM contract, room, room_type WHERE contract.room_id = room.id AND room.room_type_id = room_type.id"

report::report(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::report)
{
    ui->setupUi(this);

    QStringList* allLines = new QStringList();
    allLines->clear();
    QSqlQuery roomQuery;
    if (roomQuery.exec(SQL_SELECT_ROOMS_LIST)) {
        while (roomQuery.next()) {
            QSqlQuery roomCountQuery;
            if (roomCountQuery.exec(QString(SQL_SELECT_ROOM_DURATIONS).arg(roomQuery.value(0).toInt()))) {
                roomCountQuery.next();
                QString str = QString("Комната №%1 - занятых дней: %2").arg(roomQuery.value(0).toInt()).arg(roomCountQuery.value(0).toInt());
                allLines->append(str);
            }
            else {
                qCritical() << "Failed executing SQL-query";
            }
        }
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
    QStringListModel* diseasesModel = new QStringListModel(*allLines, NULL);
    diseasesModel->setStringList(*allLines);
    ui->rooms->setModel(diseasesModel);

    QSqlQuery guestsCountQuery;
    if (guestsCountQuery.exec(SQL_SELECT_GUESTS_COUNT)) {
        guestsCountQuery.next();
        ui->guests_number->setText(guestsCountQuery.value(0).toString());
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }

    QSqlQuery costsQuery;
    int total = 0;
    if (costsQuery.exec(SQL_SELECT_COSTS)) {
        while (costsQuery.next()) {
            total += costsQuery.value(0).toInt() * costsQuery.value(1).toInt();
        }
        ui->total->setText(QString("%1").arg(total));
    }
}

report::~report()
{
    delete ui;
}