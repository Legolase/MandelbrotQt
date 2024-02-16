#include "position_asker.h"
#include <QPushButton>

position_asker::position_asker(QWidget *parent)
    : QDialog{parent}
{
    x = new QLineEdit();
    y = new QLineEdit();
    scale = new QLineEdit();
    x->setPlaceholderText("x");
    y->setPlaceholderText("y");
    scale->setPlaceholderText("scale");
    vbox.setSpacing(1);
    vbox.addWidget(x);
    vbox.addWidget(y);
    vbox.addWidget(scale);

    QPushButton* ok = new QPushButton("Ok");
    QPushButton* cancel = new QPushButton("Cancel");
    QHBoxLayout* hbox = new QHBoxLayout();

    hbox->addWidget(ok, 1, Qt::AlignRight);
    hbox->addWidget(cancel, 0);
    vbox.addLayout(hbox);

    setLayout(&vbox);
    setFixedSize(220, 130);
    setWindowTitle("Teleport");
    connect(ok, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
}
