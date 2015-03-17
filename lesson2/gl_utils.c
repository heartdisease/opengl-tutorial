/** This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include "gl_utils.h"

#include <stdio.h>
#include <stdlib.h>

void printProgramInfoLog(GLProgram* program) {
	GLchar* logMessage;
	GLint logLength;

	glGetProgramiv(program->id, GL_INFO_LOG_LENGTH, &logLength);

	logMessage = (GLchar*) malloc(sizeof(GLchar) * logLength);
	glGetProgramInfoLog(program->id, logLength, NULL, logMessage);

	fprintf(stderr, "%s\n", logMessage);
	free(logMessage);
}

void checkProgramLinkStatus(GLProgram* program) {
	GLint status = GL_FALSE;

	glGetProgramiv(program->id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		printProgramInfoLog(program);
	}
}

/* TODO: review for buffer overflows */
GLchar* getShaderSource(const char* sourceFile) {
	long size;
	GLchar* source;
	FILE* handle = fopen(sourceFile, "rb");

	fseek(handle, 0, SEEK_END);
	size = ftell(handle);
	rewind(handle);
	source = (GLchar*) malloc(sizeof(GLchar) * (size + 1));
	fread(source, sizeof(GLchar), size, handle);
	source[size] = '\0';
	fclose(handle);

	return source;
}

void printShaderInfoLog(GLShader* shader) {
	GLchar* logMessage;
	GLint logLength;

	glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &logLength);

	logMessage = (GLchar*) malloc(sizeof(GLchar) * logLength);
	glGetShaderInfoLog(shader->id, logLength, NULL, logMessage);

	fprintf(stderr, "%s\n", logMessage);
	free(logMessage);
}

void checkShaderCompileStatus(GLShader* shader) {
	GLint status = GL_FALSE;

	glGetShaderiv(shader->id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		printShaderInfoLog(shader);
	}
}
