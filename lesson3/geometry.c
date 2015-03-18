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
	/* lesson2 */
	geom->colors = (GLfloat*) malloc(sizeof(GLfloat) * geom->itemSize * geom->numItems);
}

static void setVertex(Geometry* geom, int i, GLfloat x, GLfloat y, GLfloat z) {
	int baseIndex = i * geom->itemSize;

	geom->vertices[baseIndex]     = x;
	geom->vertices[baseIndex + 1] = y;
	geom->vertices[baseIndex + 2] = z;
}

static void setColor(Geometry* geom, int i, GLfloat r, GLfloat g, GLfloat b) {
	int baseIndex = i * geom->itemSize;

	geom->colors[baseIndex]     = r;
	geom->colors[baseIndex + 1] = g;
	geom->colors[baseIndex + 2] = b;
}

void createTriangle(Geometry* geom) {
	init(geom, 3);

	/* top-center */
	setVertex(geom, 0,  0.0f,  1.0f, 0.0f);
	setColor(geom, 0, 1.0f, 0.0f, 0.0f);
	/* bottom-right */
	setVertex(geom, 1,  1.0f, -1.0f, 0.0f);
	setColor(geom, 1, 0.0f, 0.0f, 1.0f);
	/* bottom-left */
	setVertex(geom, 2, -1.0f, -1.0f, 0.0f);
	setColor(geom, 2, 0.0f, 1.0f, 0.0f);
}

void createQuad(Geometry* geom) {
	init(geom, 5);

	/* Top Left */
	setVertex(geom, 0, -1.0f,  1.0f, 0.0f);
	setColor(geom, 0, 0.5f, 0.5f, 1.0f);
	/* Top Right */
	setVertex(geom, 1,  1.0f,  1.0f, 0.0f);
	setColor(geom, 1, 0.5f, 0.5f, 1.0f);
	/* Bottom Right */
	setVertex(geom, 2,  1.0f, -1.0f, 0.0f);
	setColor(geom, 2, 0.5f, 0.5f, 1.0f);
	/* Bottom Left */
	setVertex(geom, 3, -1.0f, -1.0f, 0.0f);
	setColor(geom, 3, 0.5f, 0.5f, 1.0f);
	/* Top Left */
	setVertex(geom, 4, -1.0f,  1.0f, 0.0f);
	setColor(geom, 4, 0.5f, 0.5f, 1.0f);
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
	free(geom->colors);

	geom->vertices = NULL;
	geom->colors = NULL;
}
