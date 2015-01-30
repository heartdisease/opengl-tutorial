#include "gl_program.h"

#include <stdio.h>
#include <stdlib.h>

static void print_info_log(gl_program* program) {
	GLchar* log_message;
	GLint log_length;

	glGetProgramiv(program->id, GL_INFO_LOG_LENGTH, &log_length);

	log_message = (GLchar*) malloc(sizeof(GLchar) * log_length);
	glGetProgramInfoLog(program->id, log_length, NULL, log_message);

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

/* interface */

void create_program(gl_program* program) {
	program->id = glCreateProgram();
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

GLint get_attrib_location(gl_program* program, const GLchar* name) {
	return glGetAttribLocation(program->id, name);
}

GLint get_uniform_location(gl_program* program, const GLchar* name) {
	return glGetUniformLocation(program->id, name);
}

void upload_mat4(gl_program* program, GLint uniform_location, mat4* m) {
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, m->data);
}

void delete_program(gl_program* program) {
	glDeleteProgram(program->id);
}
