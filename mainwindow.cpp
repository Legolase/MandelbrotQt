#include "mainwindow.h"
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QClipboard>
#include <QGuiApplication>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    setCentralWidget(&image);
    centralWidget()->setMinimumSize(image.DEFAULT_WIDTH, image.DEFAULT_HEIGHT);
    //set bar
    x.setMinimumWidth(100);
    y.setMinimumWidth(100);
    scale.setMinimumWidth(100);
    statusBar()->addWidget(&x);
    statusBar()->addWidget(&y);
    statusBar()->addWidget(&scale);
    //copy
    QPushButton* copy = new QPushButton("Copy");
    statusBar()->addWidget(copy);
    connect(copy, &QPushButton::clicked, this, &MainWindow::copy_pos);
    connect(&image, &mandelbrot::info_changed, this, &MainWindow::update_status_bar);

    //set menu
    QAction* open_asker = new QAction("Teleport", this);
    QMenu* navigation = menuBar()->addMenu("Navigation");
    navigation->addAction(open_asker);
    open_asker->setShortcut(tr("CTRL+T"));
    connect(open_asker, &QAction::triggered, this, &MainWindow::open_asker);

}

void MainWindow::update_status_bar()
{
    static QString sx("x: ");
    static QString sy("y: ");
    static QString sscl("scale: ");
    image_info const& inf = image.get_info();
    x.setText(sx + QString::number(inf.start.x, 'g', 10));
    y.setText(sy + QString::number(inf.start.y, 'g', 10));
    scale.setText(sscl + QString::number(inf.scale, 'g', 10));
}

void MainWindow::open_asker()
{
    if (asker.exec()) {
        floating_t x, y, scale;
        bool ok = true;
        x = asker.x->text().toDouble(&ok);
        if (!ok) return;
        y = asker.y->text().toDouble(&ok);
        if (!ok) return;
        scale = asker.scale->text().toDouble(&ok);
        if (!ok) return;
        image_info pos = image.get_info();
        pos.start.x = x;
        pos.start.y = y;
        pos.scale = scale;
        image.set_info(pos);
    }
}

void MainWindow::copy_pos()
{
    QString s;
    image_info info = image.get_info();
    s.append(QString::number(info.start.x, 'g', 10) + "\n");
    s.append(QString::number(info.start.y, 'g', 10) + "\n");
    s.append(QString::number(info.scale, 'g', 10) + "\n");
    QGuiApplication::clipboard()->setText(s);
}
