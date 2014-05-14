#ifndef MESH_H
#define MESH_H

#include "Point.h"

#include <memory>
#include <ostream>


class QDataStream;


class Mesh
{
  public:
    Mesh(void)
      : m_width(0)
      , m_heigth(0)
      , m_size_x(0)
      , m_size_y(0)
      , m_size(0)
      , m_points(nullptr)
    { }

    Mesh(float density, int width, int heigth)
      : m_width(0)
      , m_heigth(0)
      , m_size_x(0)
      , m_size_y(0)
      , m_size(0)
      , m_points(nullptr)
    { resize(density, width, heigth); }

    Mesh(float density_w, float density_h, int width, int heigth)
      : m_width(0)
      , m_heigth(0)
      , m_size_x(0)
      , m_size_y(0)
      , m_size(0)
      , m_points(nullptr)
    { resize(density_w, density_h, width, heigth); }

    Mesh(int mesh_w, int mesh_h, int width, int heigth, bool recalc = true)
      : m_width(width)
      , m_heigth(heigth)
      , m_size_x(mesh_w)
      , m_size_y(mesh_h)
      , m_size(mesh_w * mesh_h)
      , m_points(new Point[m_size])
    { if (recalc) resample(); }

    Mesh(Mesh && other) noexcept
      : m_width(other.m_width)
      , m_heigth(other.m_heigth)
      , m_size_x(other.m_size_x)
      , m_size_y(other.m_size_y)
      , m_size(other.m_size)
      , m_points(nullptr)
    {
      m_points.swap(other.m_points);
    }

    Mesh & operator=(Mesh && other) noexcept
    {
      m_width = other.m_width;
      m_heigth = other.m_heigth;
      m_size_x = other.m_size_x;
      m_size_y = other.m_size_y;
      m_size = other.m_size;
      m_points.swap(other.m_points);
      return *this;
    }

    // getter functions
    int width(void) const { return m_width; }
    int heigth(void) const { return m_heigth; }
    int pointCount(void) const { return m_size; }
    int sizeX(void) const { return m_size_x; }
    int sizeY(void) const { return m_size_y; }

    // indexing and data retrieval
    const Point *data(void) const { return m_points.get(); }
    Point *data(void) { return m_points.get(); }
    Point operator()(int x, int y) const { return m_points[y * m_size_x + x]; }
    Point & operator()(int x, int y) { return m_points[y * m_size_x + x]; }
    Point at(int x, int y) const { return m_points[y * m_size_x + x]; }
    Point & at(int x, int y) { return m_points[y * m_size_x + x]; }

    // sets a point on given coordinates to a new value,
    // while taking care of not damaging the edge of the mesh
    void setPoint(int col, int row, int x, int y);

    // checking functions
    bool isResampledTo(int width, int height) const;
    bool hasValidDimensions(void) const { return (m_size_x >= 2) && (m_size_y >= 2); }
    bool hasSameDimensions(const Mesh & other) const
    { return (m_size_x == other.m_size_x) && (m_size_y == other.m_size_y); }

    // resizing and resampling functions

    // resamples the spacing between mesh points, so that it
    // fits the given width and height (the number of mesh points remains unchanged)
    void resampleTo(int width, int height)
    {
      m_width = width;
      m_heigth = height;
      return resample();
    }

    // resizes the mesh (i.e. it changes the number of control points in the mesh while
    // also resampling it to given width and height)
    void resize(int mesh_w, int mesh_h, int width, int height);
    void resize(float density_w, float density_h, int width, int height);

    // resizes the mesh according to density, which is a number in range [0.0 - 1.0]
    // and determines how smaller is the size of mesh compared to a given width and height
    void resize(float density, int width, int height)
    { return resize(density, density, width, height); }

    // manipulation functions

    // searches for a point with given coordinates, returns nullptr if no such point is found
    // the tolerance parameter defines a radius around the point for which the point is still
    // considered to be found
    Point *findPoint(int x, int y, int tolerance = 0) const;

    // searches for a point with given coordinates and returns its coordinates in the mesh
    bool findPoint(int x, int y, int & col, int & row, int tolerance = 0) const
    {
      Point *pt = findPoint(x, y, tolerance);
      if (pt == nullptr) return false;
      int idx = pt - m_points.get();
      row = idx / m_size_x;
      col = idx % m_size_x;
      return true;
    }

    // this functions simply copies point data from the other mesh
    // the dimensions of bot meshes must agree
    // returns false if dimensions do not agree
    bool copyPoints(const Mesh & other);

    // This method updates the points in the mesh by interpolating
    // between a given source and destination mesh
    // The sizes of all three meshes have to match
    void interpolate(const Mesh & src_mesh, const Mesh & dst_mesh, float t);

    // debugging functions
    friend std::ostream & operator<<(std::ostream & os, const Mesh & mesh);

    // serialization functions
    friend QDataStream & operator<<(QDataStream & stream, const Mesh & mesh);  // output
    friend QDataStream & operator>>(QDataStream & stream, Mesh & mesh);        // input


  private:
    void resample(void);

  private:
    int m_width;                        /// the width that is represented by mesh
    int m_heigth;                       /// the heigth that is represented by mesh
    int m_size_x;                       /// number of control points in horizontal direction
    int m_size_y;                       /// number of control points in vertical direction
    int m_size;                         /// the count of control points
    std::unique_ptr<Point[]> m_points;  /// and array of control points
};

#endif // MESH_H
