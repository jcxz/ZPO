#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "morph.h"



MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  //ui->widget->setStyleSheet("background-color: red;");
  ui->srcWarp->setImage("data/Man.jpg");
  ui->dstWarp->setImage("data/Woman.jpg");
}


MainWindow::~MainWindow(void)
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  morph(ui->srcWarp->image(), ui->srcWarp->mesh(),
        ui->dstWarp->image(), ui->dstWarp->mesh(),
        100);
}
