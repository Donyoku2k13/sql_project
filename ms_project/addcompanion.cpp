#include "addcompanion.h"
#include "ui_addcompanion.h"

#include <QErrorMessage>

addCompanion::addCompanion(int guest_counter, QString& companion_first_name,
    QString& companion_middle_name, QString& companion_last_name,
    QString& companion_passport, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::addCompanion),
    _companion_first_name(companion_first_name),
    _companion_middle_name(companion_middle_name),
    _companion_last_name(companion_last_name),
    _companion_passport(companion_passport)

{
    ui->setupUi(this);
    if (guest_counter != -1)
        ui->label_5->setText(QString("Добавление сопровождающего №%1").arg(guest_counter));
    ui->label_5->setStyleSheet(QString("font-size:20pt; font-weight:600; color: white"));

}

addCompanion::~addCompanion()
{
    delete ui;
}

void addCompanion::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    emit this->closed();
}

void addCompanion::on_pushButton_clicked()
{
    _companion_first_name = ui->first_name->text();
    _companion_middle_name = ui->middle_name->text();
    _companion_last_name = ui->last_name->text();
    _companion_passport = ui->passport->text();
    if ((_companion_first_name == QString() ||
        _companion_middle_name == QString() ||
        _companion_last_name == QString() ||
        _companion_passport == QString()) &&
        (_companion_first_name != QString() ||
            _companion_middle_name != QString() ||
            _companion_last_name != QString() ||
            _companion_passport != QString())
        ) {
        (new QErrorMessage(this))->showMessage("Не все поля заполнены!");
    }
    else {
        this->close();
    }
}