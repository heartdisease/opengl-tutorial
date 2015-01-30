#ifndef __GL_PROGRAM_H
#define __GL_PROGRAM_H

#include <GL/glew.h>

#include "gl_shader.h"

typedef struct gl_program {
    GLuint id;
    GLint attrib_pointer0;
} gl_program;

void create_program(gl_program* program);

void attach_shader(gl_program* program, gl_shader* shader);

void link_program(gl_program* program);

void use_program(gl_program* program);

void request_attrib_location(gl_program* program, const GLchar* attrib_name);

void delete_program(gl_program* program);

#endif // __GL_PROGRAM_H
