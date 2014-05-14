#include "MeshWarpWidget.h"
#include "morph.h"
#include "debug.h"

#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>


static constexpr int CONTROL_POINT_SIZE = 8;//6;



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


void MeshWarpWidget::handleCPChange(int x, int y)
{
  m_active_cp_x = x;
  m_active_cp_y = y;
  m_has_active_cp = true;
  update();
}


void MeshWarpWidget::handleCPDeactivation(void)
{
  m_has_active_cp = false;
  update();
}


#if 0
void MeshWarpWidget::drawActiveCP(QPainter & painter)
{
  //QPen old_pen = painter.pen();
  //painter.setPen(old_pen);

  const Point p = m_mesh(m_active_cp_x, m_active_cp_y);
  painter.drawEllipse(p.x - (CONTROL_POINT_SIZE / 2), p.y - (CONTROL_POINT_SIZE / 2),
                      CONTROL_POINT_SIZE, CONTROL_POINT_SIZE);
}


void MeshWarpWidget::drawMesh(QPainter & painter)
{
  //QPen pen(QColor(Qt::red));
  //QPen pen(QColor(0xFF, 0xFF, 0xFF));
  //QPen pen(QColor(Qt::black));
  QPen pen(QColor(255, 153, 0));
  painter.setPen(pen);

  //painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
  //painter.setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
  //painter.setCompositionMode(QPainter::CompositionMode_Xor);
  //painter.setCompositionMode(QPainter::CompositionMode_Lighten);
  //painter.setCompositionMode(QPainter::CompositionMode_ColorDodge);
  //painter.setCompositionMode(QPainter::CompositionMode_ColorBurn);
  //painter.setCompositionMode(QPainter::CompositionMode_Difference);
  //painter.setPen(QColor(0xFF,0xFF,0xFF));

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

  //QPen pen(painter.pen());
  pen = painter.pen();
  pen.setWidth(CONTROL_POINT_SIZE);
  painter.setPen(pen);

  for (int i = 0; i < n; ++i)
  {
    painter.drawPoint(p_points->x, p_points->y);
    ++p_points;
  }

  if (m_has_active_cp)
  {
    drawActiveCP(painter);
  }

  // TODO: mozno nastavit pero spat
}
#else
QImage MeshWarpWidget::drawMesh(void)
{
  // create image and painter
  QImage mesh_img(m_img.width() + CONTROL_POINT_SIZE,
                  m_img.height() + CONTROL_POINT_SIZE,
                  QImage::Format_ARGB32);

  QPainter painter(&mesh_img);

  QTransform transform;
  transform.translate(CONTROL_POINT_SIZE / 2, CONTROL_POINT_SIZE / 2);
  painter.setTransform(transform);

  // create pen
  //QPen pen(QColor(255, 153, 0, 128));
  //QPen pen(QColor(255, 153, 0, 255));
  //QPen pen(QColor(255, 153, 0));

  QPen pen(QColor(255, 255, 255, 255));
  painter.setPen(pen);
  //painter.setCompositionMode(QPainter::CompositionMode_Source);

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

  //pen.setColor(QColor(205, 153, 0));
  //pen.setColor(QColor(205, 153, 0, 255));

  //pen.setColor(QColor(210, 210, 210, 255));
  pen.setColor(QColor(255, 255, 255, 255));
  pen.setWidth(CONTROL_POINT_SIZE);
  painter.setPen(pen);

  for (int i = 0; i < n; ++i)
  {
    painter.drawPoint(p_points->x, p_points->y);
    ++p_points;
  }

#if 0
  if (m_has_active_cp)
  {
    //pen.setColor(QColor(185, 103, 0, 255));

    pen.setColor(QColor(255, 153, 0, 255));
    painter.setPen(pen);
    const Point p = m_mesh(m_active_cp_x, m_active_cp_y);
    painter.drawEllipse(p.x - (CONTROL_POINT_SIZE / 2), p.y - (CONTROL_POINT_SIZE / 2),
                        CONTROL_POINT_SIZE, CONTROL_POINT_SIZE);
  }
#endif

  return mesh_img;
}
#endif


void MeshWarpWidget::paintEvent(QPaintEvent *event)
{
  /*
  m_transform = QTransform();
  float x = (width() - m_img.width()) * 0.5f + m_offset_x;
  float y = (height() - m_img.height()) * 0.5f + m_offset_y;
  m_transform.translate(x, y);
  */

  //m_transform = QTransform(1.0f, 0.0f, 0.0f, 1.0f,
  m_transform = QTransform(m_scale, 0.0f, 0.0f, m_scale,
                           (width() - m_img.width()) * 0.5f + m_offset_x,
                           (height() - m_img.height()) * 0.5f + m_offset_y);

  QPainter painter(this);

  /*
   * The following 3 lines of code should enable Qt stylesheets for this widget
   * this code was inspired by qt's documentation.
   * see: http://developer.qt.nokia.com/doc/qt-4.8/stylesheet-reference.html
   */
  //QStyleOption opt;
  //opt.init(this);
  //style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

  painter.fillRect(0, 0, width(), height(), QColor(0x20, 0x1F, 0x1F));

  painter.setTransform(m_transform);
  painter.drawImage(0, 0, m_img);

  /*
  painter.drawImage(QRect(0, 0,
                          std::min(width(), m_img.width()),
                          std::min(height(), m_img.height())),
                    m_img);
                    */

  //drawMesh(painter);

  painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
  //painter.setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
  //painter.setCompositionMode(QPainter::CompositionMode_Xor);
  //painter.setCompositionMode(QPainter::CompositionMode_Lighten);
  //painter.setCompositionMode(QPainter::CompositionMode_Darken);
  //painter.setCompositionMode(QPainter::CompositionMode_ColorDodge);
  //painter.setCompositionMode(QPainter::CompositionMode_ColorBurn);
  //painter.setCompositionMode(QPainter::CompositionMode_Difference);
  //painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
  //painter.setCompositionMode(QPainter::CompositionMode_Screen);
  //painter.setCompositionMode(QPainter::CompositionMode_Overlay);
  //painter.setCompositionMode(QPainter::CompositionMode_Multiply);

  painter.drawImage(-(CONTROL_POINT_SIZE / 2), -(CONTROL_POINT_SIZE / 2), drawMesh());

  //painter.drawImage(QRect(-(CONTROL_POINT_SIZE / 2),
  //                        -(CONTROL_POINT_SIZE / 2),
  //                        std::min(width(), m_img.width()),
  //                        std::min(height(), m_img.height())),
  //                  m_img);

  if (m_has_active_cp)
  {
    painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);

    //pen.setColor(QColor(185, 103, 0, 255));

    QPen pen;
    pen.setWidth(CONTROL_POINT_SIZE);
    pen.setColor(QColor(255, 153, 0, 255));
    painter.setPen(pen);
    const Point p = m_mesh(m_active_cp_x, m_active_cp_y);
    painter.drawEllipse(p.x - (CONTROL_POINT_SIZE / 2), p.y - (CONTROL_POINT_SIZE / 2),
                        CONTROL_POINT_SIZE, CONTROL_POINT_SIZE);
  }

  return QWidget::paintEvent(event);
}


void MeshWarpWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (m_has_active_cp)
  {
    int x = 0, y = 0;
    m_transform.inverted().map(event->x(), event->y(), &x, &y);
    m_mesh.setPoint(m_active_cp_x, m_active_cp_y, x, y);

    //m_mesh.setPoint(m_active_cp_x, m_active_cp_y,
    //                event->x() + m_transform.inverted().dx(),
    //                event->y() + m_transform.inverted().dy());
    update();
  }
  else if (m_panning)
  {
    //std::cerr << "Panning: event_x=" << event->x() << ", event_y=" << event->y() << ", offset_x=" << m_offset_x << ", m_offset_y=" << m_offset_y << std::endl;
    m_offset_x = (event->x() - m_origin_x) + m_vect_x;
    m_offset_y = (event->y() - m_origin_y) + m_vect_y;
    update();
  }

  return QWidget::mouseMoveEvent(event);
}


void MeshWarpWidget::mousePressEvent(QMouseEvent *event)
{
  std::cerr << "Searching control point at: ["
            << (event->x() + m_transform.inverted().dx())
            << ", "
            << (event->y() + m_transform.inverted().dy())
            << "]" << std::endl;

  std::cerr << "Mesh at [1, 1]" << m_mesh.at(1, 1) << std::endl;

  int x = 0, y = 0;
  m_transform.inverted().map(event->x(), event->y(), &x, &y);
  std::cerr << "mapped point: x=" << x << ",y=" << y << std::endl;
  m_has_active_cp = m_mesh.findPoint(x, y, m_active_cp_x, m_active_cp_y,
                                     CONTROL_POINT_SIZE / 2);

  //m_has_active_cp = m_mesh.findPoint(event->x() + m_transform.inverted().dx(),
  //                                   event->y() + m_transform.inverted().dy(),
  //                                   m_active_cp_x, m_active_cp_y,
  //                                   CONTROL_POINT_SIZE / 2);

  if (m_has_active_cp)
  {
    std::cerr << "Found: [" << m_active_cp_x << "," << m_active_cp_y << "]" << std::endl;
    m_panning = false;
    emit activeCPChanged(m_active_cp_x, m_active_cp_y);
  }
  else
  {
    std::cerr << "Panning enabled" << std::endl;
    m_origin_x = event->x();
    m_origin_y = event->y();
    m_panning = true;
    emit activeCPDeactivated();
  }

  update();

  return QWidget::mousePressEvent(event);
}


void MeshWarpWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (m_has_active_cp)
  {
    if (m_show_warped)
    {
      m_img = warp(m_orig_img, m_orig_mesh, m_mesh);
    }

    update();
  }
  else if (m_panning)
  {
    m_vect_x += (event->x() - m_origin_x);
    m_vect_y += (event->y() - m_origin_y);
    m_panning = false;
    std::cerr << "Panning DISabled" << std::endl;
  }

  return QWidget::mouseReleaseEvent(event);
}


void MeshWarpWidget::wheelEvent(QWheelEvent *event)
{
  if (event->delta() > 0)
  {
    m_scale += 0.1;
  }
  else
  {
    m_scale -= 0.1;
  }

  if (m_scale > 5.0f) m_scale = 5.0f;
  if (m_scale <= 0.0f) m_scale = 0.01f;

  std::cerr << "m_scale: " << m_scale << std::endl;
  update();
  return QWidget::wheelEvent(event);
}


void MeshWarpWidget::resetTransformations(void)
{
  m_scale = 1.0f;
  m_origin_x = 0;
  m_origin_y = 0;
  m_vect_x = 0;
  m_vect_y = 0;
  m_offset_x = 0;
  m_offset_y = 0;
  m_transform = QTransform();
}


void MeshWarpWidget::keyPressEvent(QKeyEvent *event)
{
  std::cerr << __FUNCTION__ << std::endl;
  switch (event->key())
  {
    case Qt::Key_R: resetTransformations(); update(); break;
  }

  return QWidget::keyPressEvent(event);
}
