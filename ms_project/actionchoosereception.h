#pragma once
#ifndef ACTIONCHOOSERECEPTION_H
#define ACTIONCHOOSERECEPTION_H

#include <QWidget>

namespace Ui {
    class actionChooseReception;
}

class actionChooseReception : public QWidget
{
    Q_OBJECT

public:
    explicit actionChooseReception(QWidget* parent = nullptr);
    ~actionChooseReception();

private:
    Ui::actionChooseReception* ui;

private slots:
    void onPrevWindowClosed();

    void on_remove_guest_clicked();

    void on_add_guest_clicked();

    void on_get_info_clicked();
    void on_pushButton_clicked();
};

#endif // ACTIONCHOOSERECEPTION_H