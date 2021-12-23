#ifndef GETINFO_H
#define GETINFO_H

#include <QMap>
#include <QWidget>

namespace Ui {
    class getInfo;
}

class getInfo : public QWidget
{
    Q_OBJECT

public:
    explicit getInfo(QWidget* parent = nullptr);
    ~getInfo();

signals:
    void closed();

private slots:
    void on_spinBox_valueChanged(int capacity);
    void select_room_changed();
    void select_city_changed();
    void guest_button_changed();
    void week_day_button_changed();

private:
    Ui::getInfo* ui;
    QMap<QString, QList<int>> _roomsAvailable;
    QMap<int, QString> _currentGuestList;
    int _room;
    int _day;

    void findWorkerByDayAndRoom();

    QString getDayString(int i);
    int getDayInt(QString i);

    void closeEvent(QCloseEvent* event);
};

#endif // GETINFO_H