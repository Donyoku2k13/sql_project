#ifndef ADDCOMPANION_H
#define ADDCOMPANION_H

#include <QWidget>

namespace Ui {
    class addCompanion;
}

class addCompanion : public QWidget
{
    Q_OBJECT

public:
    addCompanion(int guest_counter,
        QString& companion_first_name,
        QString& companion_middle_name,
        QString& companion_last_name,
        QString& companion_passport,
        QWidget* parent = nullptr);
    ~addCompanion();

private:
    Ui::addCompanion* ui;
    void closeEvent(QCloseEvent* event);
    QString& _companion_first_name;
    QString& _companion_middle_name;
    QString& _companion_last_name;
    QString& _companion_passport;

signals:
    void closed();
private slots:
    void on_pushButton_clicked();
};

#endif // ADDCOMPANION_H