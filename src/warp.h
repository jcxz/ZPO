#ifndef WARP_H
#define WARP_H

class Mesh;
class QImage;

QImage warp(const QImage & img, const Mesh & src_mesh, const Mesh & dst_mesh);

#endif // WARP_H
