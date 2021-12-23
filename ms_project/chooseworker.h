#ifndef CHOOSEWORKER_H
#define CHOOSEWORKER_H

#include <QWidget>

namespace Ui {
    class ChooseWorker;
}

class ChooseWorker : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseWorker(QWidget* parent = nullptr);
    ~ChooseWorker();

private:
    Ui::ChooseWorker* ui;
};

#endif // CHOOSEWORKER_H