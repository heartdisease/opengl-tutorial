/** This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include "geometry.h"

#include <stdio.h>
#include <stdlib.h>

static void init(Geometry* geom, GLsizei numItems) {
	geom->itemSize = 3;
	geom->numItems = numItems;
	geom->vertices = (GLfloat*) malloc(sizeof(GLfloat) * geom->itemSize * geom->numItems);
}

static void setVertex(Geometry* geom, int i, GLfloat x, GLfloat y, GLfloat z) {
	int baseIndex = i * geom->itemSize;

	geom->vertices[baseIndex]     = x;
	geom->vertices[baseIndex + 1] = y;
	geom->vertices[baseIndex + 2] = z;
}

void createTriangle(Geometry* geom) {
	init(geom, 3);

	setVertex(geom, 0,  0.0f,  1.0f, 0.0f); /* top-center */
	setVertex(geom, 1,  1.0f, -1.0f, 0.0f); /* bottom-right */
	setVertex(geom, 2, -1.0f, -1.0f, 0.0f); /* bottom-left */
}

void createQuad(Geometry* geom) {
	init(geom, 5);

	setVertex(geom, 0, -1.0f,  1.0f, 0.0f); /* Top Left */
	setVertex(geom, 1,  1.0f,  1.0f, 0.0f); /* Top Right */
	setVertex(geom, 2,  1.0f, -1.0f, 0.0f); /* Bottom Right */
	setVertex(geom, 3, -1.0f, -1.0f, 0.0f); /* Bottom Left */
	setVertex(geom, 4, -1.0f,  1.0f, 0.0f); /* Top Left */
}

void printGeometry(Geometry* geom) {
	int i, v;

	printf("Geometry{");
	for (i = 0; i < geom->numItems; ++i) {
		if (i > 0) {
			printf(", ");
		}
		printf("[");
		for (v = 0; v < geom->itemSize; ++v) {
			if (v > 0) {
				printf(", ");
			}
			printf("%.2f", geom->vertices[i * geom->itemSize + v]);
		}
		printf("]");
	}
	printf("}\n");
}

void deleteGeometry(Geometry* geom) {
	free(geom->vertices);
	geom->vertices = NULL;
}
