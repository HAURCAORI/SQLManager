#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sqlhandler.init("tcp://203.251.63.137:5000/test", "guest", "1234");
    sqlhandler.start();
    QObject::connect(this, SIGNAL(Print()), &sqlhandler, SLOT(printData()), Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void test() {
    std::cout << "a" << std::endl;
}
void MainWindow::add()
{
    sqlhandler.printTable("test",{"id", "value"});
}
