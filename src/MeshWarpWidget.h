#ifndef MESHWARPWIDGET_H
#define MESHWARPWIDGET_H

#include "Mesh.h"
#include "morph.h"
#include "debug.h"

#include <QWidget>


class MeshWarpWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit MeshWarpWidget(QWidget *parent = 0)
      : QWidget(parent)
      , m_scale(1.0f)
      , m_origin_x(0)
      , m_origin_y(0)
      , m_vect_x(0)
      , m_vect_y(0)
      , m_offset_x(0)
      , m_offset_y(0)
      , m_panning(false)
      , m_active_cp_x(0)
      , m_active_cp_y(0)
      , m_has_active_cp(false)
      , m_transform()
      , m_orig_img()
      , m_img()
      , m_orig_mesh()
      , m_mesh()
      , m_show_warped(false)
    {
      DBGM("Original mesh : " << m_orig_mesh);
      DBGM("New mesh      : " << m_mesh);
      setFocusPolicy(Qt::StrongFocus);  // this is to make key's work
    }

    explicit MeshWarpWidget(const QImage & img, QWidget *parent = 0)
      : QWidget(parent)
      , m_scale(1.0f)
      , m_origin_x(0)
      , m_origin_y(0)
      , m_vect_x(0)
      , m_vect_y(0)
      , m_offset_x(0)
      , m_offset_y(0)
      , m_panning(false)
      , m_active_cp_x(0)
      , m_active_cp_y(0)
      , m_has_active_cp(false)
      , m_transform()
      , m_orig_img(img)
      , m_img(img)
      , m_orig_mesh(0.01f, 0.01f, img.width(), img.height())
      , m_mesh(0.01f, 0.01f, img.width(), img.height())
      , m_show_warped(false)
    {
      DBGM("Original mesh : " << m_orig_mesh);
      DBGM("New mesh      : " << m_mesh);
      setFocusPolicy(Qt::StrongFocus);  // this is to make key's work
    }

    const Mesh & mesh(void) const { return m_mesh; }
    const QImage & image(void) const { return m_img; }

    bool setImage(const QImage & img);
    bool setImage(const QString & filename);

    void set(QImage & img, Mesh & mesh);

    void toggleShowWarped(void)
    {
      m_show_warped = !m_show_warped;
      if (m_show_warped) m_img = m_orig_img;
    }

  private slots:
    void handleCPChange(int x, int y);
    void handleCPDeactivation(void);

  signals:
    void activeCPChanged(int x, int y);
    void activeCPDeactivated(void);

  protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

  private:
    //void drawMesh(QPainter & painter);
    //void drawActiveCP(QPainter & painter);

    QImage drawMesh(void);
    void resetTransformations(void);

  private:
    float m_scale;

    int m_origin_x;
    int m_origin_y;
    int m_vect_x;
    int m_vect_y;
    int m_offset_x;
    int m_offset_y;
    bool m_panning;

    int m_active_cp_x;
    int m_active_cp_y;
    bool m_has_active_cp;

    QTransform m_transform;

    QImage m_orig_img;
    QImage m_img;

    Mesh m_orig_mesh;
    Mesh m_mesh;

    bool m_show_warped;
};

#endif // MESHWARPWIDGET_H
