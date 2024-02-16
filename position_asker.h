#ifndef POSITION_ASKER_H
#define POSITION_ASKER_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <memory>

class position_asker : public QDialog
{
    Q_OBJECT
public:
    explicit position_asker(QWidget *parent = nullptr);

    QLineEdit* x, *y, *scale;
private:
    QVBoxLayout vbox;
};

#endif // POSITION_ASKER_H
