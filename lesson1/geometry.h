#ifndef __GEOMETRY_H
#define __GEOMETRY_H

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
