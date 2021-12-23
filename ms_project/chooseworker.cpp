#include "chooseworker.h"
#include "ui_chooseworker.h"

ChooseWorker::ChooseWorker(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ChooseWorker)
{
    ui->setupUi(this);
}

ChooseWorker::~ChooseWorker()
{
    delete ui;
}