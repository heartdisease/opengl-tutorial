#ifndef __GEOMETRY_H
#define __GEOMETRY_H
/** This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <GL/glew.h>

typedef struct Geometry {
	GLint itemSize;    /* number of components per generic vertex attribute. Must be 1, 2, 3 or 4 */
	GLsizei numItems;  /* number of vertices (e.g. 3 for a triangle) */
	GLfloat* colors;   /* lesson2 - array containg RGB color codes for each vertex */
	GLfloat* vertices; /* array containing all vertices */
} Geometry;

void createTriangle(Geometry* geom);

void createQuad(Geometry* geom);

void printGeometry(Geometry* geom);

void deleteGeometry(Geometry* geom);

#endif
