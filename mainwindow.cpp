#include "mainwindow.h"
#include "ui_mainwindow.h"

void test() {
    std::cout << "a" << std::endl;
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    sqlhandler.start();
    sqlhandler.connect("tcp://203.251.63.137:5000/test", "guest", "1234");
    sqlhandler.EnqueueJob(test);
    sqlhandler.printTable("test",{"id", "value"});
    QObject::connect(this, SIGNAL(testSignal()), &sqlhandler, SLOT(testSlot()), Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add()
{
    sqlhandler.printTable("test",{"id", "value"});
}
