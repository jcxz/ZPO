/*
 * Copyright (C) 2014 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
 *
 * This file is part of ZPO.
 *
 * ZPO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ZPO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ZPO. If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "morph.h"
#include "MoviePlayerWindow.h"
#include "Movie.h"

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

  ui->progressBar->hide();
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

  if ((ui->srcWarp->isEmpty()) || (ui->dstWarp->isEmpty()))
  {
    QMessageBox::warning(this, tr("Error"), tr("Nothing to be morphed"));
    return;
  }

  if (ui->srcWarp->image().size() != ui->dstWarp->image().size())
  {
    QMessageBox::warning(this, tr("Error"),
                         tr("Images to be morphed do not have equal dimensions."
                            "Use the scale button to make their sizes equal."));
    return;
  }

  //Movie *movie = morphMovie(ui->srcWarp->image(), ui->srcWarp->mesh(),
  //                          ui->dstWarp->image(), ui->dstWarp->mesh(),
  //                          ui->sbFrameCount->value());

  Movie *movie = morphMovie(ui->dstWarp->image(), ui->dstWarp->mesh(),
                            ui->srcWarp->image(), ui->srcWarp->mesh(),
                            ui->sbFrameCount->value());
  if (movie == nullptr)
  {
    QMessageBox::critical(this, tr("Error"),
                         tr("Failed to morph images"));
    return;
  }

  MoviePlayerWindow *player = new MoviePlayerWindow(movie);
  player->setAttribute(Qt::WA_DeleteOnClose);
  player->activateWindow();  // this brings the window to front
  player->show();
}


Movie *MainWindow::morphMovie(const QImage & src_img, const Mesh & src_mesh,
                              const QImage & dst_img, const Mesh & dst_mesh,
                              int nframes)
{
  if (!src_mesh.hasSameDimensions(dst_mesh))
  {
    ERRORM("source mesh has different dimensions than destination mesh");
    return nullptr;
  }

  Mesh tmp_mesh(src_mesh.sizeX(), src_mesh.sizeY(), src_mesh.width(), src_mesh.heigth());

  Movie *movie = new Movie;

  ui->progressBar->setMaximum(nframes);
  ui->progressBar->show();

  for (int i = 0; i < nframes; ++i)
  {
    float t = float(i) / (float(nframes - 1));

    QImage frame(morphFrame(src_img, src_mesh, dst_img, dst_mesh, tmp_mesh, t));
    if (frame.isNull())
    {
      WARNM("Failed to morph frame n. " << i);
    }

    movie->addFrame(frame);

    ui->progressBar->setValue(i);
  }

  ui->progressBar->hide();

  return movie;
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

  ui->sbSrcWidth->setValue(src_img.width());
  ui->sbSrcHeigth->setValue(src_img.height());
  ui->sbDstWidth->setValue(dst_img.width());
  ui->sbDstHeigth->setValue(dst_img.height());
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
