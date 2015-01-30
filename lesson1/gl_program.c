#include "gl_program.h"

#include <stdio.h>
#include <stdlib.h>

static void print_info_log(gl_program* program) {
	GLchar* log_message;
	GLint log_length;

	glGetProgramiv(program->id, GL_INFO_LOG_LENGTH, &log_length);

	log_message = (GLchar*) malloc(sizeof(GLchar) * (log_length + 1)); // includes '\0' or not??
	glGetProgramInfoLog(program->id, log_length, NULL, log_message);
	log_message[log_length] = '\0';

	fprintf(stderr, "%s\n", log_message);
	free(log_message);
}

static void check_link_status(gl_program* program) {
	GLint status = GL_FALSE;

	glGetProgramiv(program->id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		print_info_log(program);
	}
}

void create_program(gl_program* program) {
	program->id = glCreateProgram();
	program->attrib_pointer0 = -1;
}

void attach_shader(gl_program* program, gl_shader* shader) {
	glAttachShader(program->id, shader->id);
}

void link_program(gl_program* program) {
	glLinkProgram(program->id);
	check_link_status(program);
}

void use_program(gl_program* program) {
	GLenum error;

	glUseProgram(program->id);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		print_info_log(program);
	}
}

void request_attrib_location(gl_program* program, const GLchar* attrib_name) {
	program->attrib_pointer0 = glGetAttribLocation(program->id, attrib_name);
}

void delete_program(gl_program* program) {
	glDeleteProgram(program->id);
}
