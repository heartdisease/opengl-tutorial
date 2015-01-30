#ifndef __GL_PROGRAM_H
#define __GL_PROGRAM_H

#include <GL/glew.h>

#include "math3d.h"
#include "gl_shader.h"

typedef struct gl_program {
    GLuint id;
} gl_program;

void create_program(gl_program* program);

void attach_shader(gl_program* program, gl_shader* shader);

void link_program(gl_program* program);

void use_program(gl_program* program);

GLint get_attrib_location(gl_program* program, const GLchar* name);

GLint get_uniform_location(gl_program* program, const GLchar* name);

void upload_mat4(gl_program* program, GLint uniform_location, mat4* m);

void delete_program(gl_program* program);

#endif
