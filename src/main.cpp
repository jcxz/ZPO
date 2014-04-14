#include "Mesh.h"
#include "Point.h"
#include "morph.h"
#include "MeshWarpWidget.h"
#include "MainWindow.h"

#include <QApplication>
#include <QImage>
#include <iostream>



static const char *formatToString(QImage::Format fmt)
{
  #define CASE(e) case e: return #e;

  switch (fmt)
  {
    CASE(QImage::Format_Invalid);
    CASE(QImage::Format_Mono);
    CASE(QImage::Format_MonoLSB);
    CASE(QImage::Format_Indexed8);
    CASE(QImage::Format_RGB32);
    CASE(QImage::Format_ARGB32);
    CASE(QImage::Format_ARGB32_Premultiplied);
    CASE(QImage::Format_RGB16);
    CASE(QImage::Format_ARGB8565_Premultiplied);
    CASE(QImage::Format_RGB666);
    CASE(QImage::Format_ARGB6666_Premultiplied);
    CASE(QImage::Format_RGB555);
    CASE(QImage::Format_ARGB8555_Premultiplied);
    CASE(QImage::Format_RGB888);
    CASE(QImage::Format_RGB444);
    CASE(QImage::Format_ARGB4444_Premultiplied);
    default: return "Uknown format";
  }

  #undef CASE

  return "Uknown format";
}


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

  std::cout << "Source image format         : " << formatToString(img.format()) << std::endl;
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



int main(int argc, char *argv[])
{
#if 0
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
#else
  QApplication app(argc, argv);
  MainWindow w;

  w.show();

  return app.exec();
#endif
}
