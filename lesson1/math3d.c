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

static GLfloat mat4_get(const mat4* m, int x, int y) {
	return m->data[y + 4 * x]; /* matrices in OpenGL are in column-major order! */
}

static void mat4_set(mat4* m, int x, int y, GLfloat value) {
	m->data[y + 4 * x] = value; /* matrices in OpenGL are in column-major order! */
}

/* interface */

void mat4_identity(mat4* m) {
	memcpy(m->data, identityMatrix, sizeof(identityMatrix)); /* TODO good style (sizeof)? */
}

/* from https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml */
void mat4_perspective(mat4* m, GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
	GLfloat f = 1 / tan(fovy / 2.0); /* = cotangent(fovy / 2f) */

	/* row 0 */
	mat4_set(m, 0, 0, f / aspect);
	mat4_set(m, 1, 0,       0.0f);
	mat4_set(m, 2, 0,       0.0f);
	mat4_set(m, 3, 0,       0.0f);

	/* row 1 */
	mat4_set(m, 0, 1, 0.0f);
	mat4_set(m, 1, 1,    f);
	mat4_set(m, 2, 1, 0.0f);
	mat4_set(m, 3, 1, 0.0f);

	/* row 2 */
	mat4_set(m, 0, 2,                                  0.0f);
	mat4_set(m, 1, 2,                                  0.0f);
	mat4_set(m, 2, 2,       (zFar + zNear) / (zNear - zFar));
	mat4_set(m, 3, 2, (2.0 * zFar * zNear) / (zNear - zFar));

	/* row 3 */
	mat4_set(m, 0, 3,  0.0f);
	mat4_set(m, 1, 3,  0.0f);
	mat4_set(m, 2, 3, -1.0f);
	mat4_set(m, 3, 3,  0.0f);
}

void mat4_translate(mat4* m, GLfloat x, GLfloat y, GLfloat z) {
	mat4_set(m, 3, 0, mat4_get(m, 3, 0) + x);
	mat4_set(m, 3, 1, mat4_get(m, 3, 1) + y);
	mat4_set(m, 3, 2, mat4_get(m, 3, 2) + z);
}

void mat4_multiply(mat4* result, mat4* m1, mat4* m2) {
	int x, y;

	for (y = 0; y < 4; ++y) {
		for (x = 0; x < 4; ++x) {
			mat4_set(
				result, x, y,
				mat4_get(m1, 0, y) * mat4_get(m2, x, 0) +
				mat4_get(m1, 1, y) * mat4_get(m2, x, 1) +
				mat4_get(m1, 2, y) * mat4_get(m2, x, 2) +
				mat4_get(m1, 3, y) * mat4_get(m2, x, 3)
			);
		}
	}
}

void mat4_print(mat4* m) {
	printf("[%.2f %.2f %.2f %.2f]\n", m->data[0], m->data[4], m->data[8],  m->data[12]);
	printf("[%.2f %.2f %.2f %.2f]\n", m->data[1], m->data[5], m->data[9],  m->data[13]);
	printf("[%.2f %.2f %.2f %.2f]\n", m->data[2], m->data[6], m->data[10], m->data[14]);
	printf("[%.2f %.2f %.2f %.2f]\n", m->data[3], m->data[7], m->data[11], m->data[15]);
	printf("\n");
}
