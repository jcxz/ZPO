#ifndef MORPH_H
#define MORPH_H

class Mesh;
class QImage;
class Movie;
class QString;

QImage warp(const QImage & img, const Mesh & src_mesh, const Mesh & dst_mesh);


QImage morphFrame(const QImage & src_img, const Mesh & src_mesh,
                  const QImage & dst_img, const Mesh & dst_mesh,
                  Mesh & tmp_mesh, float t);

void morph(const QImage & src_img, const Mesh & src_mesh,
           const QImage & dst_img, const Mesh & dst_mesh,
           int nframes);

#if 0
Movie *morphMovie(const QImage & src_img, const Mesh & src_mesh,
                  const QImage & dst_img, const Mesh & dst_mesh,
                  int nframes);
#endif

bool loadMorphProject(const QString & filename,
                      QImage & src_img, Mesh & src_mesh,
                      QImage & dst_img, Mesh & dst_mesh);

bool saveMorphProject(const QString & filename,
                      const QImage & src_img, const Mesh & src_mesh,
                      const QImage & dst_img, const Mesh & dst_mesh);

#endif // MORPH_H
