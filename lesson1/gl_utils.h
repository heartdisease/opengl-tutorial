#ifndef __GL_UTILS_H
#define __GL_UTILS_H
/** This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <GL/glew.h>

typedef struct GLVertexArray {
	GLuint id;
} GLVertexArray;

typedef struct GLBuffer {
	GLuint  id;
	GLenum  type;
	GLenum  mode;
	GLint   size;
	GLsizei length;
} GLBuffer;

typedef struct GLShader {
    GLuint id;
} GLShader;

typedef struct GLProgram {
    GLuint id;
} GLProgram;


GLchar* getShaderSource(const char* sourceFile);

void printShaderInfoLog(GLShader* shader);

void checkShaderCompileStatus(GLShader* shader);

void printProgramInfoLog(GLProgram* program);

void checkProgramLinkStatus(GLProgram* program);

#endif
