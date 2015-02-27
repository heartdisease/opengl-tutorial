#ifndef __MATH3D_H
#define __MATH3D_H

#include <GL/glew.h>

typedef struct Mat4 {
    GLfloat data[16];
} Mat4;

void mat4Identity(Mat4* result);

void mat4Perspective(Mat4* result, GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

void mat4Translate(Mat4* m, GLfloat x, GLfloat y, GLfloat z);

void mat4Multiply(Mat4* result, Mat4* m1, Mat4* m2);

void mat4Print(Mat4* result);

#endif
