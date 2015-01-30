#include "geometry.h"

#include <stdio.h>
#include <stdlib.h>

static void init_geometry(geometry* geom, GLsizei length, GLenum mode) {
	geom->size = 3;
	geom->length = length;
	geom->mode = mode;
	geom->vertices = (GLfloat*) malloc(sizeof(GLfloat) * geom->size * geom->length);
}

static void set_vertex(geometry* geom, int i, GLfloat x, GLfloat y, GLfloat z) {
	int base_index = i * geom->size;

	geom->vertices[base_index] = x;
	geom->vertices[base_index + 1] = y;
	geom->vertices[base_index + 2] = z;
}

void create_triangle(geometry* geom) {
	init_geometry(geom, 3, GL_TRIANGLE_STRIP);
	set_vertex(geom, 0,  0.0f,  1.0f, 0.0f); // top-center
	set_vertex(geom, 1, -1.0f, -1.0f, 0.0f); // bottom-left (???)
	set_vertex(geom, 2,  1.0f, -1.0f, 0.0f); // bottom-right (???)
}

void create_quad(geometry* geom) {
	init_geometry(geom, 5, GL_TRIANGLE_STRIP);
	set_vertex(geom, 0, -1.0f,  1.0f, 0.0f); // Top Left
	set_vertex(geom, 1,  1.0f,  1.0f, 0.0f); // Top Right
	set_vertex(geom, 2,  1.0f, -1.0f, 0.0f); // Bottom Right
	set_vertex(geom, 3, -1.0f, -1.0f, 0.0f); // Bottom Left
	set_vertex(geom, 4, -1.0f,  1.0f, 0.0f); // Top Left
}

void print_geometry(geometry* geom) {
	int i, v;

	printf("geometry{");
	for (i = 0; i < geom->length; ++i) {
		if (i > 0) {
			printf(", ");
		}
		printf("[");
		for (v = 0; v < geom->size; ++v) {
			if (v > 0) {
				printf(", ");
			}
			printf("%.2f", geom->vertices[i * geom->size + v]);
		}
		printf("]");
	}
	printf("}\n");
}

void delete_geometry(geometry* geom) {
	free(geom->vertices);
	geom->vertices = NULL;
}
