#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "morph.h"
#include "MoviePlayerWindow.h"

#include <QFileDialog>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  //ui->widget->setStyleSheet("background-color: red;");
  ui->srcWarp->setImage("D:\\AC601\\obrazky\\PasoveFoto\\orezane.jpg");
  ui->dstWarp->setImage("D:\\AC601\\obrazky\\Katka.png");
  //ui->dstWarp->setImage("D:\\AC601\\obrazky\\Fun\\cow_costume.jpg");
  //ui->srcWarp->setImage("data/Man.jpg");
  //ui->dstWarp->setImage("data/Woman.jpg");

  // synchronization from source warp widget to destination warp widget
  connect(ui->srcWarp, SIGNAL(activeCPChanged(int,int)),
          ui->dstWarp, SLOT(handleCPChange(int,int)));
  connect(ui->srcWarp, SIGNAL(activeCPDeactivated()),
          ui->dstWarp, SLOT(handleCPDeactivation()));

  // synchronization between widget theother way around
  connect(ui->dstWarp, SIGNAL(activeCPChanged(int,int)),
          ui->srcWarp, SLOT(handleCPChange(int,int)));
  connect(ui->dstWarp, SIGNAL(activeCPDeactivated()),
          ui->srcWarp, SLOT(handleCPDeactivation()));
}


MainWindow::~MainWindow(void)
{
  delete ui;
}


void MainWindow::morph(void)
{
  //morph(ui->srcWarp->image(), ui->srcWarp->mesh(),
  //      ui->dstWarp->image(), ui->dstWarp->mesh(),
  //      100);

  Movie *movie = morphMovie(ui->srcWarp->image(), ui->srcWarp->mesh(),
                            ui->dstWarp->image(), ui->dstWarp->mesh(),
                            100);

  MoviePlayerWindow *player = new MoviePlayerWindow(movie);
  player->setAttribute(Qt::WA_DeleteOnClose);
  player->activateWindow();  // this brings the window to front
  player->show();
}


void MainWindow::loadSourceImage(void)
{
  QString filename(QFileDialog::getOpenFileName(this, tr("Source image")));
  if (filename.isNull()) return;

  if (!ui->srcWarp->setImage(filename))
  {
    QMessageBox::critical(this, tr("Error"), tr("Failed to load the image"));
  }
}


void MainWindow::loadDestinationImage(void)
{
  QString filename(QFileDialog::getOpenFileName(this, tr("Destination image")));
  if (filename.isNull()) return;

  if (!ui->dstWarp->setImage(filename))
  {
    QMessageBox::critical(this, tr("Error"), tr("Failed to load the image"));
  }
}
