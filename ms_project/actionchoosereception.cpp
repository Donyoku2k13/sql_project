#include "actionchoosereception.h"
#include "ui_actionchoosereception.h"

#include "addnewvisitor.h"
#include "deletevisitor.h"
#include "getinfo.h"
#include "report.h"

#include <QSqlDatabase>
#include <QErrorMessage>

actionChooseReception::actionChooseReception(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::actionChooseReception)
{
    ui->setupUi(this);

    QSqlDatabase _db = QSqlDatabase::addDatabase("QPSQL");
    _db.setDatabaseName("hotel");
    _db.setUserName("postgres");
    _db.setHostName("127.0.0.1");
    _db.setPort(5432);
    _db.setPassword("12345678");
    if (_db.open()) {
        qDebug();
    }
    else {
        _db.lastError();
        (new QErrorMessage(this))->showMessage(QString("%1").arg(_db.lastError().text()));
    }
}

actionChooseReception::~actionChooseReception()
{
    delete ui;
}

void actionChooseReception::onPrevWindowClosed()
{
    this->show();
    delete sender();
}

void actionChooseReception::on_remove_guest_clicked()
{
    this->hide();
    deleteVisitor* a = new deleteVisitor();
    a->show();
    connect(a, SIGNAL(closed()), this, SLOT(onPrevWindowClosed()));
}

void actionChooseReception::on_add_guest_clicked()
{
    this->hide();
    addNewVisitor* newVisitor = new addNewVisitor();
    newVisitor->show();
    connect(newVisitor, SIGNAL(closed()), this, SLOT(onPrevWindowClosed()));
}

void actionChooseReception::on_get_info_clicked()
{
    this->hide();
    getInfo* getInfoWindow = new getInfo();
    getInfoWindow->show();
    connect(getInfoWindow, SIGNAL(closed()), this, SLOT(onPrevWindowClosed()));
}

void actionChooseReception::on_pushButton_clicked()
{
    this->hide();
    report* reportWindow = new report();
    reportWindow->show();
    connect(reportWindow, SIGNAL(closed()), this, SLOT(onPrevWindowClosed()));
}