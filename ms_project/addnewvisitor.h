#ifndef ADDNEWVISITOR_H
#define ADDNEWVISITOR_H

#include <QWidget>

#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

namespace Ui {
    class addNewVisitor;
}

class addNewVisitor : public QWidget
{
    Q_OBJECT

public:
    addNewVisitor(QWidget* parent = nullptr);
    ~addNewVisitor();

private:
    Ui::addNewVisitor* ui;
    void closeEvent(QCloseEvent* event);
    void makeBill();
    QMap<QString, QList<int>> _roomsAvailable;

    QString _guest_first_name;
    QString _guest_middle_name;
    QString _guest_last_name;
    QString _guest_passport;
    int _guest_id;
    int _guest_count;
    int _room_id;
    QString _companion_1_first_name;
    QString _companion_1_middle_name;
    QString _companion_1_last_name;
    QString _companion_1_passport;
    QString _companion_2_first_name;
    QString _companion_2_middle_name;
    QString _companion_2_last_name;
    QString _companion_2_passport;
    int _duration;
    QString _city;

signals:
    void closed();

private slots:
    void finishWork();
    void on_spinBox_valueChanged(int capacity);
    void on_pushButton_clicked();
};

#endif // ADDNEWVISITOR_H