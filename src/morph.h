#ifndef WARP_H
#define WARP_H

class Mesh;
class QImage;
class Movie;

QImage warp(const QImage & img, const Mesh & src_mesh, const Mesh & dst_mesh);


QImage morphFrame(const QImage & src_img, const Mesh & src_mesh,
                  const QImage & dst_img, const Mesh & dst_mesh,
                  Mesh & tmp_mesh, float t);

void morph(const QImage & src_img, const Mesh & src_mesh,
           const QImage & dst_img, const Mesh & dst_mesh,
           int nframes);

Movie *morphMovie(const QImage & src_img, const Mesh & src_mesh,
                  const QImage & dst_img, const Mesh & dst_mesh,
                  int nframes);

#endif // WARP_H
