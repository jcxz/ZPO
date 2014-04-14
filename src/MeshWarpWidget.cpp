#include "MeshWarpWidget.h"
#include "warp.h"

#include <QPainter>
#include <QMouseEvent>


static constexpr int CONTROL_POINT_SIZE = 6;


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
