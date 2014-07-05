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

#include "Morpher.h"
#include "Point.h"
#include "Mesh.h"
#include "debug.h"

#include <memory>
//#include <vector>

#include <QImage>


///////////////////////////////////// Debugging Functions ///////////////////////////////////////////////

#ifdef MORPHER_DEBUG
static void printIntercepts(int *src_row_intercepts, int *dst_row_intercepts, int img_h, int mesh_w)
{
  std::cout << "[mesh_width, img_height]" << std::endl;

  for (int j = 0; j < mesh_w; ++j)
  {
    int range_start = j * img_h;
    for (int i = 0; i < img_h; ++i)
    {
      int range_end = j * img_h + i;//range_start + 1;
      std::cout << "[" << j << "," << i << "]: src[" << src_row_intercepts[range_start] << ", " << src_row_intercepts[range_end]
                << "] dst[" << dst_row_intercepts[range_start] << ", " << dst_row_intercepts[range_end] << "]"
                << std::endl;
      range_start = range_end;
    }
  }
}


// tato funkcia zobrazi priesecniky vertkalnych polylines s riadkami obrazu
static void displayVerticalPolyLineRowIntersections(uint32_t *img, int img_w, int img_h, int mesh_w, int *row_intercepts)
{
  for (int j = 0; j < mesh_w; ++j)
  {
    for (int i = 0; i < img_h; ++i)
    {
      int x = row_intercepts[j * img_h + i];
      if (x == img_w)
        x = img_w - 1;
      else if (x > img_w)
      {
        std::cerr << "ERROR: Cannot display [" << x << ", " << i << "]" << std::endl;
        return;
      }
      img[i * img_w + x] = qRgb(255, 0, 0);
    }
  }
}


// tato funklcia zobrazi priesecniky horizontalnych polylines so stlpcami obrazu
static void displayHorizontalPolyLineColumnIntersections(uint32_t *img, int img_w, int img_h, int mesh_h, int *col_intercepts)
{
  for (int j = 0; j < mesh_h; ++j)    // pre kazdy horizontalny polyline
  {
    for (int i = 0; i < img_w; ++i)   // pre kazdy bod na riadku
    {
      int y = col_intercepts[j * img_w + i];
      if (y == img_h)
        y = img_h - 1;
      else if (y > img_h)
      {
        std::cerr << "ERROR: Cannot display [" << i << ", " << y << "]" << std::endl;
        return;
      }
      img[y * img_w + i] = qRgb(255, 0, 0);
    }
  }
}
#endif

/////////////////////////////////////////// Helper functions for warping ////////////////////////////////////////////////

static void calcVerticalPolyLineRowIntersections(const Mesh & mesh, int mesh_col, int num_y_vals, int *intersections)
{
  Point cp1 = mesh(mesh_col, 0);
  Point cp2 = mesh(mesh_col, 1);
  int mesh_row = 1;

  for (int y = 0; y < num_y_vals; ++y)
  {
    while (y > cp2.y)
    {
      cp1 = mesh(mesh_col, mesh_row);
      mesh_row++;
      cp2 = mesh(mesh_col, mesh_row);
    }

    float cp1_x = cp1.x;
    float cp1_y = cp1.y;
    float cp2_x = cp2.x;
    float cp2_y = cp2.y;

    //std::cout << "y: " << y << ", cp1: " << cp1 << ", cp2: " << cp2 << std::endl;

    float x = cp1_x + (cp2_x - cp1_x) / (cp2_y - cp1_y) * (y - cp1_y);
    intersections[y] = int(x);
  }
}


static void calcHorizontalPolyLineColumnIntersections(const Mesh & mesh, int mesh_row, int num_x_vals, int *intersections)
{
  Point cp1 = mesh(0, mesh_row);
  Point cp2 = mesh(1, mesh_row);
  int mesh_col = 1;

  for (int x = 0; x < num_x_vals; ++x)
  {
    while (x > cp2.x)
    {
      cp1 = mesh(mesh_col, mesh_row);
      mesh_col++;
      cp2 = mesh(mesh_col, mesh_row);
    }

    float cp1_x = cp1.x;
    float cp1_y = cp1.y;
    float cp2_x = cp2.x;
    float cp2_y = cp2.y;

    //std::cout << "x: " << x << ", cp1: " << *cp1 << ", cp2: " << *cp2 << std::endl;

    float y = cp1_y + (cp2_y - cp1_y) / (cp2_x - cp1_x) * (x - cp1_x);
    intersections[x] = int(y);
  }
}


static void resampleRow(const uint32_t *src_row, int src_start, int src_end,
                              uint32_t *dst_row, int dst_start, int dst_end)
{
  int src_range = src_end - src_start;
  int dst_range = dst_end - dst_start;
  float ratio = float(src_range) / float(dst_range);

  for (int i = 0; i < dst_range; ++i)
  {
    dst_row[i + dst_start] = src_row[int(i * ratio) + src_start];
  }
}


static void resampleColumn(const uint32_t *src, int src_start, int src_end,
                                 uint32_t *dst, int dst_start, int dst_end,
                           int img_w, int img_x)
{
  int src_range = src_end - src_start;
  int dst_range = dst_end - dst_start;
  float ratio = float(src_range) / float(dst_range);

  for (int i = 0; i < dst_range; ++i)
  {
    dst[(i + dst_start) * img_w + img_x] = src[(int(i * ratio) + src_start) * img_w + img_x];
  }
}


static void warp_impl(const QImage & src_img, const Mesh & src_mesh, const Mesh & dst_mesh, QImage & tmp_img, QImage & dst_img, int img_w, int img_h)
{
  // overit, ze mesh nema viac control pointov (vacsiu velkost) ako obrazok
  int mesh_w = src_mesh.sizeX();

  // priesecniky su ulozene po splajnoch
  std::unique_ptr<int[]> src_row_intercepts(new int[img_h * mesh_w]);
  std::unique_ptr<int[]> dst_row_intercepts(new int[img_h * mesh_w]);

  // zisti priesecniky vertikalnych polylines s riadkami obrazku
  // vertikalnych polylines je vzdy maximalne mesh_w
  {
    int *src_row_intercepts_ptr = src_row_intercepts.get();
    int *dst_row_intercepts_ptr = dst_row_intercepts.get();
    for (int i = 0; i < mesh_w; ++i)
    {
      calcVerticalPolyLineRowIntersections(src_mesh, i, img_h, src_row_intercepts_ptr);
      calcVerticalPolyLineRowIntersections(dst_mesh, i, img_h, dst_row_intercepts_ptr);
      src_row_intercepts_ptr += img_h;
      dst_row_intercepts_ptr += img_h;
    }
  }

  // resampluj zistene intervaly priesecnikov medzi src_mesh a dst_mesh
  {
    uint32_t *src_img_pixels = (uint32_t *) src_img.bits();
    uint32_t *dst_img_pixels = (uint32_t *) tmp_img.bits();
    for (int i = 0; i < img_h; ++i)
    {
      //std::cout << std::setw(3) << i << ": ";
      for (int j = 1; j < mesh_w; ++j)
      {
        int range_start = (j - 1) * img_h + i;
        int range_end = j * img_h + i;
        //std::cout << "src[" << src_row_intercepts[range_start] << ", " << src_row_intercepts[range_end] << "] dst["
        //                    << dst_row_intercepts[range_start] << ", " << dst_row_intercepts[range_end] << "] ";

        //std::cout << " dst[" << dst_row_intercepts[range_start] << ", " << dst_row_intercepts[range_end] << "] ";

        resampleRow(src_img_pixels, src_row_intercepts[range_start], src_row_intercepts[range_end],
                    dst_img_pixels, dst_row_intercepts[range_start], dst_row_intercepts[range_end]);
      }
      //std::cout << std::endl;
      src_img_pixels += img_w;
      dst_img_pixels += img_w;
    }
  }

#if 0
  displayVerticalPolyLineRowIntersections((uint32_t *) dst_img.bits(), img_w, img_h, mesh_w, dst_row_intercepts.get());
#endif

  ////////////////////////////// druha cast algoritmu (warping po stlpcoch) ///////////////////////////////

  int mesh_h = src_mesh.sizeY();

  // priesecniky su ulozene po splajnoch
  std::unique_ptr<int[]> src_col_intercepts(new int[img_w * mesh_h]);
  std::unique_ptr<int[]> dst_col_intercepts(new int[img_w * mesh_h]);

  // zisti priesecniky horizontalnych polylines so stlpcami obrazku
  // horizontalnych polylines je vzdy maximalne mesh_h
  {
    int *src_col_intercepts_ptr = src_col_intercepts.get();
    int *dst_col_intercepts_ptr = dst_col_intercepts.get();
    for (int i = 0; i < mesh_h; ++i)
    {
      calcHorizontalPolyLineColumnIntersections(src_mesh, i, img_w, src_col_intercepts_ptr);
      calcHorizontalPolyLineColumnIntersections(dst_mesh, i, img_w, dst_col_intercepts_ptr);
      src_col_intercepts_ptr += img_w;
      dst_col_intercepts_ptr += img_w;
    }
  }

  // resampluj zistene intervaly priesecnikov medzi src_mesh a dst_mesh
  {
    uint32_t *src_img_pixels = (uint32_t *) tmp_img.bits();
    uint32_t *dst_img_pixels = (uint32_t *) dst_img.bits();
    for (int i = 0; i < img_w; ++i)
    {
      //std::cout << std::setw(3) << i << ": ";
      for (int j = 1; j < mesh_h; ++j)
      {
        int range_start = (j - 1) * img_w + i;
        int range_end = j * img_w + i;
        //std::cout << "src[" << src_col_intercepts[range_start] << ", " << src_col_intercepts[range_end] << "] dst["
        //                    << dst_col_intercepts[range_start] << ", " << dst_col_intercepts[range_end] << "] ";

        //std::cout << " dst[" << dst_col_intercepts[range_start] << ", " << dst_col_intercepts[range_end] << "] ";

        resampleColumn(src_img_pixels, src_col_intercepts[range_start], src_col_intercepts[range_end],
                       dst_img_pixels, dst_col_intercepts[range_start], dst_col_intercepts[range_end],
                       img_w, i);
      }
      //std::cout << std::endl;
      //src_img_pixels += img_w;
      //dst_img_pixels += img_w;
    }
  }

#if 0
  displayHorizontalPolyLineColumnIntersections((uint32_t *) dst_img.bits(), img_w, img_h, mesh_h, dst_col_intercepts.get());
#endif
}


////////////////////////////////////// Warping (Public interface) /////////////////////////////////////////////////////////

QImage Morpher::warp(const QImage & src_img, const Mesh & src_mesh, const Mesh & dst_mesh)
{
  // check on preconditions
  if (!src_mesh.hasSameDimensions(dst_mesh))
  {
    std::cerr << "WARNING: source and destination mesh must have equal dimensions" << std::endl;
    return QImage();
  }

  if (!src_mesh.hasValidDimensions())
  {
    std::cerr << "WARNING: source and destination mesh have invalid dimensions" << std::endl;
    return QImage();
  }

  int img_w = src_img.width();
  int img_h = src_img.height();
  if ((!src_mesh.isResampledTo(img_w, img_h)) || (!dst_mesh.isResampledTo(img_w, img_h)))
  {
    std::cerr << "WARNING: The source or the destination mesh is not properly stretched to match dimensions of the input image" << std::endl;
    return QImage();
  }

  QImage::Format src_img_format = src_img.format();
  if ((src_img_format != QImage::Format_ARGB32) && (src_img_format != QImage::Format_RGB32))
  {
    std::cerr << "WARNING: The input image has to be in ARGB32 format" << std::endl;
    return QImage();
  }

  // allocate destination image
  QImage dst_img(img_w, img_h, src_img_format);
  if (dst_img.isNull())
  {
    std::cerr << "WARNING: Failed to allocate destination image" << std::endl;
    return dst_img;
  }

  // allocate temporary helper image
  QImage tmp_img(img_w, img_h, src_img_format);
  if (tmp_img.isNull())
  {
    std::cerr << "WARNING: Failed to allocate temporary helper image" << std::endl;
    return tmp_img;
  }

  warp_impl(src_img, src_mesh, dst_mesh, tmp_img, dst_img, img_w, img_h);

  return dst_img;
}


////////////////////////////////////// Morphing (Public interface) /////////////////////////////////////////////////////////

QImage Morpher::morphFrame(const QImage & src_img, const Mesh & src_mesh,
                           const QImage & dst_img, const Mesh & dst_mesh,
                           Mesh & tmp_mesh, float t)
{
  tmp_mesh.interpolate(src_mesh, dst_mesh, t);

  QImage img1(warp(src_img, src_mesh, tmp_mesh));
  if (img1.isNull())
  {
    std::cerr << "Failed to warp source to temporary"  << std::endl;
    return QImage();
  }

  QImage img2(warp(dst_img, dst_mesh, tmp_mesh));
  if (img2.isNull())
  {
    std::cerr << "Failed to warp destination to temporary"  << std::endl;
    return QImage();
  }

  assert(img1.size() == img2.size());

  int img_w = img1.width();
  int img_h = img1.height();
  int cnt = img_w * img_h;

  QImage img3(img_w, img_h, QImage::Format_ARGB32);

  const uint8_t * __restrict__ p_img1 = (uint8_t *) img1.bits();
  const uint8_t * __restrict__ p_img2 = (uint8_t *) img2.bits();
  uint8_t * __restrict__ p_img3 = (uint8_t *) img3.bits();
  float w1 = t;
  float w2 = (1.0f - t);

  for (int i = 0; i < cnt; ++i)
  {
    p_img3[0] = p_img1[0] * w1 + p_img2[0] * w2;
    p_img3[1] = p_img1[1] * w1 + p_img2[1] * w2;
    p_img3[2] = p_img1[2] * w1 + p_img2[2] * w2;
    p_img3[3] = 255;
    p_img1 += 4;
    p_img2 += 4;
    p_img3 += 4;
  }
  return img3;
}


void Morpher::morph(const QImage & src_img, const Mesh & src_mesh,
                    const QImage & dst_img, const Mesh & dst_mesh,
                    int nframes)
{
  if (!src_mesh.hasSameDimensions(dst_mesh))
  {
    std::cerr << "WARNING: source mesh has different dimensions than destination mesh" << std::endl;
    return;
  }

  Mesh tmp_mesh(src_mesh.sizeX(), src_mesh.sizeY(), src_mesh.width(), src_mesh.heigth());

  for (int i = 0; i < nframes; ++i)
  {
    float t = float(i) / (float(nframes - 1));

    QImage img(morphFrame(src_img, src_mesh, dst_img, dst_mesh, tmp_mesh, t));
    if (img.isNull())
    {
      std::cerr << "WARNING: Failed to morph frame n. " << i << std::endl;
    }

    if (!img.save(QString("frame_%1.jpg").arg(i)))
    {
      std::cerr << "WARNING: Failed to save frame n. " << i << std::endl;
    }
  }
}
