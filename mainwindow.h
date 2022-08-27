#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sqlhandler.h"

#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void add();

signals:
    void Print();

private:
    Ui::MainWindow *ui;
    SQLHandler sqlhandler;
};

#endif // MAINWINDOW_H
