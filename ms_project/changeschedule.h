#ifndef CHANGESCHEDULE_H
#define CHANGESCHEDULE_H

#include <QSqlTableModel>
#include <QWidget>

namespace Ui {
    class changeSchedule;
}

class changeSchedule : public QWidget
{
    Q_OBJECT

public:
    explicit changeSchedule(QWidget* parent = nullptr);
    ~changeSchedule();

private:
    Ui::changeSchedule* ui;
    void closeEvent(QCloseEvent* event);
    int getDayInt(QString i);
    QSqlTableModel* _model;

signals:
    void closed();

private slots:
    void on_tableView_doubleClicked(const QModelIndex& index);
    void prevIsClosed(bool isOK);
};

#endif // CHANGESCHEDULE_H