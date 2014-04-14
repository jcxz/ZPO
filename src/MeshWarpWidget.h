#ifndef MESHWARPWIDGET_H
#define MESHWARPWIDGET_H

#include "Mesh.h"

#include <QWidget>
#include <iostream>



class MeshWarpWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit MeshWarpWidget(QWidget *parent = 0)
      : QWidget(parent)
      //, m_active_cp(nullptr)
      , m_active_cp_x(0)
      , m_active_cp_y(0)
      , m_has_active_cp(false)
      , m_orig_img()
      , m_img()
      , m_transform()
      , m_orig_mesh()
      , m_mesh()
    {
      std::cout << "Original mesh : " << m_orig_mesh << std::endl;
      std::cout << "New mesh      : " << m_mesh << std::endl;
    }

    explicit MeshWarpWidget(const QImage & img, QWidget *parent = 0)
      : QWidget(parent)
      //, m_active_cp(nullptr)
      , m_active_cp_x(0)
      , m_active_cp_y(0)
      , m_has_active_cp(false)
      , m_orig_img(img)
      , m_img(img)
      , m_transform()
      , m_orig_mesh(0.01f, 0.01f, img.width(), img.height())
      , m_mesh(0.01f, 0.01f, img.width(), img.height())
    {
      std::cout << "Original mesh : " << m_orig_mesh << std::endl;
      std::cout << "New mesh      : " << m_mesh << std::endl;
    }

    const Mesh & mesh(void) const { return m_mesh; }
    const QImage & image(void) const { return m_img; }

    bool setImage(const QImage & img);
    bool setImage(const QString & filename);

  protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

  private:
    void drawMesh(QPainter & painter);

  private:
    //Point *m_active_cp;   // the currently active control point
    int m_active_cp_x;
    int m_active_cp_y;
    bool m_has_active_cp;
    QImage m_orig_img;
    QImage m_img;
    QTransform m_transform;
    Mesh m_orig_mesh;
    Mesh m_mesh;
};

#endif // MESHWARPWIDGET_H
