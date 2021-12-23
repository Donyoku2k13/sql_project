#include "bill.h"
#include "ui_bill.h"

#include <QSqlQuery>
#include <QDebug>

#define SQL_SELECT_GUEST_INFO "SELECT contract.id, guest.first_name, guest.last_name, guest.middle_name, contract.arrival, (contract.arrival + interval'1 day' * contract.duration)::date, contract.duration, room_type.price FROM contract, guest, room, room_type WHERE contract.guest_id = guest.passport AND contract.room_id = room.id AND room.room_type_id = room_type.id AND guest.passport = %1"

bill::bill(QString guestPassport, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::bill)
{
    ui->setupUi(this);

    QSqlQuery query;
    if (query.exec(QString(SQL_SELECT_GUEST_INFO).arg(guestPassport))) {
        query.next();
        ui->number->setText(query.value(0).toString());
        ui->first_name->setText(query.value(1).toString());
        ui->last_name->setText(query.value(2).toString());
        ui->middle_name->setText(query.value(3).toString());
        ui->arrival->setText(query.value(4).toString());
        ui->departure->setText(query.value(5).toString());
        ui->duration->setText(query.value(6).toString());
        ui->cost->setText(query.value(7).toString());
        ui->date->setText(query.value(4).toString());
        int cost = query.value(7).toInt() * query.value(6).toInt();
        ui->total->setText(QString("%1").arg(cost));

    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
}

bill::~bill()
{
    delete ui;
}