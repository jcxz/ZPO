#include "MeshWarpWidget.h"
#include "morph.h"

#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>


static constexpr int CONTROL_POINT_SIZE = 6;



bool MeshWarpWidget::setImage(const QImage & img)
{
  m_active_cp_x = 0;
  m_active_cp_y = 0;
  m_has_active_cp = false;
  m_orig_img = img;
  m_img = img;
  m_transform = QTransform();
  m_orig_mesh.resize(0.01f, 0.01f, img.width(), img.height());
  m_mesh.resize(0.01f, 0.01f, img.width(), img.height());
  return true;
}


bool MeshWarpWidget::setImage(const QString & filename)
{
  QImage img(filename);
  if (img.isNull())
  {
    std::cerr << "Failed to open image: " << filename.toStdString() << std::endl;
    return false;
  }
  return setImage(img);
}


void MeshWarpWidget::drawMesh(QPainter & painter)
{
  int mesh_w = m_mesh.sizeX();
  int mesh_h = m_mesh.sizeY();

  // draw horizontal mesh lines
  for (int i = 0; i < mesh_h; ++i)
  {
    Point start = m_mesh(0, i);
    for (int j = 1; j < mesh_w; ++j)
    {
      Point end = m_mesh(j, i);
      painter.drawLine(start.x, start.y, end.x, end.y);
      start = end;
    }
  }

  // draw horizontal mesh lines
  for (int j = 0; j < mesh_w; ++j)
  {
    Point start = m_mesh(j, 0);
    for (int i = 1; i < mesh_h; ++i)
    {
      Point end = m_mesh(j, i);
      painter.drawLine(start.x, start.y, end.x, end.y);
      start = end;
    }
  }

  // draw control points them selves
  Point * __restrict__ p_points = m_mesh.data();
  int n = m_mesh.pointCount();

  QPen pen(painter.pen());
  pen.setWidth(CONTROL_POINT_SIZE);
  painter.setPen(pen);

  for (int i = 0; i < n; ++i)
  {
    painter.drawPoint(p_points->x, p_points->y);
    ++p_points;
  }

  // TODO: mozno nastavit pero spat
}


void MeshWarpWidget::paintEvent(QPaintEvent *event)
{
  m_transform = QTransform();
  float x = (width() - m_img.width()) * 0.5f;
  float y = (height() - m_img.height()) * 0.5f;
  m_transform.translate(x, y);

  QPainter painter(this);

  /*
   * The following 3 lines of code should enable Qt stylesheets for this widget
   * this code was inspired by qt's documentation.
   * see: http://developer.qt.nokia.com/doc/qt-4.8/stylesheet-reference.html
   */
  QStyleOption opt;
  opt.init(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

  painter.setTransform(m_transform);
  painter.drawImage(0, 0, m_img);

  QPen pen(QColor(Qt::red));
  painter.setPen(pen);
  drawMesh(painter);

  return QWidget::paintEvent(event);
}


void MeshWarpWidget::mouseMoveEvent(QMouseEvent *event)
{
#if 0
  if (m_active_cp != nullptr)
  {
    m_active_cp->x = event->x() - m_transform.dx();
    m_active_cp->y = event->y() - m_transform.dy();
    update();
  }
#else
  if (m_has_active_cp)
  {
    m_mesh.setPoint(m_active_cp_x, m_active_cp_y,
                    event->x() - m_transform.dx(),
                    event->y() - m_transform.dy());
    update();
  }
#endif

  return QWidget::mouseMoveEvent(event);
}


void MeshWarpWidget::mousePressEvent(QMouseEvent *event)
{
#if 0
  m_active_cp = m_mesh.findPoint(event->x() - m_transform.dx(),
                                 event->y() - m_transform.dy(),
                                 CONTROL_POINT_SIZE);
  if (m_active_cp != nullptr)
  {
    std::cerr << "Found: " << *m_active_cp << std::endl;
  }
#else
  m_has_active_cp = m_mesh.findPoint(event->x() - m_transform.dx(),
                                     event->y() - m_transform.dy(),
                                     m_active_cp_x, m_active_cp_y,
                                     CONTROL_POINT_SIZE / 2);
  if (m_has_active_cp)
  {
    std::cerr << "Found: [" << m_active_cp_x << "," << m_active_cp_y << "]" << std::endl;
  }
#endif

  return QWidget::mousePressEvent(event);
}


void MeshWarpWidget::mouseReleaseEvent(QMouseEvent *event)
{
#if 0
  if (m_active_cp != nullptr)
  {
    m_img = warp(m_orig_img, m_orig_mesh, m_mesh);
    update();
  }
#else
  if (m_has_active_cp)
  {
    m_img = warp(m_orig_img, m_orig_mesh, m_mesh);
    update();
  }
#endif

  return QWidget::mouseReleaseEvent(event);
}
