#ifndef __GEOMETRY_H
#define __GEOMETRY_H

#include <GL/glew.h>

typedef struct geometry {
	GLint size;        /* number of components per generic vertex attribute. Must be 1, 2, 3 or 4 */
	GLsizei length;    /* number of vertices (e.g. 3 for a triangle) */
	GLenum mode;       /* rendering mode in which the vertices should be drawn (see glDrawArrays()) */
	GLfloat* vertices; /* array containing all vertices */
} geometry;

void create_triangle(geometry* geom);

void create_quad(geometry* geom);

void print_geometry(geometry* geom);

void delete_geometry(geometry* geom);

#endif
