#include "gl_utils.h"

#include <stdio.h>
#include <stdlib.h>

void printProgramInfoLog(GLProgram* program) {
	GLchar* log_message;
	GLint log_length;

	glGetProgramiv(program->id, GL_INFO_LOG_LENGTH, &log_length);

	log_message = (GLchar*) malloc(sizeof(GLchar) * log_length);
	glGetProgramInfoLog(program->id, log_length, NULL, log_message);

	fprintf(stderr, "%s\n", log_message);
	free(log_message);
}

void checkProgramLinkStatus(GLProgram* program) {
	GLint status = GL_FALSE;

	glGetProgramiv(program->id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		printProgramInfoLog(program);
	}
}

/* TODO: review! */
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
	GLchar* log_message;
	GLint log_length;

	glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &log_length);

	log_message = (GLchar*) malloc(sizeof(GLchar) * log_length);
	glGetShaderInfoLog(shader->id, log_length, NULL, log_message);

	fprintf(stderr, "%s\n", log_message);
	free(log_message);
}

void checkShaderCompileStatus(GLShader* shader) {
	GLint status = GL_FALSE;

	glGetShaderiv(shader->id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		printShaderInfoLog(shader);
	}
}
