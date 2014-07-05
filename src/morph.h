/*
 * Copyright (C) 2014 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
 *
 * This file is part of ZPO.
 *
 * ZPO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ZPO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ZPO. If not, see <http://www.gnu.org/licenses/>.
 */

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
