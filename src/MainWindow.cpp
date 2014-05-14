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
  //ui->srcWarp->setImage("D:\\AC601\\obrazky\\PasoveFoto\\orezane.jpg");
  //ui->dstWarp->setImage("D:\\AC601\\obrazky\\Katka.png");
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
                            ui->sbFrameCount->value());

  MoviePlayerWindow *player = new MoviePlayerWindow(movie);
  player->setAttribute(Qt::WA_DeleteOnClose);
  player->activateWindow();  // this brings the window to front
  player->show();
}


void MainWindow::loadProject(void)
{
  QString filename(QFileDialog::getOpenFileName(this, tr("Morph project")));
  if (filename.isNull()) return;

  QImage src_img;
  QImage dst_img;
  Mesh src_mesh;
  Mesh dst_mesh;

  if (!loadMorphProject(filename, src_img, src_mesh, dst_img, dst_mesh))
  {
    QMessageBox::critical(this, tr("Error"), tr("Failed to load the morph project"));
  }

  ui->srcWarp->set(src_img, src_mesh);
  ui->dstWarp->set(dst_img, dst_mesh);
}


void MainWindow::saveProject(void)
{
  QString filename(QFileDialog::getSaveFileName(this, tr("Morph project"),
                                                "./project.morph",
                                                tr("Morph Files (*.morph);;All Files (*.*)")));
  if (filename.isNull()) return;

  if (!saveMorphProject(filename, ui->srcWarp->image(), ui->srcWarp->mesh(),
                        ui->dstWarp->image(), ui->dstWarp->mesh()))
  {
    QMessageBox::critical(this, tr("Error"), tr("Failed to load the morph project"));
  }
}


void MainWindow::loadSourceImage(void)
{
  QString filename(QFileDialog::getOpenFileName(this, tr("Source image")));
  if (filename.isNull()) return;

  QImage img(filename);
  if (img.isNull())
  {
    QMessageBox::critical(this, tr("Error"), tr("Failed to load the image"));
    return;
  }

  ui->srcWarp->setImage(img);
  ui->sbSrcWidth->setValue(img.width());
  ui->sbSrcHeigth->setValue(img.height());
}


void MainWindow::loadDestinationImage(void)
{
  QString filename(QFileDialog::getOpenFileName(this, tr("Source image")));
  if (filename.isNull()) return;

  QImage img(filename);
  if (img.isNull())
  {
    QMessageBox::critical(this, tr("Error"), tr("Failed to load the image"));
    return;
  }

  ui->dstWarp->setImage(img);
  ui->sbDstWidth->setValue(img.width());
  ui->sbDstHeigth->setValue(img.height());
}


void MainWindow::scaleImages(void)
{
  const QImage & src = ui->srcWarp->image();
  const QImage & dst = ui->dstWarp->image();

  int src_w = ui->sbSrcWidth->value();
  int src_h = ui->sbSrcHeigth->value();
  int dst_w = ui->sbDstWidth->value();
  int dst_h = ui->sbDstHeigth->value();

  if (ui->rbCustom->isChecked())
  {
    if ((src_w != dst_w) || (src_h != dst_h))
    {
      QMessageBox::warning(this, tr("Error"),
                           tr("The width or the heigth of the destination and the source image do not agree"));
      return;
    }
  }
  else
  {
    src_w = (src_w + dst_w) / 2;
    src_h = (src_h + dst_h) / 2;
  }

  ui->srcWarp->setImage(src.scaled(src_w, src_h));
  ui->dstWarp->setImage(dst.scaled(src_w, src_h));

  ui->sbSrcWidth->setValue(src_w);
  ui->sbSrcHeigth->setValue(src_h);
  ui->sbDstWidth->setValue(src_w);
  ui->sbDstHeigth->setValue(src_h);
}
