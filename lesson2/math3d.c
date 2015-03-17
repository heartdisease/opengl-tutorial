/** This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include "math3d.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* row-major order = column-major order for identity matrix */
static GLfloat identityMatrix[] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

static GLfloat get(const Mat4* m, int x, int y) {
	return m->data[y + 4 * x]; /* matrices in OpenGL are in column-major order! */
}

static void set(Mat4* m, int x, int y, GLfloat value) {
	m->data[y + 4 * x] = value; /* matrices in OpenGL are in column-major order! */
}

/* interface */

void mat4Identity(Mat4* m) {
	memcpy(m->data, identityMatrix, sizeof(identityMatrix)); /* TODO good style (sizeof)? */
}

/* from https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml */
void mat4Perspective(Mat4* m, GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
	GLfloat f = 1 / tan(fovy / 2.0); /* = cotangent(fovy / 2f) */

	/* row 0 */
	set(m, 0, 0, f / aspect);
	set(m, 1, 0,       0.0f);
	set(m, 2, 0,       0.0f);
	set(m, 3, 0,       0.0f);

	/* row 1 */
	set(m, 0, 1, 0.0f);
	set(m, 1, 1,    f);
	set(m, 2, 1, 0.0f);
	set(m, 3, 1, 0.0f);

	/* row 2 */
	set(m, 0, 2,                                  0.0f);
	set(m, 1, 2,                                  0.0f);
	set(m, 2, 2,       (zFar + zNear) / (zNear - zFar));
	set(m, 3, 2, (2.0 * zFar * zNear) / (zNear - zFar));

	/* row 3 */
	set(m, 0, 3,  0.0f);
	set(m, 1, 3,  0.0f);
	set(m, 2, 3, -1.0f);
	set(m, 3, 3,  0.0f);
}

void mat4Translate(Mat4* m, GLfloat x, GLfloat y, GLfloat z) {
	set(m, 3, 0, get(m, 3, 0) + x);
	set(m, 3, 1, get(m, 3, 1) + y);
	set(m, 3, 2, get(m, 3, 2) + z);
}

void mat4Multiply(Mat4* result, Mat4* m1, Mat4* m2) {
	int x, y;

	for (y = 0; y < 4; ++y) {
		for (x = 0; x < 4; ++x) {
			set(
				result, x, y,
				get(m1, 0, y) * get(m2, x, 0) +
				get(m1, 1, y) * get(m2, x, 1) +
				get(m1, 2, y) * get(m2, x, 2) +
				get(m1, 3, y) * get(m2, x, 3)
			);
		}
	}
}

void mat4Print(Mat4* m) {
	printf("[%.2f %.2f %.2f %.2f]\n", m->data[0], m->data[4], m->data[8],  m->data[12]);
	printf("[%.2f %.2f %.2f %.2f]\n", m->data[1], m->data[5], m->data[9],  m->data[13]);
	printf("[%.2f %.2f %.2f %.2f]\n", m->data[2], m->data[6], m->data[10], m->data[14]);
	printf("[%.2f %.2f %.2f %.2f]\n", m->data[3], m->data[7], m->data[11], m->data[15]);
	printf("\n");
}
