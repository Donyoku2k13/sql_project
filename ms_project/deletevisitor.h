#ifndef DELETEVISITOR_H
#define DELETEVISITOR_H

#include <QWidget>
#include <QMap>

namespace Ui {
    class deleteVisitor;
}

class deleteVisitor : public QWidget
{
    Q_OBJECT

public:
    explicit deleteVisitor(QWidget* parent = nullptr);
    ~deleteVisitor();

private slots:
    void on_exitButton_clicked();

    void on_guestsList_clicked(const QModelIndex& index);

signals:
    void closed();

private:
    Ui::deleteVisitor* ui;
    void closeEvent(QCloseEvent* event);
    QMap<QString, int> _currentVisitors;
};

#endif // DELETEVISITOR_H