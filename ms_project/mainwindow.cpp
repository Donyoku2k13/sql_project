#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "actionchoosereception.h"
#include "chooseactionadmin.h"

#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>

#include <QErrorMessage>
#include <QString>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_admin_button_clicked()
{
    chooseActionAdmin* adminAction = new chooseActionAdmin();
    adminAction->show();
    this->close();
}

void MainWindow::on_reception_button_clicked()
{
    actionChooseReception* receptionAction = new actionChooseReception();
    receptionAction->show();
    this->close();
}