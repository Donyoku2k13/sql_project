#include "deletevisitor.h"
#include "ui_deletevisitor.h"

#include <QDebug>
#include <QSqlQuery>
#include <QStringListModel>

#define SQL_SELECT_GUEST_LIST "SELECT guest.first_name, guest.middle_name, guest.last_name, room.id FROM guest, current_guest_list, room WHERE current_guest_list.guest_id = guest.passport AND current_guest_list.room_id = room.id"
#define SQL_DELETE_GUEST "DELETE FROM current_guest_list WHERE room_id = %1; UPDATE room SET is_available = TRUE WHERE id = %1"

deleteVisitor::deleteVisitor(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::deleteVisitor)
{
    ui->setupUi(this);
    ui->exitButton->setEnabled(false);

    QSqlQuery query;
    if (query.exec(SQL_SELECT_GUEST_LIST)) {
        QStringList* allLines = new QStringList(); //Your list for lines from the file.
        allLines->clear();
        while (query.next()) {

            QStringList l;
            l.append(query.value(0).toString());
            l.append(query.value(1).toString());
            l.append(query.value(2).toString());
            l.append(query.value(3).toString());
            QString str = QString("%1\t\t%2 \t\t%3    \t%4").arg(l[0]).arg(l[1]).arg(l[2]).arg(l[3]);
            _currentVisitors.insert(str, l[3].toInt());

            allLines->append(str);
        }
        QStringListModel* linesModel = new QStringListModel(*allLines, NULL);
        linesModel->setStringList(*allLines);
        ui->guestsList->setModel(linesModel);
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
}

deleteVisitor::~deleteVisitor()
{
    delete ui;
}

void deleteVisitor::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    emit this->closed();
}

void deleteVisitor::on_exitButton_clicked()
{
    QSqlQuery deleteQuery;
    int room_id;
    if (!ui->guestsList->selectionModel()->selectedRows().empty()) {
        QString str = ui->guestsList->selectionModel()->selectedRows()[0].data().toString();
        room_id = _currentVisitors.value(str);
        if (!deleteQuery.exec(QString(SQL_DELETE_GUEST).arg(room_id)))
            qCritical() << qPrintable("Failed executing SQL-queryà");
    }

    QSqlQuery query;
    if (query.exec(SQL_SELECT_GUEST_LIST)) {
        QStringList* allLines = new QStringList(); //Your list for lines from the file.
        allLines->clear();
        while (query.next()) {

            QStringList l;
            l.append(query.value(0).toString());
            l.append(query.value(1).toString());
            l.append(query.value(2).toString());
            l.append(query.value(3).toString());
            QString str = QString("%1\t\t%2 \t\t%3    \t%4").arg(l[0]).arg(l[1]).arg(l[2]).arg(l[3]);
            _currentVisitors.insert(str, l[3].toInt());

            allLines->append(str);
        }
        QStringListModel* linesModel = new QStringListModel(*allLines, NULL);
        linesModel->setStringList(*allLines);
        ui->guestsList->setModel(linesModel);
    }
    else {
        qCritical() << "Failed executing SQL-query";
    }
}

void deleteVisitor::on_guestsList_clicked(const QModelIndex& index)
{
    ui->exitButton->setEnabled(true);
}