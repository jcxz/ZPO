#include "Mesh.h"
#include "debug.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cassert>
#include <QDataStream>



void Mesh::setPoint(int col, int row, int x, int y)
{
  Point & pt = at(col, row);
  if ((col > 0) && (col < (m_size_x - 1)))
  {
    pt.x = x;
  }

  if ((row > 0) && (row < (m_size_y - 1)))
  {
    pt.y = y;
  }
}


void Mesh::resample(void)
{
  float factor_w = float(m_width) / float(m_size_x - 1);
  float factor_h = float(m_heigth) / float(m_size_y - 1);

  for (int y = 0; y < m_size_y; ++y)
  {
    for (int x = 0; x < m_size_x; ++x)
    {
      Point & pt = at(x, y);
      pt.x = x * factor_w;
      pt.y = y * factor_h;
    }
  }
}


#if 0
void Mesh::resampleTo(int width, int height)
{
  m_width = width;
  m_heigth = height;
  float factor_w = float(width) / float(m_size_x - 1);
  float factor_h = float(height) / float(m_size_y - 1);
  Point * __restrict__ points = m_points.get();

  for (int i = 0; i < m_size; ++i)
  {
    points->x = float(points->x) * factor_w;
    points->y = float(points->y) * factor_h;
    ++points;
  }
}
#endif


#if 0
void Mesh::resize(float density_w, float density_h, int width, int heigth)
{
  std::cout << "density_w: " << density_w << std::endl;
  std::cout << "density_h: " << density_h << std::endl;

  if ((density_w <= 0.0f) || (density_h <= 0.0f))
  {
    std::cerr << "ERROR: Density cannot be negative or equal to zero" << std::endl;
    return;
  }

  if (density_w > 1.0f) density_w = 1.0f;
  if (density_h > 1.0f) density_h = 1.0f;

  std::cout << "density_w: " << density_w << std::endl;
  std::cout << "density_h: " << density_h << std::endl;

  //int mesh_w = density_w * width + 1;
  //int mesh_h = density_h * height + 1;
  //int size = mesh_w * mesh_h;

  // toto ale mozno nie je uplne exception safe,
  // najprv by sa asi mala alokovat pamat, aby m_width a m_height a m_size
  // zostali platne aj ked vyleti vynimka
  m_width = density_w * width;    // +1 aby mesh obsahovala aj krajne body
  m_heigth = density_h * heigth;  // +1 aby mesh obsahovala aj krajne body
  m_size = m_width * m_heigth;
  m_points.reset(new Point[m_size]);

  std::cout << "m_heigth : " << m_heigth << std::endl;
  std::cout << "m_width  : " << m_width << std::endl;

  for (int y = 0; y < m_heigth; ++y)
  {
    for (int x = 0; x < m_width; ++x)
    {
      Point & pt = (*this)(x, y);
      pt.x = x * density_w;
      pt.y = y * density_h;
    }
  }
}
#endif


void Mesh::resize(int mesh_w, int mesh_h, int width, int heigth)
{
  int size = mesh_w * mesh_h;
  m_points.reset(new Point[size]);
  m_size = size;
  m_size_x = mesh_w;
  m_size_y = mesh_h;
  m_width = width;
  m_heigth = heigth;

  // -1 kvoli tomu aby sa siet roztiahla tak, ze bude obsahovat aj krajne body z width a height
  float factor_w = float(width) / float(mesh_w - 1);
  float factor_h = float(heigth) / float(mesh_h - 1);

  //std::cout << "mesh width    : " << mesh_w << std::endl;
  //std::cout << "mesh height   : " << mesh_h << std::endl;
  //std::cout << "image width   : " << width << std::endl;
  //std::cout << "image height  : " << height << std::endl;
  //std::cout << "width factor  : " << factor_w << std::endl;
  //std::cout << "height factor : " << factor_h << std::endl;

  for (int y = 0; y < mesh_h; ++y)
  {
    for (int x = 0; x < mesh_w; ++x)
    {
      Point & pt = (*this)(x, y);
      pt.x = x * factor_w;
      pt.y = y * factor_h;
    }
  }
}


void Mesh::resize(float density_w, float density_h, int width, int heigth)
{
  if ((density_w <= 0.0f) || (density_h <= 0.0f))
  {
    std::cerr << "ERROR: Density cannot be negative or equal to zero" << std::endl;
    return;
  }

  if (density_w > 1.0f) density_w = 1.0f;
  if (density_h > 1.0f) density_h = 1.0f;

  // the int cast is very important here, else this will run into recursion
  return resize(int(width * density_w), int(heigth * density_h), width, heigth);
}


bool Mesh::isResampledTo(int width, int heigth) const
{
  for (int x = 0; x < m_size_x; ++x)
  {
    if (at(x, 0).y != 0) return false;
  }

  for (int x = 0; x < m_size_x; ++x)
  {
    if (at(x, m_size_y - 1).y != heigth) return false;
  }

  for (int y = 0; y < m_size_y; ++y)
  {
    if (at(0, y).x != 0) return false;
  }

  for (int y = 0; y < m_size_y; ++y)
  {
    if (at(m_size_x - 1, y).x != width) return false;
  }

  return true;
}


Point *Mesh::findPoint(int x, int y, int tolerance) const
{
  //std::cerr << "Searching for: [" << x << "," << y << "] with tolerance " << tolerance << std::endl;

  // first check that the point is inside the mesh
  if ((x < -tolerance) || (x > (m_width + tolerance)) || (y < -tolerance) || (y > (m_heigth + tolerance)))
  {
    std::cerr << "WARNING: the searched for point lies outside of the mesh boundaries" << std::endl;
    return nullptr;
  }

  int max_x = x + tolerance;
  int min_x = x - tolerance;
  int max_y = y + tolerance;
  int min_y = y - tolerance;
  Point * __restrict__ points = m_points.get();

  //std::cerr << "min_x: " << min_x << ", max_x: " << max_x << ", min_y: " << min_y << "max_y: " << max_y << std::endl;

  // zatial je pouzita ta najprimitivnejsia metoda lineraneho vyhladania,
  // mozno to bude este neskor treba nejako zoptimalizovat
  for (int i = 0; i < m_size; ++i)
  {
    if ((points->x >= min_x) && (points->x <= max_x) &&
        (points->y >= min_y) && (points->y <= max_y))
    {
      return points;
    }
    ++points;
  }

  return nullptr;
}


bool Mesh::copyPoints(const Mesh & other)
{
  if ((other.m_size_x != m_size_x) || (other.m_size_y != m_size_y))
  {
    std::cerr << "WARNING: mesh dimensions do not agree" << std::endl;
    return false;
  }

  std::memcpy(m_points.get(), other.m_points.get(), sizeof(Point) * m_size);

  return true;
}


void Mesh::interpolate(const Mesh & src_mesh, const Mesh & dst_mesh, float t)
{
  assert(hasSameDimensions(src_mesh) && hasSameDimensions(dst_mesh));

  float w1 = t;
  float w2 = (1.0f - t);

  const Point * __restrict__ p_src_mesh = src_mesh.data();
  const Point * __restrict__ p_dst_mesh = dst_mesh.data();
  Point * __restrict__ p_this_mesh = data();

  for (int i = 0; i < m_size; ++i)
  {
    p_this_mesh->x = p_src_mesh->x * w1 + p_dst_mesh->x * w2;
    p_this_mesh->y = p_src_mesh->y * w1 + p_dst_mesh->y * w2;

    ++p_src_mesh;
    ++p_dst_mesh;
    ++p_this_mesh;
  }
}


static int digitCount(int num)
{
  int cnt = 1;

  if (num < 0)
  {
    ++cnt;
    num = -num;
  }

  int i = 10;
  while (num >= i)
  {
    ++cnt;
    i *= 10;
    if (i > 999999999) return cnt + 1;
  }

  return cnt;
}


static void maxDigitCount(Point * __restrict__ points, int n, int & max_x_cnt, int & max_y_cnt)
{
  max_x_cnt = 0;
  max_y_cnt = 0;

  for (int i = 0; i < n; ++i)
  {
    int x_cnt = digitCount(points->x);
    if (x_cnt > max_x_cnt) max_x_cnt = x_cnt;
    int y_cnt = digitCount(points->y);
    if (y_cnt > max_y_cnt) max_y_cnt = y_cnt;
    ++points;
  }
}


std::ostream & operator<<(std::ostream & os, const Mesh & mesh)
{
  os << "width x height: [" << mesh.m_size_x << "," << mesh.m_size_y << "]\n";

  int x_width, y_width;
  maxDigitCount(mesh.m_points.get(), mesh.m_size, x_width, y_width);

  for (int y = 0; y < mesh.m_size_y; ++y)
  {
    for (int x = 0; x < mesh.m_size_x; ++x)
    {
      //os << std::setw(4) << std::right << mesh(x, y) << ' ';
      //os << mesh(x, y) << ' ';
      const Point & pt = mesh(x, y);
      os << "[" << std::setw(x_width) << pt.x << "," << std::setw(y_width) << pt.y << "] ";
    }
    os << '\n';
  }

  return os;
}


QDataStream & operator<<(QDataStream & stream, const Mesh & mesh)
{
  stream << mesh.m_width;
  stream << mesh.m_heigth;
  stream << mesh.m_size_x;
  stream << mesh.m_size_y;
  stream << mesh.m_size;

  const Point * __restrict__ p_data = mesh.data();
  for (int i = 0; i < mesh.m_size; ++i)
  {
    stream << p_data->x;
    stream << p_data->y;
    ++p_data;
  }

  return stream;
}


QDataStream & operator>>(QDataStream & stream, Mesh & mesh)
{
  stream >> mesh.m_width;
  stream >> mesh.m_heigth;
  stream >> mesh.m_size_x;
  stream >> mesh.m_size_y;
  stream >> mesh.m_size;

  if (stream.status() != QDataStream::Ok)
  {
    ERRORM("Error while reading mesh header from the stream");
    return stream;
  }

  if ((mesh.m_width < 0) || (mesh.m_heigth < 0) ||
      (mesh.m_size_x < 0) || (mesh.m_size_y < 0) ||
      (mesh.m_size < 0))
  {
    ERRORM("The size of the mesh or its width or heigth is negative");
    stream.setStatus(QDataStream::ReadCorruptData);
    return stream;
  }

  mesh.m_points.reset(new Point[mesh.m_size]);

  Point * __restrict__ p_data = mesh.data();
  for (int i = 0; i < mesh.m_size; ++i)
  {
    stream >> p_data->x;
    stream >> p_data->y;
    ++p_data;
  }

  return stream;
}
