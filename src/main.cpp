#include "Mesh.h"
#include "Point.h"
#include "morph.h"
#include "Movie.h"
#include "MeshWarpWidget.h"
#include "MainWindow.h"
#include "MoviePlayerWindow.h"
#include "utils.h"

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <iostream>



static bool testWarping(void)
{
  // load the input image
  QImage img("D:\\AC601\\ostatne\\Programovanie\\C_and_C++\\Qt_projects\\MeshWarping\\Face360.BMP");
  //QImage img("D:\\AC601\\obrazky\\PasoveFoto\\orezane.jpg");
  if (img.isNull())
  {
    std::cerr << "Failed to load input image" << std::endl;
    return false;
  }

  std::cout << "Source image format         : " << utils::qImageFormatToString(img.format()) << std::endl;
  std::cout << "Source image width x height : " << img.width() << " x " << img.height() << std::endl;

  // set up src mesh
  Mesh mesh_src(7, 7, img.width(), img.height());
  Mesh mesh_dst(7, 7, img.width(), img.height());

  // move some points in the dst mesh
  //mesh_dst(2, 3).x -= 10;
  //mesh_dst(4, 3).x += 10;
  //mesh_dst(3, 2).y -= 10;
  //mesh_dst(3, 4).y += 10;

  mesh_dst(2, 3).x -= 20;
  mesh_dst(4, 3).x += 20;
  mesh_dst(3, 2).y -= 20;
  mesh_dst(3, 4).y += 20;

  //mesh_dst(2, 3).x -= 20;
  //mesh_dst(4, 3).x += 20;
  //mesh_dst(2, 3).x -= 30;
  //mesh_dst(4, 3).x += 30;

  // print both meshes (for debugging)
  std::cout << "Source mesh: " << std::endl;
  std::cout << mesh_src << std::endl;
  std::cout << "Destination mesh: " << std::endl;
  std::cout << mesh_dst << std::endl;

  // perform warping
  QImage res(warp(img, mesh_src, mesh_dst));
  if (res.isNull())
  {
    std::cerr << "Failed to warp the given image" << std::endl;
    return false;
  }

  // write the output
  if (!res.save("Face360_warped.bmp"))
  //if (!res.save("PasoveFoto_warped.bmp"))
  {
    std::cerr << "Failed to save the resulting warped image" << std::endl;
    return false;
  }

  return true;
}


void genMovie(int frame_w, int frame_h, QImage::Format frame_fmt,
              int frames_count, Movie & movie, bool save_frames = false)
{
  INFOM("Generating frames ...");

  int argc = 0;
  char **argv = nullptr;
  QGuiApplication app(argc, argv);
  QPainter painter;
  for (int i = 0; i < frames_count; ++i)
  {
    QImage frame(frame_w, frame_h, frame_fmt);

    frame.fill(Qt::red);

    painter.begin(&frame);
    painter.drawText(10, 30, QString("frame #%1").arg(i));
    painter.end();

    if (save_frames)
    {
      QString frame_filename(QString("output/testMovie/generated_%1.jpg").arg(i));
      if (!frame.save(frame_filename))
      {
        WARNM("Failed to save generated frame #" << i <<
              " to " << frame_filename.toStdString());
      }
    }

    movie.addFrame(frame);
  }

  INFOM("Frames generated");
}


static bool testMovie(void)
{
  constexpr int frame_w = 100;
  constexpr int frame_h = 100;
  constexpr QImage::Format frame_fmt = QImage::Format_ARGB32;

  Movie movie;

  genMovie(frame_w, frame_h, frame_fmt, 100, movie, true);

  INFOM("Saving as AVI");

  if (!movie.saveAsAVI("output/testMovie/generated.avi"))
  {
    ERRORM("Failed to save frames to file output/testMovie/generated.avi");
    return false;
  }

  INFOM("AVI saved");

  return true;
}



int main(int argc, char *argv[])
{
#if 0
  testMovie();
  return 1;
#elif 0
  //Mesh test(0.3f, 0.5f, 10, 10);
  //std::cout << "test mesh: " << test << std::endl;
  testWarping();
  return 1;
#elif 0
  QApplication app(argc, argv);

  //QImage img("Face360.BMP");
  QImage img("D:\\AC601\\obrazky\\PasoveFoto\\orezane.jpg");
  if (img.isNull())
  {
    std::cerr << "Failed to load input image" << std::endl;
    return 1;
  }

  MeshWarpWidget w(img);

  w.show();

  return app.exec();
#elif 0
  QApplication app(argc, argv);

  MoviePlayerWindow player;

  Movie *movie = new Movie;
  genMovie(300, 300, QImage::Format_ARGB32, 100, *movie);

  player.setMovie(movie);

  player.show();

  return app.exec();
#else
  QApplication app(argc, argv);
  MainWindow w;

  QFile style_sheet(":/style.qss");
  if (!style_sheet.open(QFile::ReadOnly))
  {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
                          QObject::tr("Failed to load stylesheet."));
    return 1;
  }

  app.setStyleSheet(style_sheet.readAll());

  w.show();
  //w.showMaximized();

  return app.exec();
#endif
}
