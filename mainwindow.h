#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "mandelbrot.h"
#include "position_asker.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private slots:
    void update_status_bar();
    void open_asker();
    void copy_pos();
private:
    mandelbrot image;
    QLabel x, y, scale;

    position_asker asker;
};

#endif // MAINWINDOW_H
