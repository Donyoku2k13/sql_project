#include "chooseactionadmin.h"
#include "ui_chooseactionadmin.h"

#include "workerfire.h"
#include "changeschedule.h"

#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>

#include <QErrorMessage>
#include <QString>

chooseActionAdmin::chooseActionAdmin(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::chooseActionAdmin)
{
    ui->setupUi(this);

    QSqlDatabase _db = QSqlDatabase::addDatabase("QPSQL");
    _db.setDatabaseName("hotel");
    _db.setUserName("director");
    _db.setHostName("127.0.0.1");
    _db.setPort(5432);
    _db.setPassword("director");
    if (_db.open()) {
        qDebug();
    }
    else {
        _db.lastError();
        (new QErrorMessage(this))->showMessage(QString("%1").arg(_db.lastError().text()));
    }
}

chooseActionAdmin::~chooseActionAdmin()
{
    delete ui;
}

void chooseActionAdmin::onPrevWindowClosed()
{
    this->show();
    delete sender();
}

void chooseActionAdmin::on_fire_worker_button_clicked()
{
    this->hide();
    WorkerFire* fireWorker = new WorkerFire();
    fireWorker->show();
    connect(fireWorker, SIGNAL(closed()), this, SLOT(onPrevWindowClosed()));
}

void chooseActionAdmin::on_cleanin_schedule_change_button_clicked()
{
    this->hide();
    changeSchedule* changeScheduleWindow = new changeSchedule();
    changeScheduleWindow->show();
    connect(changeScheduleWindow, SIGNAL(closed()), this, SLOT(onPrevWindowClosed()));
}