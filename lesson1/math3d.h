#ifndef __MATH3D_H
#define __MATH3D_H

#include <GL/glew.h>

typedef struct mat4 {
    GLfloat data[16];
} mat4;

void mat4_identity(mat4* result);

void mat4_perspective(mat4* result, GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

void mat4_translate(mat4* m, GLfloat x, GLfloat y, GLfloat z);

void mat4_multiply(mat4* result, mat4* m1, mat4* m2);

void mat4_print(mat4* result);

#endif // __MATH3D_H
