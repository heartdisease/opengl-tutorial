#ifndef __GL_UTILS_H
#define __GL_UTILS_H

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
