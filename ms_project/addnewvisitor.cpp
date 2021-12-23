#include "addnewvisitor.h"
#include "ui_addnewvisitor.h"

#include "addcompanion.h"
#include "bill.h"

#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QErrorMessage>

#define SQL_INSERT_CITY "INSERT INTO city (city_name) SELECT '%1' WHERE NOT EXISTS (SELECT city_name FROM city WHERE city_name = '%1')"
#define SQL_INSERT_GUEST "INSERT INTO guest(first_name, middle_name, last_name, passport, city_id) SELECT '%1', '%2', '%3', %4, city.id FROM city WHERE city.city_name = '%5' AND NOT EXISTS (SELECT passport FROM guest WHERE passport = %4)"
#define SQL_SELECT_ROOMS "SELECT room.id, room_type.capacity, room_type.price FROM public.room, public.room_type WHERE room.is_available = TRUE AND room.room_type_id = room_type.id ORDER BY room_type.capacity, room_type.price, room.id"
#define SQL_SELECT_GUEST "SELECT id FROM guest WHERE passport = %1"
#define SQL_INSERT_VISIT "INSERT INTO current_guest_list (guest_id, companion_1_id, companion_2_id, room_id) VALUES (%1, %2, %3, %4); INSERT INTO contract (arrival, duration, guest_id, room_id) VALUES (now(), %5, %1, %4); UPDATE room SET is_available = FALSE WHERE id = %4"

addNewVisitor::addNewVisitor(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::addNewVisitor),
    _guest_first_name(QString()),
    _guest_middle_name(QString()),
    _guest_last_name(QString()),
    _guest_passport(QString()),
    _guest_count(-1),
    _room_id(-1),
    _companion_1_first_name(QString()),
    _companion_1_middle_name(QString()),
    _companion_1_last_name(QString()),
    _companion_1_passport(QString("NULL")),
    _companion_2_first_name(QString()),
    _companion_2_middle_name(QString()),
    _companion_2_last_name(QString()),
    _companion_2_passport(QString("NULL")),
    _duration(-1),
    _city(QString())
{
    ui->setupUi(this);

    QSqlQuery query;
    if (query.exec(SQL_SELECT_ROOMS)) {
        QStringList* allLines = new QStringList(); //Your list for lines from the file.
        allLines->clear();
        while (query.next()) {

            QList<int> l;
            l.append(query.value(0).toInt());
            l.append(query.value(1).toInt());
            l.append(query.value(2).toInt());
            QString str = QString("%1 \t %2 \t %3").arg(l[0]).arg(l[1]).arg(l[2]);
            _roomsAvailable.insert(str, l);

            allLines->append(str);
        }
        QStringListModel* linesModel = new QStringListModel(*allLines, NULL);
        linesModel->setStringList(*allLines);
        ui->rooms->setModel(linesModel);
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
}

addNewVisitor::~addNewVisitor()
{
    delete ui;
}

void addNewVisitor::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    emit this->closed();
}

void addNewVisitor::makeBill()
{
    bill* billWindow = new bill(_guest_passport);
    billWindow->show();
}

void addNewVisitor::finishWork()
{
    QSqlQuery insert_city_query;
    insert_city_query.exec(QString(SQL_INSERT_CITY).arg(_city));
    QSqlQuery insert_guest_query;
    insert_guest_query.exec(QString(SQL_INSERT_GUEST).arg(_guest_first_name).arg(_guest_middle_name).arg(_guest_last_name).arg(_guest_passport).arg(_city));
    insert_guest_query.exec(QString(SQL_INSERT_GUEST).arg(_companion_1_first_name).arg(_companion_1_middle_name).arg(_companion_1_last_name).arg(_companion_1_passport).arg(_city));
    insert_guest_query.exec(QString(SQL_INSERT_GUEST).arg(_companion_2_first_name).arg(_companion_2_middle_name).arg(_companion_2_last_name).arg(_companion_2_passport).arg(_city));

    QString str = QString(SQL_INSERT_VISIT).arg(_guest_passport).arg(_companion_1_passport).arg(_companion_2_passport).arg(_room_id).arg(_duration);
    insert_guest_query.exec(QString(SQL_INSERT_VISIT).arg(_guest_passport).arg(_companion_1_passport).arg(_companion_2_passport).arg(_room_id).arg(_duration));

    this->makeBill();

    this->close();
}

void addNewVisitor::on_spinBox_valueChanged(int capacity)
{
    QStringList* allLines = new QStringList();
    allLines->clear();

    for (QList<int> list : _roomsAvailable) {
        if (list[1] >= capacity) {
            QString str = QString("%1 \t %2 \t %3").arg(list[0]).arg(list[1]).arg(list[2]);
            allLines->append(str);
        }
    }

    QStringListModel* linesModel = new QStringListModel(*allLines, NULL);
    linesModel->setStringList(*allLines);
    ui->rooms->setModel(linesModel);
}

void addNewVisitor::on_pushButton_clicked()
{
    _guest_first_name = ui->first_name_line->text();
    _guest_middle_name = ui->middle_name_line->text();
    _guest_last_name = ui->last_name_line->text();
    _guest_passport = ui->passport_line->text();
    _guest_count = ui->spinBox->value();
    _duration = ui->duration->value();
    _city = ui->city->text();
    if (!ui->rooms->selectionModel()->selectedRows().empty()) {
        QString str = ui->rooms->selectionModel()->selectedRows()[0].data().toString();
        _room_id = _roomsAvailable.value(str)[0];
    }
    if (_guest_first_name == QString() ||
        _guest_middle_name == QString() ||
        _guest_last_name == QString() ||
        _guest_passport == QString() ||
        _city == QString() ||
        _duration == -1 ||
        _room_id == -1) {
        (new QErrorMessage(this))->showMessage("Не все поля заполнены!");
    }
    else {
        if (_guest_count == 2) {
            addCompanion* companion = new addCompanion(-1, _companion_1_first_name, _companion_1_middle_name, _companion_1_last_name, _companion_1_passport);
            companion->show();
            connect(companion, SIGNAL(closed()), this, SLOT(finishWork()));
        }
        else  if (_guest_count == 3) {
            addCompanion* companion1 = new addCompanion(1, _companion_1_first_name, _companion_1_middle_name, _companion_1_last_name, _companion_1_passport);
            companion1->show();
            addCompanion* companion2 = new addCompanion(2, _companion_2_first_name, _companion_2_middle_name, _companion_2_last_name, _companion_2_passport);

            connect(companion1, SIGNAL(closed()), companion2, SLOT(show()));
            connect(companion2, SIGNAL(closed()), this, SLOT(finishWork()));
        }
        else {
            finishWork();
            return;
        }
        this->hide();
    }
}