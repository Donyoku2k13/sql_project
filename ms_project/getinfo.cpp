#include "getinfo.h"
#include "ui_getinfo.h"
#include "popupmenu.h"

#include <QSqlQuery>
#include <QDebug>
#include <QStringListModel>

#define SQL_SELECT_BUSY_ROOMS "SELECT room.id, room_type.capacity, room_type.price FROM public.room, public.room_type WHERE room.is_available = FALSE AND room.room_type_id = room_type.id ORDER BY room.id"
#define SQL_SELECT_GUEST_IN_ROOM_LIST "SELECT guest.first_name, guest.middle_name, guest.last_name, guest.passport FROM current_guest_list, room, guest WHERE current_guest_list.room_id = room.id AND room.id = %1 AND (guest.passport = current_guest_list.guest_id OR guest.passport = current_guest_list.companion_1_id OR guest.passport = current_guest_list.companion_2_id)"
#define SQL_SELECT_AVAILABLE_ROOMS "SELECT room.id, room_type.capacity, room_type.price FROM public.room, public.room_type WHERE room.is_available = TRUE AND room.room_type_id = room_type.id ORDER BY room_type.price"
#define SQL_SELECT_CITIES "SELECT city_name FROM city ORDER BY city_name"
#define SQL_SELECT_GUESTS_FROM_CITY "SELECT guest.first_name, guest.middle_name, guest.last_name FROM guest, city WHERE city.city_name = '%1' AND city.id = guest.city_id"
#define SQL_SELECT_CURRENT_GUEST_LIST "SELECT guest.first_name, guest.middle_name, guest.last_name, room.id FROM current_guest_list, guest, room WHERE current_guest_list.guest_id = guest.passport AND current_guest_list.room_id = room.id"
#define SQL_SELECT_WORKER_BY_DAY_AND_GUEST "SELECT worker.first_name, worker.middle_name, worker.last_name FROM worker, cleaning_schedule, dayweek, room WHERE room.id = %1 AND dayweek.id = %2 AND dayweek.id = cleaning_schedule.dayweek_id AND  room.floor_id = cleaning_schedule.floor_id AND worker.id = cleaning_schedule.worker_id"

getInfo::getInfo(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::getInfo)
{
    ui->setupUi(this);

    //-------------------------------Свободные комнаты----------------------------------
    QSqlQuery query;
    if (query.exec(SQL_SELECT_AVAILABLE_ROOMS)) {
        QStringList* allLines = new QStringList(); //Your list for lines from the file.
        allLines->clear();
        while (query.next()) {

            QList<int> l;
            l.append(query.value(0).toInt());
            l.append(query.value(1).toInt());
            l.append(query.value(2).toInt());
            QString str = QString("%1 \t %2 \t %3").arg(l[0]).arg(l[1]).arg(l[2]);
            _roomsAvailable.insert(str, l);

            if (l[1] == 1)
                allLines->append(str);
        }
        QStringListModel* linesModel = new QStringListModel(*allLines, NULL);
        linesModel->setStringList(*allLines);
        ui->roomsList->setModel(linesModel);
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
    //-----------------------------------------------------------------------------------



    //---------------------------Список гостей в комнате---------------------------------
    QSqlQuery busy_rooms_query;
    if (busy_rooms_query.exec(SQL_SELECT_BUSY_ROOMS)) {
        QStringList* allLines = new QStringList(); //Your list for lines from the file.
        allLines->clear();
        PopupMenu* guest_in_room_menu = new PopupMenu(ui->select_room, this);
        while (busy_rooms_query.next()) {

            QList<int> l;
            l.append(busy_rooms_query.value(0).toInt());
            l.append(busy_rooms_query.value(1).toInt());
            l.append(busy_rooms_query.value(2).toInt());
            QString str = QString("%1\t\t%2 \t\t%3").arg(l[0]).arg(l[1]).arg(l[2]);

            allLines->append(str);

            QAction* action = guest_in_room_menu->addAction(QString("%1").arg(l[0]));
            connect(action, SIGNAL(triggered()), this, SLOT(select_room_changed()));
        }
        ui->select_room->setMenu(guest_in_room_menu);
        guest_in_room_menu->setStyleSheet("QMenu::item:selected{background-color: rgb(33, 57, 84); } QMenu::item{color: white;};");
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
    //-----------------------------------------------------------------------------------



    //-------------------Гости, прибывшие из определенного города------------------------
    QSqlQuery cities_query;
    if (cities_query.exec(SQL_SELECT_CITIES)) {
        PopupMenu* city_menu = new PopupMenu(ui->select_city, this);
        while (cities_query.next()) {

            QString city = cities_query.value(0).toString();
            QAction* action = city_menu->addAction(QString("%1").arg(city));
            connect(action, SIGNAL(triggered()), this, SLOT(select_city_changed()));
        }
        ui->select_city->setMenu(city_menu);
        city_menu->setStyleSheet("QMenu::item:selected{background-color: rgb(33, 57, 84); } QMenu::item{color: white;};");
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
    //-----------------------------------------------------------------------------------



    //---------ФИО работника, убиравшего определенную комнату в определенный день--------
    _room = -1;
    _day = -1;
    ui->show_worker->setHidden(true);
    QSqlQuery current_guest_list_query;
    if (current_guest_list_query.exec(SQL_SELECT_CURRENT_GUEST_LIST)) {
        PopupMenu* current_guest_list_menu = new PopupMenu(ui->guest_button, this);
        while (current_guest_list_query.next()) {

            QString current_guest_list = current_guest_list_query.value(0).toString() + " " +
                current_guest_list_query.value(1).toString() + " " +
                current_guest_list_query.value(2).toString();

            _currentGuestList.insert(current_guest_list_query.value(3).toInt(), current_guest_list);

            QAction* action = current_guest_list_menu->addAction(QString("%1").arg(current_guest_list));
            connect(action, SIGNAL(triggered()), this, SLOT(guest_button_changed()));
        }
        ui->guest_button->setMenu(current_guest_list_menu);
        current_guest_list_menu->setStyleSheet("QMenu::item:selected{background-color: rgb(33, 57, 84); } QMenu::item{color: white;};");

        PopupMenu* day_week_menu = new PopupMenu(ui->week_day_button, this);
        for (int i = 1; i <= 7; i++) {
            QAction* action1 = day_week_menu->addAction(QString("%1").arg(getDayString(i)));
            connect(action1, SIGNAL(triggered()), this, SLOT(week_day_button_changed()));
        }

        ui->week_day_button->setMenu(day_week_menu);
        day_week_menu->setStyleSheet("QMenu::item:selected{background-color: rgb(33, 57, 84); } QMenu::item{color: white;};");

    }
    else {
        qCritical() << "Failed executing SQL-query";
    }

    //-----------------------------------------------------------------------------------

}

getInfo::~getInfo()
{
    delete ui;
}

void getInfo::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    emit this->closed();
}

void getInfo::on_spinBox_valueChanged(int capacity)
{
    QStringList* allLines = new QStringList();
    allLines->clear();

    for (QList<int> list : _roomsAvailable) {
        if (list[1] == capacity) {
            QString str = QString("%1 \t %2 \t %3").arg(list[0]).arg(list[1]).arg(list[2]);
            allLines->append(str);
        }
    }

    QStringListModel* linesModel = new QStringListModel(*allLines, NULL);
    linesModel->setStringList(*allLines);
    ui->roomsList->setModel(linesModel);
}

void getInfo::select_room_changed()
{
    QAction* action = qobject_cast<QAction*>(sender());
    ui->select_room->setText(action->text());

    QSqlQuery select_guest_query;
    if (select_guest_query.exec(QString(SQL_SELECT_GUEST_IN_ROOM_LIST).arg(action->text()))) {
        QStringList* allLines = new QStringList(); //Your list for lines from the file.
        allLines->clear();
        while (select_guest_query.next()) {
            QStringList l;
            l.append(select_guest_query.value(0).toString());
            l.append(select_guest_query.value(1).toString());
            l.append(select_guest_query.value(2).toString());
            l.append(select_guest_query.value(3).toString());
            QString str = QString("%1\t %2 \t %3 \t %4").arg(l[0]).arg(l[1]).arg(l[2]).arg(l[3]);

            allLines->append(str);
        }
        QStringListModel* linesModel = new QStringListModel(*allLines, NULL);
        linesModel->setStringList(*allLines);
        ui->guestList->setModel(linesModel);
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
}

void getInfo::select_city_changed()
{
    QAction* action = qobject_cast<QAction*>(sender());
    ui->select_city->setText(action->text());

    QSqlQuery select_guest_query;
    if (select_guest_query.exec(QString(SQL_SELECT_GUESTS_FROM_CITY).arg(action->text()))) {
        QStringList* allLines = new QStringList(); //Your list for lines from the file.
        allLines->clear();
        while (select_guest_query.next()) {
            QStringList l;
            l.append(select_guest_query.value(0).toString());
            l.append(select_guest_query.value(1).toString());
            l.append(select_guest_query.value(2).toString());
            QString str = QString("%1\t %2 \t %3").arg(l[0]).arg(l[1]).arg(l[2]);

            allLines->append(str);
        }
        QStringListModel* linesModel = new QStringListModel(*allLines, NULL);
        linesModel->setStringList(*allLines);
        ui->guest_from_city->setModel(linesModel);
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
}

void getInfo::guest_button_changed()
{
    QAction* action = qobject_cast<QAction*>(sender());
    ui->guest_button->setText(action->text());
    _room = _currentGuestList.key(action->text());

    if (_room > 0 && _day > 0)
        findWorkerByDayAndRoom();
}

void getInfo::week_day_button_changed()
{
    QAction* action = qobject_cast<QAction*>(sender());
    ui->week_day_button->setText(action->text());
    _day = getDayInt(action->text());

    if (_room > 0 && _day > 0)
        findWorkerByDayAndRoom();
}

void getInfo::findWorkerByDayAndRoom()
{
    QSqlQuery select_worker_by_day_and_guest_query;
    if (select_worker_by_day_and_guest_query.exec(QString(SQL_SELECT_WORKER_BY_DAY_AND_GUEST).arg(_room).arg(_day))) {
        QString current_guest_list;
        while (select_worker_by_day_and_guest_query.next()) {
            current_guest_list = "  " + select_worker_by_day_and_guest_query.value(0).toString() + " " +
                select_worker_by_day_and_guest_query.value(1).toString() + " " +
                select_worker_by_day_and_guest_query.value(2).toString() + "  ";
        }
        ui->show_worker->setText(current_guest_list);
        ui->show_worker->show();
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
}

QString getInfo::getDayString(int i)
{
    switch (i) {
    case 1:
        return "Понедельник";

    case 2:
        return "Вторник";

    case 3:
        return "Среда";

    case 4:
        return "Четверг";

    case 5:
        return "Пятница";

    case 6:
        return "Суббота";

    case 7:
        return "Воскресенье";
    }
    return "";
}

int getInfo::getDayInt(QString i)
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