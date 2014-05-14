#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "morph.h"
#include "MoviePlayerWindow.h"


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  //ui->widget->setStyleSheet("background-color: red;");
  //ui->srcWarp->setImage("D:\\AC601\\obrazky\\PasoveFoto\\orezane.jpg");
  //ui->dstWarp->setImage("D:\\AC601\\obrazky\\Katka.png");
  //ui->dstWarp->setImage("D:\\AC601\\obrazky\\Fun\\cow_costume.jpg");
  ui->srcWarp->setImage("data/Man.jpg");
  ui->dstWarp->setImage("data/Woman.jpg");
}


MainWindow::~MainWindow(void)
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  //morph(ui->srcWarp->image(), ui->srcWarp->mesh(),
  //      ui->dstWarp->image(), ui->dstWarp->mesh(),
  //      100);

  MoviePlayerWindow *player = new MoviePlayerWindow;
  player->setAttribute(Qt::WA_DeleteOnClose);
  player->show();
}
