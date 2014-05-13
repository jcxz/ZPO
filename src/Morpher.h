#ifndef MORPHER_H
#define MORPHER_H


class QImage;
class Mesh;


class Morpher
{
  public:

    QImage warp(const QImage & src_img, const Mesh & src_mesh, const Mesh & dst_mesh);

    //void morphFrame(const QImage & src_img, const Mesh & src_mesh,
    //                const QImage & dst_img, const Mesh & dst_mesh,
    //                Mesh & tmp_mesh, float t, QImage & result);

    QImage morphFrame(const QImage & src_img, const Mesh & src_mesh,
                      const QImage & dst_img, const Mesh & dst_mesh,
                      Mesh & tmp_mesh, float t);

    void morph(const QImage & src_img, const Mesh & src_mesh,
               const QImage & dst_img, const Mesh & dst_mesh,
               int nframes);

  private:

};

#endif
